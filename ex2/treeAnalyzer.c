#include "treeFunction.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static int index;

static void skip_spaces(const char* str) {
    while (str[index] == ' ') index++;
}

// Create a new tree node
static TreeNode* create_node(const char* data) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node) {
        strcpy(node->data, data);
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

// Parse token - simplified since input is already validated
static void parse_token(const char* str, char* token) {
    skip_spaces(str);
    int token_len = 0;
    while (isalpha(str[index])) {
        token[token_len++] = str[index++];
    }
    token[token_len] = '\0';
}

// Forward declaration
static TreeNode* build_node(const char* str);

// Build child node - simplified since we know input is valid
static TreeNode* build_child(const char* str) {
    skip_spaces(str);
    
    if (str[index] == '(') {
        // Child is a full node
        return build_node(str);
    } else {
        // Child is a simple token
        char token[100];
        parse_token(str, token);
        TreeNode* node = create_node(token);
        
        skip_spaces(str);
        
        // Check if this token has children
        if (str[index] == '(') {
            index++; // skip '('
            
            // Parse first child
            skip_spaces(str);
            if (str[index] != ')') {
                node->left = build_child(str);
                
                // Parse second child if exists
                skip_spaces(str);
                if (str[index] != ')') {
                    node->right = build_child(str);
                }
            }
            
            skip_spaces(str);
            index++; // skip ')'
        }
        
        return node;
    }
}

static TreeNode* build_node(const char* str) {
    skip_spaces(str);
    index++; // skip '('
    
    // Parse node name
    char node_name[100];
    parse_token(str, node_name);
    TreeNode* node = create_node(node_name);
    
    skip_spaces(str);
    
    // Check if there are children
    if (str[index] == '(') {
        index++; // skip '('
        
        // Parse first child
        skip_spaces(str);
        if (str[index] != ')') {
            node->left = build_child(str);
            
            // Parse second child if exists
            skip_spaces(str);
            if (str[index] != ')') {
                node->right = build_child(str);
            }
        }
        
        skip_spaces(str);
        index++; // skip ')'
    }
    
    skip_spaces(str);
    index++; // skip ')'
    
    return node;
}

TreeNode* parse_and_build_tree(const char* tree_string) {
    index = 0;
    return build_node(tree_string);
}

int calculate_height(TreeNode* root) {
    if (root == NULL) return -1;
    if (root->left == NULL && root->right == NULL) return 0;
    
    int left_height = calculate_height(root->left);
    int right_height = calculate_height(root->right);
    
    return 1 + (left_height > right_height ? left_height : right_height);
}

int count_nodes(TreeNode* root) {
    if (root == NULL) return 0;
    return 1 + count_nodes(root->left) + count_nodes(root->right);
}

int count_terminal_nodes(TreeNode* root) {
    if (root == NULL) return 0;
    if (root->left == NULL && root->right == NULL) return 1;
    return count_terminal_nodes(root->left) + count_terminal_nodes(root->right);
}

void free_tree(TreeNode* root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}