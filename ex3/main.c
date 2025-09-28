#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char input_string[MAX_INPUT_SIZE];
    int root_index = 1;

    if (fgets(input_string, MAX_INPUT_SIZE, stdin) == NULL)
    {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }

    input_string[strcspn(input_string, "\n")] = 0;

    memset(tree, 0, sizeof(tree));
    parseTree(input_string);

    if (!isValidNode(root_index))
    {
        fprintf(stderr, "Error: Failed to parse the tree input or root is invalid.\n");
        return 1;
    }

    printf("Input: %s\n", input_string);

    printTreeArray(root_index);

    printf("\nFinal Output (Iterative Traversal):\n");
    preorder_iterative(root_index);
    inorder_iterative(root_index);
    postorder_iterative(root_index);

    return 0;
}