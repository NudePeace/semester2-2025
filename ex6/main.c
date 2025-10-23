#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 

#define MAX_VERTICES 100    
#define SPARSE_EDGES 100   
#define DENSE_EDGES 4000   

// ==================== 자료구조 정의 ====================

typedef struct {
    int matrix[MAX_VERTICES][MAX_VERTICES];
    int num_vertices;
} AdjMatrix;

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct {
    Node** array;
    int num_vertices;
} AdjList;

// ==================== 생성 및 해제 함수 ====================

AdjMatrix* create_adj_matrix(int vertices) {
    AdjMatrix* graph = (AdjMatrix*)malloc(sizeof(AdjMatrix));
    if (!graph) return NULL;
    graph->num_vertices = vertices;
    memset(graph->matrix, 0, sizeof(graph->matrix));
    return graph;
}

AdjList* create_adj_list(int vertices) {
    AdjList* graph = (AdjList*)malloc(sizeof(AdjList));
    if (!graph) return NULL;
    graph->num_vertices = vertices;
    graph->array = (Node**)malloc(vertices * sizeof(Node*));
    if (!graph->array) {
        free(graph);
        return NULL;
    }
    for (int i = 0; i < vertices; i++) {
        graph->array[i] = NULL;
    }
    return graph;
}

Node* create_node(int vertex) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return NULL;
    new_node->vertex = vertex;
    new_node->next = NULL;
    return new_node;
}

void free_adj_list(AdjList* graph) {
    if (!graph) return;
    for(int i = 0; i < graph->num_vertices; i++) {
        Node* temp = graph->array[i];
        while(temp) {
            Node* to_free = temp;
            temp = temp->next;
            free(to_free);
        }
    }
    free(graph->array);
    free(graph);
}

// ==================== 인접 행렬 연산 및 비교 횟수 측정 ====================

int matrix_add_edge(AdjMatrix* graph, int src, int dest) {
    if (src >= 0 && src < graph->num_vertices && dest >= 0 && dest < graph->num_vertices) {
        graph->matrix[src][dest] = 1;
        graph->matrix[dest][src] = 1;
    }
    return 1;
}

int matrix_remove_edge(AdjMatrix* graph, int src, int dest) {
    if (src >= 0 && src < graph->num_vertices && dest >= 0 && dest < graph->num_vertices) {
        graph->matrix[src][dest] = 0;
        graph->matrix[dest][src] = 0;
    }
    return 1;
}

int matrix_is_connected(AdjMatrix* graph, int src, int dest) {
    int comparison_count = 1; 
    if (src >= 0 && src < graph->num_vertices && dest >= 0 && dest < graph->num_vertices) {
        comparison_count++; 
    }
    return comparison_count;
}

int matrix_print_neighbors(AdjMatrix* graph, int vertex) {
    int comparison_count = 1;
    if (vertex >= 0 && vertex < graph->num_vertices) {
        for (int i = 0; i < graph->num_vertices; i++) {
            comparison_count++; 
            comparison_count++; //
        }
    }
    return comparison_count;
}

size_t matrix_memory_usage(AdjMatrix* graph) {
    return sizeof(AdjMatrix);
}


// ==================== 인접 리스트 연산 및 비교 횟수 측정 ====================

int list_add_edge(AdjList* graph, int src, int dest) {
    int comparison_count = 1; 
    if (src >= 0 && src < graph->num_vertices && dest >= 0 && dest < graph->num_vertices) {
        Node* new_node_dest = create_node(dest);
        Node* new_node_src = create_node(src);
        
        if (new_node_dest) {
            new_node_dest->next = graph->array[src];
            graph->array[src] = new_node_dest;
        }

        if (new_node_src) {
            new_node_src->next = graph->array[dest];
            graph->array[dest] = new_node_src;
        }
    }
    return comparison_count;
}

int remove_node_from_list(Node** head_ref, int target) {
    Node* temp = *head_ref;
    Node* prev = NULL;
    int count = 0;
    while (temp != NULL) {
        count++; 
        count++; 
        if (temp->vertex == target) {
            count++; 
            if (prev == NULL)
                *head_ref = temp->next;
            else
                prev->next = temp->next;
            free(temp);
            return count; 
        }
        prev = temp;
        temp = temp->next;
    }
    count++; 
    return count; 
}

