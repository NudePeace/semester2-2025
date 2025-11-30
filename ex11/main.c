#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 200

typedef struct
{
    int id;
    char name[MAX_NAME_LEN];
    char gender;
    int korean;
    int english;
    int math;
    int total_score;
} Student;

typedef struct
{
    long long comparisons;
} PerformanceMetrics;

typedef struct AVLNode
{
    Student data;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

// Utility functions
void swap_students(Student *a, Student *b);
void shuffle_students(Student *arr, int n);
int compare_id_asc(const Student *a, const Student *b);
Student *load_data(const char *filename, int *out_count);
Student *copy_data(const Student *source, int n);
void shell_sort(Student *arr, int n, int (*compare_func)(const Student *, const Student *));

// AVL functions
int get_height(AVLNode *node);
int get_balance_factor(AVLNode *node);
int max(int a, int b);
AVLNode *new_node(Student s);
AVLNode *right_rotate(AVLNode *y);
AVLNode *left_rotate(AVLNode *x);
AVLNode *insert_avl(AVLNode *node, Student s, PerformanceMetrics *metrics);
Student *avl_search(AVLNode *root, int target_id, PerformanceMetrics *metrics);
AVLNode *avl_delete(AVLNode *node, int target_id, PerformanceMetrics *metrics, int *found); 
void free_avl_tree(AVLNode *root);

// Array functions
Student *sequential_search(const Student *arr, int n, int target_id, PerformanceMetrics *metrics);
int unsorted_array_insert(Student **arr_ptr, int *n_ptr, int *capacity_ptr, Student s, PerformanceMetrics *metrics);
int unsorted_array_delete(Student *arr, int *n_ptr, int target_id, PerformanceMetrics *metrics);
Student *binary_search(const Student *arr, int n, int target_id, PerformanceMetrics *metrics);
int sorted_array_insert(Student **arr_ptr, int *n_ptr, int *capacity_ptr, Student s, PerformanceMetrics *metrics);
int sorted_array_delete(Student *arr, int *n_ptr, int target_id, PerformanceMetrics *metrics);

Student *load_data(const char *filename, int *out_count)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("Failed to open file"); return NULL; }
    char line[MAX_LINE_LEN];
    int capacity = 10;
    int count = 0;
    Student *arr = (Student *)malloc(sizeof(Student) * capacity);
    if (!arr) { perror("Memory allocation failed"); fclose(fp); return NULL; }
    if (fgets(line, sizeof(line), fp) == NULL) { free(arr); fclose(fp); *out_count = 0; return NULL; }
    while (fgets(line, sizeof(line), fp))
    {
        if (count >= capacity)
        {
            capacity *= 2;
            Student *temp = (Student *)realloc(arr, sizeof(Student) * capacity);
            if (!temp) { perror("Reallocation failed. Returning partially read data."); fclose(fp); *out_count = count; return arr; }
            arr = temp;
        }
        Student s = {0};
        char *token;
        token = strtok(line, "\t"); if (token) s.id = atoi(token);
        token = strtok(NULL, "\t"); if (token) strncpy(s.name, token, MAX_NAME_LEN - 1); s.name[MAX_NAME_LEN - 1] = '\0';
        token = strtok(NULL, "\t"); if (token && token[0] != '\n') s.gender = token[0];
        token = strtok(NULL, "\t"); if (token) s.korean = atoi(token);
        token = strtok(NULL, "\t"); if (token) s.english = atoi(token);
        token = strtok(NULL, "\t\n"); if (token) s.math = atoi(token);
        s.total_score = s.korean + s.english + s.math;
        arr[count++] = s;
    }
    fclose(fp);
    Student *tight = (Student *)realloc(arr, sizeof(Student) * count);
    if (tight) { arr = tight; }
    *out_count = count;
    return arr;
}

Student *copy_data(const Student *source, int n)
{
    if (!source || n <= 0) return NULL;
    Student *copy = (Student *)malloc(sizeof(Student) * n);
    if (copy) { memcpy(copy, source, sizeof(Student) * n); }
    return copy;
}

