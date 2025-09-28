#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

TreeNode tree[MAX_NODES];
int node_count = 0;

void initialize(Stack *s) { s->top = -1; }
int isEmpty(Stack *s) { return s->top == -1; }

void push(Stack *s, int value)
{
    if (s->top == STACK_SIZE - 1)
    {
        fprintf(stderr, "Error: Stack overflow (Stack size %d)\n", STACK_SIZE);
        exit(EXIT_FAILURE);
    }
    s->items[++(s->top)] = value;
}

int pop(Stack *s)
{
    if (isEmpty(s))
        return -1;
    return s->items[(s->top)--];
}

int isValidNode(int index)
{
    return index >= 1 && index < MAX_NODES && tree[index].data != '\0';
}

void skipWhitespace(const char *str, int *i)
{
    while (str[*i] != '\0' && isspace(str[*i]))
    {
        (*i)++;
    }
}

int getNextIndex(int parent_index, int side)
{
    return (side == 1) ? (2 * parent_index) : (2 * parent_index + 1);
}

int parseTree(const char *input_string)
{
    int i = 0;
    skipWhitespace(input_string, &i);
    if (input_string[i] != '(')
    {
        fprintf(stderr, "Error: Tree must start with '('.\n");
        return -1;
    }
    i++;
    skipWhitespace(input_string, &i);

    if (isalpha(input_string[i]))
    {
        int root_index = 1;
        tree[root_index].data = input_string[i];

        i++;
        skipWhitespace(input_string, &i);

        if (input_string[i] == '(')
        {
            i++;
            parseChildren(input_string, &i, root_index);
        }

        skipWhitespace(input_string, &i);
        if (input_string[i] != ')')
        {
            fprintf(stderr, "Error: Missing final closing parenthesis ')' at index %d.\n", i);
        }

        if (node_count == 0)
            node_count = 1;

        return root_index;
    }
    fprintf(stderr, "Error: Root node data expected.\n");
    return -1;
}

int parseNode(const char *str, int *i, int parent_index, int side)
{
    skipWhitespace(str, i);
    if (str[*i] == '\0' || str[*i] == ')')
        return 0;

    int current_node_index = getNextIndex(parent_index, side);
    if (current_node_index >= MAX_NODES)
    {
        fprintf(stderr, "Error: Max nodes exceeded.\n");
        exit(EXIT_FAILURE);
    }

    tree[current_node_index].data = str[*i];

    if (current_node_index > node_count)
        node_count = current_node_index;

    (*i)++;
    skipWhitespace(str, i);

    if (str[*i] == '(')
    {
        (*i)++;
        parseChildren(str, i, current_node_index);
        skipWhitespace(str, i);
        if (str[*i] != ')')
        {
            fprintf(stderr, "Error: Missing closing parenthesis ')' at index %d.\n", *i);
        }
        (*i)++;
    }
    return current_node_index;
}

void parseChildren(const char *str, int *i, int current_node_index)
{
    skipWhitespace(str, i);
    if (str[*i] == ')' || str[*i] == '\0')
        return;

    tree[current_node_index].left = parseNode(str, i, current_node_index, 1);

    skipWhitespace(str, i);
    if (str[*i] == ')' || str[*i] == '\0')
        return;

    tree[current_node_index].right = parseNode(str, i, current_node_index, 2);
}

void preorder_iterative(int root_index)
{
    if (!isValidNode(root_index))
        return;
    Stack s;
    initialize(&s);
    push(&s, root_index);
    printf("pre-order: ");
    while (!isEmpty(&s))
    {
        int current_index = pop(&s);
        printf("%c ", tree[current_index].data);

        if (tree[current_index].right != 0 && isValidNode(tree[current_index].right))
        {
            push(&s, tree[current_index].right);
        }

        if (tree[current_index].left != 0 && isValidNode(tree[current_index].left))
        {
            push(&s, tree[current_index].left);
        }
    }
    printf("\n");
}

void inorder_iterative(int root_index)
{
    Stack s;
    initialize(&s);
    int current_index = root_index;

    printf("in-order: ");

    while (!isEmpty(&s) || current_index != -1)
    {

        while (current_index != -1 && isValidNode(current_index))
        {
            push(&s, current_index);

            if (tree[current_index].left != 0)
            {
                current_index = tree[current_index].left;
            }
            else
            {
                current_index = -1;
            }
        }

        if (!isEmpty(&s))
        {
            current_index = pop(&s);

            if (current_index == -1)
                break;

            printf("%c ", tree[current_index].data);

            if (tree[current_index].right != 0)
            {
                current_index = tree[current_index].right;
            }
            else
            {
                current_index = -1;
            }
        }
        else
        {
            break;
        }
    }
    printf("\n");
}

void postorder_iterative(int root_index)
{
    if (!isValidNode(root_index))
        return;
    Stack s1, s2;
    initialize(&s1);
    initialize(&s2);
    push(&s1, root_index);
    while (!isEmpty(&s1))
    {
        int current_index = pop(&s1);
        push(&s2, current_index);

        if (tree[current_index].left != 0 && isValidNode(tree[current_index].left))
        {
            push(&s1, tree[current_index].left);
        }
        if (tree[current_index].right != 0 && isValidNode(tree[current_index].right))
        {
            push(&s1, tree[current_index].right);
        }
    }
    printf("post-order: ");
    while (!isEmpty(&s2))
    {
        printf("%c ", tree[pop(&s2)].data);
    }
    printf("\n");
}

void printTreeArray(int root_index)
{
    printf("\n--- Tree Array Structure (Root Index = %d) ---\n", root_index);
    printf("| Index (i) | Data | Left Child (2i) | Right Child (2i+1) |\n");
    printf("|:---------:|:----:|:-----------------:|:-------------------:|\n");

    for (int i = 0; i < MAX_NODES; i++)
    {
        if (tree[i].data != '\0')
        {
            printf("| %9d | %4c | %17d | %19d |\n",
                   i, tree[i].data, tree[i].left, tree[i].right);
        }
    }
}