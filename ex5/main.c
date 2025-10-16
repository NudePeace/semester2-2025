#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ============================================
// ARRAY FUNCTIONS
// ============================================

int linearSearch(int arr[], int n, int target, int* count) {
    for (int i = 0; i < n; i++) {
        (*count)++;  
        if (arr[i] == target)
            return 1; 
    }
    return 0;  
}

// ============================================
// BST (Binary Search Tree) FUNCTIONS
// ============================================

typedef struct Node {
    int data;
    struct Node *left, *right;
} Node;

Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node* insertBST(Node* root, int value) {
    if (root == NULL)
        return createNode(value);
    if (value < root->data)
        root->left = insertBST(root->left, value);
    else if (value > root->data)
        root->right = insertBST(root->right, value);
    return root;
}

int searchBST(Node* root, int target, int* count) {
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

void freeBST(Node* root) {
    if (root == NULL) return;
    freeBST(root->left);
    freeBST(root->right);
    free(root);
}

// ============================================
// AVL TREE FUNCTIONS
// ============================================

typedef struct AVLNode {
    int data;
    struct AVLNode *left, *right;
    int height;
} AVLNode;

AVLNode* createAVLNode(int value) {
    AVLNode* newNode = (AVLNode*)malloc(sizeof(AVLNode));
    newNode->data = value;
    newNode->left = newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

int height(AVLNode* node) {
    return node == NULL ? 0 : node->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int getBalance(AVLNode* node) {
    return node == NULL ? 0 : height(node->left) - height(node->right);
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

AVLNode* insertAVL(AVLNode* node, int value) {
    if (node == NULL)
        return createAVLNode(value);
    
    if (value < node->data)
        node->left = insertAVL(node->left, value);
    else if (value > node->data)
        node->right = insertAVL(node->right, value);
    else
        return node;
    
    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);
    
    // Left Left Case
    if (balance > 1 && value < node->left->data)
        return rightRotate(node);
    
    // Right Right Case
    if (balance < -1 && value > node->right->data)
        return leftRotate(node);
    
    // Left Right Case
    if (balance > 1 && value > node->left->data) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    
    // Right Left Case
    if (balance < -1 && value < node->right->data) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    
    return node;
}

int searchAVL(AVLNode* root, int target, int* count) {
    AVLNode* current = root;
    while (current != NULL) {
        (*count)++;  // Count every comparison
        if (target == current->data)
            return 1;  // Found
        else if (target < current->data)
            current = current->left;
        else
            current = current->right;
    }
    return 0;  
}

// Free AVL
void freeAVL(AVLNode* root) {
    if (root == NULL) return;
    freeAVL(root->left);
    freeAVL(root->right);
    free(root);
}

// ============================================
// UTILITY FUNCTIONS (for all data structures)
// ============================================

void shuffle(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void generateDataset(int data[], int type) {
    switch(type) {
        case 1: // Random unique integers 0-10000
            for (int i = 0; i < 10001; i++) {
                data[i] = i;
            }
            shuffle(data, 10001);
            break;
        case 2: // Sorted 0-999
            for (int i = 0; i < 1000; i++) {
                data[i] = i;
            }
            break;
        case 3: // Sorted 999-0
            for (int i = 0; i < 1000; i++) {
                data[i] = 999 - i;
            }
            break;
        case 4: // i * (i % 2 + 2)
            for (int i = 0; i < 1000; i++) {
                data[i] = i * (i % 2 + 2);
            }
            break;
    }
}

void runTest(int datasetType, const char* datasetName) {
    int data[10001];
    int searchTargets[1000];
    
    generateDataset(data, datasetType);
    
    int* arr = (int*)malloc(1000 * sizeof(int));
    Node* bstRoot = NULL;
    AVLNode* avlRoot = NULL;
    
    for (int i = 0; i < 1000; i++) {
        arr[i] = data[i];
        bstRoot = insertBST(bstRoot, data[i]);
        avlRoot = insertAVL(avlRoot, data[i]);
    }
    
    for (int i = 0; i < 1000; i++) {
        searchTargets[i] = rand() % 10001;
    }
    
    int totalArrayCount = 0;
    int totalBSTCount = 0;
    int totalAVLCount = 0;
    
    for (int i = 0; i < 1000; i++) {
        int count = 0;
        linearSearch(arr, 1000, searchTargets[i], &count);
        totalArrayCount += count;
        
        count = 0;
        searchBST(bstRoot, searchTargets[i], &count);
        totalBSTCount += count;
        
        count = 0;
        searchAVL(avlRoot, searchTargets[i], &count);
        totalAVLCount += count;
    }
    
    printf("Array: %s에서 평균 %.2f회 탐색\n", 
           datasetName, totalArrayCount / 1000.0);
    printf("BST: %s에서 평균 %.2f회 탐색\n", 
           datasetName, totalBSTCount / 1000.0);
    printf("AVL: %s에서 평균 %.2f회 탐색\n\n", 
           datasetName, totalAVLCount / 1000.0);
    
    free(arr);
    freeBST(bstRoot);
    freeAVL(avlRoot);
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    srand((unsigned)time(NULL));
    
    printf("=== Search Performance Comparison ===\n\n");
    
    runTest(1, "데이터 (1)");
    runTest(2, "데이터 (2)");
    runTest(3, "데이터 (3)");
    runTest(4, "데이터 (4)");
    
    return 0;
}