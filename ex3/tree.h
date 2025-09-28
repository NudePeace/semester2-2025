#ifndef TREE_H
#define TREE_H

#define MAX_NODES 128
#define STACK_SIZE 512
#define MAX_INPUT_SIZE 512

typedef struct
{
    char data;
    int left;
    int right;
} TreeNode;

typedef struct
{
    int items[STACK_SIZE];
    int top;
} Stack;

extern TreeNode tree[MAX_NODES];
extern int node_count;

// --- Utility Functions ---
void initialize(Stack *s);
int isEmpty(Stack *s);
void push(Stack *s, int value);
int pop(Stack *s);
int isValidNode(int index);

// --- Parsing Functions ---
int parseTree(const char *input_string);
int parseNode(const char *str, int *i, int parent_index, int side);
void parseChildren(const char *str, int *i, int current_node_index);
void skipWhitespace(const char *str, int *i);
int getNextIndex();

// --- Traversal Functions ---
void preorder_iterative(int root_index);
void inorder_iterative(int root_index);
void postorder_iterative(int root_index);

void printTreeArray(int root_index);

#endif