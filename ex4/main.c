#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
double get_time() {
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / freq.QuadPart;
}
#else
#include <sys/time.h>
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}
#endif

// Binary Search Tree (BST) Structure 
typedef struct Node {
    int data;
    struct Node *left, *right;
} Node;

// Create a new node
Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Insert a node into the BST
Node* insert(Node* root, int value) {
    if (root == NULL)
        return createNode(value);
    if (value < root->data)
        root->left = insert(root->left, value);
    else if (value > root->data)
        root->right = insert(root->right, value);
    return root;
}

// Iterative BST search
int searchBST_iterative(Node* root, int target, int* count) {
    Node* current = root;
    while (current != NULL) {
        (*count)++;
        if (target == current->data)
            return 1;
        else if (target < current->data)
            current = current->left;
        else
            current = current->right;
    }
    return 0;
}

// Linear search in an array
int linearSearch(int arr[], int n, int target, int* count) {
    for (int i = 0; i < n; i++) {
        (*count)++;
        if (arr[i] == target)
            return 1;
    }
    return 0;
}

int main() {
    int arr[100];
    Node* root = NULL;
    srand((unsigned)time(NULL));

    for (int i = 0; i < 100; i++) {
        arr[i] = rand() % 1001;  
        root = insert(root, arr[i]);
    }

    int target = arr[rand() % 100];
    printf("Target number: %d\n", target);

    //Linear search in array
    int linearCount = 0;
    double start1 = get_time();
    int foundLinear = linearSearch(arr, 100, target, &linearCount);
    double end1 = get_time();

    //BST search
    int bstCount = 0;
    double start2 = get_time();
    int foundBST = searchBST_iterative(root, target, &bstCount);
    double end2 = get_time();

    //Print comparison results
    printf("\n--- Search Performance Comparison ---\n");

    printf("Linear Search: %s\n", foundLinear ? "Found" : "Not Found");
    printf("  Comparisons: %d\n", linearCount);
    printf("  Time taken: %.9f seconds (%.3f μs)\n", end1 - start1, (end1 - start1) * 1e6);

    printf("\nBST Search (Iterative): %s\n", foundBST ? "Found" : "Not Found");
    printf("  Comparisons: %d\n", bstCount);
    printf("  Time taken: %.9f seconds (%.3f μs)\n", end2 - start2, (end2 - start2) * 1e6);

    return 0;
}