int list_remove_edge(AdjList* graph, int src, int dest) {
    int comparison_count = 1;
    if (src >= 0 && src < graph->num_vertices && dest >= 0 && dest < graph->num_vertices) {
        comparison_count += remove_node_from_list(&graph->array[src], dest);
        comparison_count += remove_node_from_list(&graph->array[dest], src);
    }
    return comparison_count;
}

int list_is_connected(AdjList* graph, int src, int dest, int* out_comparison) {
    int comparison_count = 1; 
    int is_connected = 0;
    if (src >= 0 && src < graph->num_vertices && dest >= 0 && dest < graph->num_vertices) {
        Node* temp = graph->array[src];
        while (temp != NULL) {
            comparison_count++; 
            comparison_count++; 
            if (temp->vertex == dest) {
                is_connected = 1;
                break;
            }
            temp = temp->next;
        }
        comparison_count++; 
    }
    *out_comparison = comparison_count;
    return is_connected;
}

void list_print_neighbors(AdjList* graph, int vertex, int* out_comparison) {
    int comparison_count = 1; 
    if (vertex >= 0 && vertex < graph->num_vertices) {
        Node* temp = graph->array[vertex];
        while (temp != NULL) {
            comparison_count++; 
            temp = temp->next;
        }
        comparison_count++; 
    }
    *out_comparison = comparison_count;
}

size_t list_memory_usage(AdjList* graph) {
    size_t total = sizeof(AdjList); 
    total += graph->num_vertices * sizeof(Node*); 

    for (int i = 0; i < graph->num_vertices; i++) {
        Node* temp = graph->array[i];
        while (temp != NULL) {
            total += sizeof(Node);
            temp = temp->next;
        }
    }
    return total;
}

// ==================== 랜덤 그래프 간선 생성 ====================
void generate_random_edges(int vertices, int num_edges, int edges[][2]) {
    int count = 0;
    while (count < num_edges) {
        int src = rand() % vertices;
        int dest = rand() % vertices;

        if (src == dest) continue; 

        if (src > dest) {
            int temp = src;
            src = dest;
            dest = temp;
        }
        
        int duplicate = 0;
        for (int i = 0; i < count; i++) {
            if (edges[i][0] == src && edges[i][1] == dest) {
                duplicate = 1;
                break;
            }
        }

        if (!duplicate) {
            edges[count][0] = src;
            edges[count][1] = dest;
            count++;
        }
    }
}

// ==================== 결과 출력 함수 ====================
void print_result_korean(const char* case_name, size_t memory,
                  int insert_comp_add, int insert_comp_del,
                  int connect_comp, int neighbor_comp) {
    printf("--------------------------------------------------\n");
    printf("%s\n", case_name);
    printf("1. 그래프 사용 메모리 용량: %zu Bytes\n", memory);
    printf("2. 간선 삽입/삭제 시 비교 횟수:\n");
    printf("   - 간선 삽입 비교 횟수 (%d개 간선): %d 번\n", (strstr(case_name, "희소") ? SPARSE_EDGES : DENSE_EDGES), insert_comp_add);
    printf("   - 간선 삭제 비교 횟수: %d 번\n", insert_comp_del);
    printf("3. 두 정점의 연결 여부 확인 비교 횟수: %d 번\n", connect_comp);
    printf("4. 특정 노드의 인접 노드 출력 비교 횟수: %d 번\n\n", neighbor_comp);
}