void swap_students(Student *a, Student *b)
{
    Student temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle_students(Student *arr, int n)
{
    if (n > 1)
    {
        srand((unsigned int)time(NULL));
        for (int i = n - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            swap_students(&arr[i], &arr[j]);
        }
    }
}

int compare_id_asc(const Student *a, const Student *b)
{
    return a->id - b->id;
}

void shell_sort(Student *arr, int n, int (*compare_func)(const Student *, const Student *))
{
    int gap = n / 2;
    while (gap > 0)
    {
        for (int i = gap; i < n; i++)
        {
            Student key = arr[i];
            int j = i;
            while (j >= gap && compare_func(&arr[j - gap], &key) > 0)
            {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = key;
        }
        gap /= 2;
    }
}


// ============ AVL Tree Functions ============

int get_height(AVLNode *node)
{
    return (node == NULL) ? 0 : node->height;
}

int get_balance_factor(AVLNode *node)
{
    return (node == NULL) ? 0 : get_height(node->left) - get_height(node->right);
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

AVLNode *new_node(Student s)
{
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    if (!node)
    {
        perror("Memory allocation failed for AVL node");
        return NULL;
    }
    node->data = s;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode *right_rotate(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;

    return x;
}

AVLNode *left_rotate(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;

    return y;
}

AVLNode *insert_avl(AVLNode *node, Student s, PerformanceMetrics *metrics)
{
    if (node == NULL)
    {
        return new_node(s);
    }

    if (metrics) metrics->comparisons++;
    int cmp = s.id - node->data.id;

    if (cmp < 0)
    {
        node->left = insert_avl(node->left, s, metrics);
    }
    else if (cmp > 0)
    {
        node->right = insert_avl(node->right, s, metrics);
    }
    else
    {
        return node;
    }

    node->height = 1 + max(get_height(node->left), get_height(node->right));

    int balance = get_balance_factor(node);

    if (balance > 1)
    {
        int left_balance = get_balance_factor(node->left);
        if (left_balance >= 0)
        {
            return right_rotate(node);
        }
        else
        {
            node->left = left_rotate(node->left);
            return right_rotate(node);
        }
    }

    if (balance < -1)
    {
        int right_balance = get_balance_factor(node->right);
        if (right_balance <= 0)
        {
            return left_rotate(node);
        }
        else
        {
            node->right = right_rotate(node->right);
            return left_rotate(node);
        }
    }

    return node;
}

Student *avl_search(AVLNode *root, int target_id, PerformanceMetrics *metrics)
{
    if (metrics) metrics->comparisons = 0;
    AVLNode *current = root;

    while (current != NULL)
    {
        if (metrics) metrics->comparisons++;
        if (current->data.id == target_id)
        {
            return &current->data;
        }
        else if (target_id < current->data.id)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return NULL;
}

AVLNode *avl_delete(AVLNode *node, int target_id, PerformanceMetrics *metrics, int *found)
{
    if (node == NULL)
    {
        return NULL;
    }

    if (metrics) metrics->comparisons++;
    int cmp = target_id - node->data.id;

    if (cmp < 0)
    {
        node->left = avl_delete(node->left, target_id, metrics, found);
    }
    else if (cmp > 0)
    {
        node->right = avl_delete(node->right, target_id, metrics, found);
    }
    else
    {
        *found = 1;
        if (node->left == NULL)
        {
            AVLNode *temp = node->right;
            free(node);
            return temp;
        }
        else if (node->right == NULL)
        {
            AVLNode *temp = node->left;
            free(node);
            return temp;
        }

        AVLNode *current = node->right;
        while (current->left != NULL)
        {
            current = current->left;
        }

        node->data = current->data;
        node->right = avl_delete(node->right, current->data.id, metrics, found);
        *found = 1; 
    }

    if (node == NULL)
        return NULL;

    node->height = 1 + max(get_height(node->left), get_height(node->right));

    int balance = get_balance_factor(node);

    if (balance > 1)
    {
        if (get_balance_factor(node->left) < 0)
        {
            node->left = left_rotate(node->left);
        }
        return right_rotate(node);
    }

    if (balance < -1)
    {
        if (get_balance_factor(node->right) > 0)
        {
            node->right = right_rotate(node->right);
        }
        return left_rotate(node);
    }

    return node;
}

void free_avl_tree(AVLNode *root)
{
    if (root != NULL)
    {
        free_avl_tree(root->left);
        free_avl_tree(root->right);
        free(root);
    }
}

// ============Unsorted Array Operations ============

Student *sequential_search(const Student *arr, int n, int target_id, PerformanceMetrics *metrics)
{
    metrics->comparisons = 0;
    for (int i = 0; i < n; i++)
    {
        metrics->comparisons++;
        if (arr[i].id == target_id)
        {
            return (Student *)&arr[i];
        }
    }
    return NULL;
}

int unsorted_array_insert(Student **arr_ptr, int *n_ptr, int *capacity_ptr, Student s, PerformanceMetrics *metrics)
{
    metrics->comparisons = 0;
    Student *arr = *arr_ptr;

    if (*n_ptr >= *capacity_ptr)
    {
        int new_capacity = (*capacity_ptr == 0) ? 1 : *capacity_ptr * 2;
        Student *temp = (Student *)realloc(*arr_ptr, sizeof(Student) * new_capacity);
        if (!temp) return 0;
        *arr_ptr = temp;
        *capacity_ptr = new_capacity;
        arr = *arr_ptr;
    }

    arr[*n_ptr] = s;
    (*n_ptr)++;
    return 1;
}

int unsorted_array_delete(Student *arr, int *n_ptr, int target_id, PerformanceMetrics *metrics)
{
    metrics->comparisons = 0;
    int i;
    int n = *n_ptr;

    for (i = 0; i < n; i++)
    {
        metrics->comparisons++;
        if (arr[i].id == target_id)
            break;
    }

    if (i == n)
        return 0;

    for (int j = i; j < n - 1; j++)
    {
        arr[j] = arr[j + 1];
    }
    (*n_ptr)--;
    return 1;
}


//Sorted Array Operation
Student *binary_search(const Student *arr, int n, int target_id, PerformanceMetrics *metrics)
{
    metrics->comparisons = 0;
    int low = 0;
    int high = n - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        metrics->comparisons++;

        if (arr[mid].id == target_id)
            return (Student *)&arr[mid];

        if (target_id < arr[mid].id)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return NULL;
}

int sorted_array_insert(Student **arr_ptr, int *n_ptr, int *capacity_ptr, Student s, PerformanceMetrics *metrics)
{
    metrics->comparisons = 0;
    Student *arr = *arr_ptr;

    if (*n_ptr >= *capacity_ptr)
    {
        int new_capacity = (*capacity_ptr == 0) ? 1 : *capacity_ptr * 2;
        Student *temp = (Student *)realloc(*arr_ptr, sizeof(Student) * new_capacity);
        if (!temp)
            return 0;
        *arr_ptr = temp;
        *capacity_ptr = new_capacity;
        arr = *arr_ptr;
    }

    int i = *n_ptr - 1;
    while (i >= 0)
    {
        metrics->comparisons++;
        if (s.id < arr[i].id)
        {
            arr[i + 1] = arr[i];
            i--;
        }
        else
        {
            break;
        }
    }
    arr[i + 1] = s;
    (*n_ptr)++;
    return 1;
}

int sorted_array_delete(Student *arr, int *n_ptr, int target_id, PerformanceMetrics *metrics)
{
    metrics->comparisons = 0;
    int low = 0;
    int high = *n_ptr - 1;
    int index_to_delete = -1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        metrics->comparisons++;
        if (arr[mid].id == target_id)
        {
            index_to_delete = mid;
            break;
        }

        if (target_id < arr[mid].id)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }

    if (index_to_delete == -1)
        return 0;

    for (int j = index_to_delete; j < *n_ptr - 1; j++)
    {
        arr[j] = arr[j + 1];
    }
    (*n_ptr)--;
    return 1;
}

int main()
{   
    int student_count = 0;
    const char *filename = "C:\\Users\\lastg\\Downloads\\dataset_id_ascending.csv";

    Student *all_students = load_data(filename, &student_count);
    if (!all_students || student_count == 0)
    {
        printf("Failed to load data\n");
        return 1;
    }

    srand(time(NULL) + rand());
    int target_index = rand() % student_count;
    Student target_student = all_students[target_index];
    int target_id = target_student.id;

    Student new_student = {
        .id = 100001 + rand() % (999999 - 100001) + 1, 
        .korean = 85, .english = 90, .math = 88, .total_score = 263,
        .gender = 'M'
    };
    strncpy(new_student.name, "New Test Student", MAX_NAME_LEN - 1);
    
    // Unsorted Array
    Student *unsorted_arr = copy_data(all_students, student_count);
    int unsorted_count = student_count;
    int unsorted_capacity = student_count;
    shuffle_students(unsorted_arr, unsorted_count);

    // Sorted Array
    Student *sorted_arr = copy_data(all_students, student_count);
    int sorted_count = student_count;
    int sorted_capacity = student_count;
    shell_sort(sorted_arr, sorted_count, compare_id_asc); 

    // AVL Tree
    AVLNode *avl_root = NULL;
    for (int i = 0; i < student_count; i++)
    {
        PerformanceMetrics temp_metrics = {0}; 
        avl_root = insert_avl(avl_root, all_students[i], &temp_metrics);
    }
    
    PerformanceMetrics metrics = {0};
    PerformanceMetrics insert_metrics = {0};
    PerformanceMetrics delete_metrics = {0};
    Student *result = NULL;
    int del_result = 0;
    int found = 0;


    //검색
    printf("\n검색 ID %d ---\n", target_id);
    
    // Unsorted Array (Sequential Search)
    result = sequential_search(unsorted_arr, unsorted_count, target_id, &metrics);
    printf("비정렬된 벼열 검색(Sequential Search): %s | 비교 횟수: %lld\n", result ? "Found" : "Not Found", metrics.comparisons);
    
    // Sorted Array (Binary Search)
    result = binary_search(sorted_arr, sorted_count, target_id, &metrics);
    printf("정렬된 배령 (이진 탐색):               %s | 비교 횟수: %lld\n", result ? "Found" : "Not Found", metrics.comparisons);
    
    // AVL Tree (Search)
    result = avl_search(avl_root, target_id, &metrics); 
    printf("AVL Tree:                           %s | 비교 횟수: %lld\n", result ? "Found" : "Not Found", metrics.comparisons);

    //삽입
    printf("\n삽입 ID %d ---\n", new_student.id);

    // Unsorted Array
    del_result = unsorted_array_insert(&unsorted_arr, &unsorted_count, &unsorted_capacity, new_student, &insert_metrics);
    printf("비정렬된 배열: %s | 비교 횟수: %lld\n", del_result ? "삽입했습니다" : "실폐", insert_metrics.comparisons, unsorted_count);

    // Sorted Array
    insert_metrics.comparisons = 0;
    del_result = sorted_array_insert(&sorted_arr, &sorted_count, &sorted_capacity, new_student, &insert_metrics);
    printf("정렬된 배열:  %s | 비교 횟수: %lld\n", del_result ? "삽입했습니다" : "실폐", insert_metrics.comparisons, sorted_count);

    // AVL Tree
    insert_metrics.comparisons = 0;
    avl_root = insert_avl(avl_root, new_student, &insert_metrics);
    printf("AVL Tree:   %s | 비교 횟수: %lld\n", avl_root ? "삽입했습니다" : "실폐", insert_metrics.comparisons);

    //삭제 
    printf("\n삭제 ID %d ---\n", target_id);

    // Unsorted Array
    delete_metrics.comparisons = 0;
    del_result = unsorted_array_delete(unsorted_arr, &unsorted_count, target_id, &delete_metrics);
    printf("비정렬된 배열: %s | 비교 횟수: %lld\n", del_result ? "삭제했습니다" : "Not Found", delete_metrics.comparisons, unsorted_count);

    // Sorted Array
    delete_metrics.comparisons = 0;
    del_result = sorted_array_delete(sorted_arr, &sorted_count, target_id, &delete_metrics);
    printf("정렬된 배열:   %s | 비교 횟수: %lld\n", del_result ? "삭제했습니다" : "Not Found", delete_metrics.comparisons, sorted_count);

    // AVL Tree
    delete_metrics.comparisons = 0;
    found = 0;
    avl_root = avl_delete(avl_root, target_id, &delete_metrics, &found);
    printf("AVL Tree:    %s | 비교 횟수: %lld\n", found ? "삭제했습니다" : "Not Found", delete_metrics.comparisons);

    printf("\n========================================================\n");

    // ============ Cleanup ============
    free(all_students);
    free(unsorted_arr);
    free(sorted_arr);
    free_avl_tree(avl_root);

    return 0;
}