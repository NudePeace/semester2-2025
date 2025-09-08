#include "treeValidator.h"
#include <ctype.h>
#include <stdbool.h>

static int index;

static void skip_spaces(const char* str) {
    while (str[index] == ' ') index++;
}

static bool parse_node(const char* str);

static bool parse_token(const char* str) {
    skip_spaces(str);
    if (!isalpha(str[index])) return false;
    
    while (isalpha(str[index])) index++;
    return true;
}

static bool parse_child(const char* str) {
    skip_spaces(str);
    
    if (str[index] == '(') {
        return parse_node(str);
    } else {
        if (!parse_token(str)) return false;
        
        skip_spaces(str);
        
        // Check if this token has children
        if (str[index] == '(') {
            index++; 
            
            int child_count = 0;
            while (true) {
                skip_spaces(str);
                
                if (str[index] == ')') {
                    index++; // skip ')'
                    break;
                }
                
                if (!parse_child(str)) return false;
                
                child_count++;
                if (child_count > 2) return false; // Binary tree: max 2 children
            }
            
            if (child_count == 0) return false; // Empty children list not allowed
        }
        
        return true;
    }
}

static bool parse_node(const char* str) {
    skip_spaces(str);
    if (str[index] != '(') return false;
    index++; // skip '('
    
    // Parse the node name
    if (!parse_token(str)) return false;
    
    skip_spaces(str);
    
    // Check if there are children
    if (str[index] == '(') {
        index++; 
        int child_count = 0;
        
        while (true) {
            skip_spaces(str);
            
            // End of children list
            if (str[index] == ')') {
                index++; // skip ')'
                break;
            }
            
            if (!parse_child(str)) return false;
            
            child_count++;
            if (child_count > 2) return false; // Binary tree: max 2 children
        }
        
        if (child_count == 0) return false; // Empty children list not allowed
    }
    
    skip_spaces(str);
    if (str[index] != ')') return false;
    index++; // skip ')'
    
    return true;
}

bool is_valid_binary_tree(const char* tree_string) {
    index = 0;
    skip_spaces(tree_string);
    
    if (!parse_node(tree_string)) return false;
    
    skip_spaces(tree_string);
    return tree_string[index] == '\0'; 
}