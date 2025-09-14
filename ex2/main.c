#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "treeFunction.h"

bool has_input_errors(const char *input)
{
    int len = strlen(input);

    if (len == 0)
        return true;

    for (int i = 0; i < len; i++)
    {
        if (input[i] == '\n' || input[i] == '\r')
            return true;
    }

    if (input[0] != '(' || input[len - 1] != ')')
        return true;

    int open_count = 0, close_count = 0;
    for (int i = 0; i < len; i++)
    {
        if (input[i] == '(')
            open_count++;
        else if (input[i] == ')')
            close_count++;
    }

    if (open_count != close_count)
        return true;

    for (int i = 0; i < len; i++)
    {
        if (!isalpha(input[i]) && input[i] != '(' && input[i] != ')' && input[i] != ' ')
        {
            return true;
        }
    }

    return false;
}

int main()
{
    char input[1000];

    printf("Enter tree strings (type 'exit' to stop):\n");

    while (1)
    {
        printf(">> ");
        if (fgets(input, sizeof(input), stdin))
        {
            input[strcspn(input, "\n")] = 0;

            if (strcmp(input, "quit") == 0)
                break;

            // Check for basic input errors first
            if (has_input_errors(input))
            {
                printf("ERROR\n\n");
                continue;
            }

            bool result = is_valid_binary_tree(input);
            printf("Result: %s\n\n", result ? "TRUE" : "FALSE");
            if (result)
            {
                TreeNode *root = parse_and_build_tree(input);
                int height = calculate_height(root);
                int total_nodes = count_nodes(root);
                int terminal_nodes = count_terminal_nodes(root);
                printf("%d, %d, %d\n\n", height, total_nodes, terminal_nodes);

                free_tree(root);
            }
        }
    }

    return 0;
}
