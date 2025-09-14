#pragma once
#include <stdbool.h>

typedef struct TreeNode {
    char data[100];  // Node name
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

bool is_valid_binary_tree(const char* tree_string);

TreeNode* parse_and_build_tree(const char* tree_string);
int calculate_height(TreeNode* root);
int count_nodes(TreeNode* root);
int count_terminal_nodes(TreeNode* root);
void free_tree(TreeNode* root);