// ==================== 메인 함수 ====================
int main() {
    srand(time(NULL)); 
    
    printf("=== 그래프 표현 방식별 성능 비교 (정점 V=100) ===\n\n");
    
    static int sparse_edges[SPARSE_EDGES][2];
    static int dense_edges[DENSE_EDGES][2];
    
    generate_random_edges(MAX_VERTICES, SPARSE_EDGES, sparse_edges);
    generate_random_edges(MAX_VERTICES, DENSE_EDGES, dense_edges);
    
    // --------------------------------------------------
    // --- 1. 희소 그래프 - 인접 행렬 (V=100, E=100) ---
    // --------------------------------------------------
    AdjMatrix* sparse_matrix = create_adj_matrix(MAX_VERTICES);
    if (!sparse_matrix) return 1;

    int insert_comp_1 = 0;
    for (int i = 0; i < SPARSE_EDGES; i++)
        insert_comp_1 += matrix_add_edge(sparse_matrix, sparse_edges[i][0], sparse_edges[i][1]);
    
    int connect_comp_1 = matrix_is_connected(sparse_matrix, 0, 1);
    int neighbor_comp_1 = matrix_print_neighbors(sparse_matrix, 0);
    int delete_comp_1 = matrix_remove_edge(sparse_matrix, sparse_edges[0][0], sparse_edges[0][1]);
    size_t memory_1 = matrix_memory_usage(sparse_matrix);
    
    print_result_korean("케이스 1: 희소 그래프 - 인접 행렬", memory_1, insert_comp_1, delete_comp_1, connect_comp_1, neighbor_comp_1);
    free(sparse_matrix);

    // --------------------------------------------------
    // --- 2. 희소 그래프 - 인접 리스트 (V=100, E=100) ---
    // --------------------------------------------------
    AdjList* sparse_list = create_adj_list(MAX_VERTICES);
    if (!sparse_list) return 1;

    int insert_comp_2 = 0;
    for (int i = 0; i < SPARSE_EDGES; i++)
        insert_comp_2 += list_add_edge(sparse_list, sparse_edges[i][0], sparse_edges[i][1]);
        
    int connect_comp_2 = 0;
    list_is_connected(sparse_list, 0, 1, &connect_comp_2);
    int neighbor_comp_2 = 0;
    list_print_neighbors(sparse_list, 0, &neighbor_comp_2);
    int delete_comp_2 = list_remove_edge(sparse_list, sparse_edges[0][0], sparse_edges[0][1]);
    size_t memory_2 = list_memory_usage(sparse_list);
    
    print_result_korean("케이스 2: 희소 그래프 - 인접 리스트", memory_2, insert_comp_2, delete_comp_2, connect_comp_2, neighbor_comp_2);
    free_adj_list(sparse_list);

    // --------------------------------------------------
    // --- 3. 밀집 그래프 - 인접 행렬 (V=100, E=4000) ---
    // --------------------------------------------------
    AdjMatrix* dense_matrix = create_adj_matrix(MAX_VERTICES);
    if (!dense_matrix) return 1;

    int insert_comp_3 = 0;
    for (int i = 0; i < DENSE_EDGES; i++)
        insert_comp_3 += matrix_add_edge(dense_matrix, dense_edges[i][0], dense_edges[i][1]);
        
    int connect_comp_3 = matrix_is_connected(dense_matrix, 0, 1);
    int neighbor_comp_3 = matrix_print_neighbors(dense_matrix, 0);
    int delete_comp_3 = matrix_remove_edge(dense_matrix, dense_edges[0][0], dense_edges[0][1]);
    size_t memory_3 = matrix_memory_usage(dense_matrix);
    
    print_result_korean("케이스 3: 밀집 그래프 - 인접 행렬", memory_3, insert_comp_3, delete_comp_3, connect_comp_3, neighbor_comp_3);
    free(dense_matrix);

    // --------------------------------------------------
    // --- 4. 밀집 그래프 - 인접 리스트 (V=100, E=4000) ---
    // --------------------------------------------------
    AdjList* dense_list = create_adj_list(MAX_VERTICES);
    if (!dense_list) return 1;

    int insert_comp_4 = 0;
    for (int i = 0; i < DENSE_EDGES; i++)
        insert_comp_4 += list_add_edge(dense_list, dense_edges[i][0], dense_edges[i][1]);
        
    int connect_comp_4 = 0;
    list_is_connected(dense_list, 0, 1, &connect_comp_4);
    int neighbor_comp_4 = 0;
    list_print_neighbors(dense_list, 0, &neighbor_comp_4);
    int delete_comp_4 = list_remove_edge(dense_list, dense_edges[0][0], dense_edges[0][1]);
    size_t memory_4 = list_memory_usage(dense_list);
    
    print_result_korean("케이스 4: 밀집 그래프 - 인접 리스트", memory_4, insert_comp_4, delete_comp_4, connect_comp_4, neighbor_comp_4);
    free_adj_list(dense_list);

    return 0;
}