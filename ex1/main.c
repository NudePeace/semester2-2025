#include <stdio.h>
#include <string.h>
#include "treeValidator.h"

int main() {
    char input[1000];

    printf("Enter tree strings (type 'exit' to stop):\n");

    while (1) {
        printf(">> ");
        if (fgets(input, sizeof(input), stdin)) {
            input[strcspn(input, "\n")] = 0;

            if (strcmp(input, "exit") == 0) break;

            bool result = is_valid_binary_tree(input);
            printf("Result: %s\n\n", result ? "TRUE" : "FALSE");
        }
    }

    return 0;
}
