#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define V 10  // 정점의 개수
#define E 20  // 간선의 개수
#define INF -1  // 경로가 없음을 나타냄

// BFS를 위한 큐 구조체
typedef struct {
    int items[V];
    int front;
    int rear;
} Queue;

// 큐 초기화
void initQueue(Queue* q) {
    q->front = -1;
    q->rear = -1;
}

// 큐가 비어있는지 확인
bool isEmpty(Queue* q) {
    return q->front == -1;
}

// 큐에 원소 추가
void enqueue(Queue* q, int value) {
    if (q->front == -1)
        q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
}

// 큐에서 원소 제거
int dequeue(Queue* q) {
    int item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
        q->front = q->rear = -1;
    }
    return item;
}

// 무작위 그래프 생성
void createRandomGraph(int graph[V][V]) {
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            graph[i][j] = 0;
        }
    }
    
    int edgeCount = 0;
    srand(time(NULL));
    
    while (edgeCount < E) {
        int u = rand() % V;
        int v = rand() % V;
        
        if (u != v && graph[u][v] == 0) {
            graph[u][v] = 1;
            graph[v][u] = 1;  
            edgeCount++;
        }
    }
}

void BFS(int graph[V][V], int src, int dist[], int parent[]) {
    bool visited[V];
    Queue q;
    
    // 초기화
    for (int i = 0; i < V; i++) {
        visited[i] = false;
        dist[i] = INF;
        parent[i] = -1;  
    }
    
    initQueue(&q);
    visited[src] = true;
    dist[src] = 0;
    enqueue(&q, src);
    
    // BFS 수행
    while (!isEmpty(&q)) {
        int u = dequeue(&q);
        
        for (int v = 0; v < V; v++) {
            if (graph[u][v] == 1 && !visited[v]) {
                visited[v] = true;
                dist[v] = dist[u] + 1;
                parent[v] = u; 
                enqueue(&q, v);
            }
        }
    }
}

void printPath(int parent[], int src, int dest) {
    if (dest == src) {
        printf("%d", src);
        return;
    }
    
    if (parent[dest] == -1) {
        printf("경로 없음");
        return;
    }
    
    printPath(parent, src, parent[dest]);
    printf(" -> %d", dest);
}

// 그래프 출력
void printGraph(int graph[V][V]) {
    printf("그래프의 인접 행렬:\n");
    printf("   ");
    for (int i = 0; i < V; i++) {
        printf("%2d ", i);
    }
    printf("\n");
    
    for (int i = 0; i < V; i++) {
        printf("%2d:", i);
        for (int j = 0; j < V; j++) {
            printf("%2d ", graph[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    // 간선 목록 출력
    printf("그래프의 간선 목록:\n");
    int edgeCount = 0;
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (graph[i][j] == 1) {
                edgeCount++;
                printf("간선 %2d: %d - %d\n", edgeCount, i, j);
            }
        }
    }
    printf("총 간선 개수: %d\n\n", edgeCount);
}

int main() {
    int graph[V][V];
    int dist[V];
    int parent[V];
    
    // 무작위 그래프 생성
    createRandomGraph(graph);
    
    // 그래프 출력
    printGraph(graph);
    
    // 모든 쌍의 최단 경로 계산 및 출력
    printf("모든 정점 쌍 간의 최단 경로:\n");
    printf("=================================================================\n\n");
    
    int pairCount = 0;
    for (int i = 0; i < V; i++) {
        BFS(graph, i, dist, parent);
        
        for (int j = i + 1; j < V; j++) {
            pairCount++;
            printf("쌍 %2d: 정점 %d -> 정점 %d\n", pairCount, i, j);
            
            if (dist[j] == INF) {
                printf("  결과: 경로 없음\n");
            } else {
                printf("  거리: %d 개의 간선\n", dist[j]);
                printf("  경로: ");
                printPath(parent, i, j);
                printf("\n");
            }
            printf("\n");
        }
    }
    
    printf("=================================================================\n");
    printf("총 정점 쌍의 개수: %d\n", pairCount);
    
    return 0;
}