#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 200 
#define NUM_REPETITIONS 1000

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char gender;
    int korean;
    int english;
    int math;
    int total_score; 
} Student;

typedef struct {
    long long comparisons; 
    double memory_usage;
} PerformanceMetrics;

typedef struct TreeNode {
    Student data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

Student* load_students(const char* filename, int* out_count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open file");
        return NULL;
    }

    char line[MAX_LINE_LEN];
    int capacity = 10; 
    int count = 0;
    Student* arr = (Student*)malloc(sizeof(Student) * capacity);

    if (!arr) {
        perror("Memory allocation failed");
        fclose(fp);
        return NULL;
    }

    if (fgets(line, sizeof(line), fp) == NULL) {
        free(arr);
        fclose(fp);
        *out_count = 0;
        return NULL; 
    }

    while (fgets(line, sizeof(line), fp)) {
        if (count >= capacity) {
            capacity *= 2;
            Student* temp = (Student*)realloc(arr, sizeof(Student) * capacity);
            
            if (!temp) {
                perror("Reallocation failed. Returning partially read data.");
                fclose(fp);
                *out_count = count;
                return arr; 
            }
            arr = temp;
        }

        Student s = {0};
        char* token;
        
        // ID
        token = strtok(line, ",");
        if (token) s.id = atoi(token);

        // NAME
        token = strtok(NULL, ",");
        if (token) strncpy(s.name, token, MAX_NAME_LEN - 1);
        s.name[MAX_NAME_LEN - 1] = '\0'; 

        // GENDER
        token = strtok(NULL, ",");
        if (token && token[0] != '\n') s.gender = token[0];

        // KOREAN
        token = strtok(NULL, ",");
        if (token) s.korean = atoi(token);

        // ENGLISH
        token = strtok(NULL, ",");
        if (token) s.english = atoi(token);

        token = strtok(NULL, ",\n"); 
        if (token) s.math = atoi(token);

        s.total_score = s.korean + s.english + s.math;

        arr[count++] = s;
    }

    fclose(fp);

    Student* tight = (Student*)realloc(arr, sizeof(Student) * count);
    if (tight) {
        arr = tight;
    } else {
        fprintf(stderr, "Warning: Tight reallocation failed, using original memory.\n");
    }

    *out_count = count;
    return arr;
}

int compare_id_asc(const Student *a, const Student *b) {
    return a->id - b->id;
}

int compare_id_desc(const Student *a, const Student *b) {
    return b->id - a->id;
}

int compare_name_asc(const Student *a, const Student *b) {
    return strcmp(a->name, b->name);
}

int compare_name_desc(const Student *a, const Student *b) {
    return strcmp(b->name, a->name);
}

int compare_gender_asc(const Student *a, const Student *b) {
    return a->gender - b->gender;
}

int compare_gender_desc(const Student *a, const Student *b) {
    return b->gender - a->gender;
}

int compare_total_score_asc(const Student *a, const Student *b) {
    if (a->total_score != b->total_score) {
        return a->total_score - b->total_score;
    }

    if (a->korean != b->korean) {
        return b->korean - a->korean; 
    }

    if (a->english != b->english) {
        return b->english - a->english;
    }

    return b->math - a->math; 
}

int compare_total_score_desc(const Student *a, const Student *b) {
    if (a->total_score != b->total_score) {
        return b->total_score - a->total_score;
    }
    
    if (a->korean != b->korean) {
        return b->korean - a->korean; 
    }

    if (a->english != b->english) {
        return b->english - a->english;
    }

    return b->math - a->math; 
}

int compare_and_count(
    const Student *a, 
    const Student *b, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics *metrics
) {
    if (metrics != NULL) {
        metrics->comparisons++;
    }
    
    return compare_func(a, b);
}

void record_memory_usage(
    PerformanceMetrics *metrics, 
    size_t auxiliary_memory_bytes
) {
    if (metrics != NULL) {
        metrics->memory_usage = (double)auxiliary_memory_bytes;
    }
}


void swap_students(Student *a, Student *b) {
    if (!a || !b) {
        abort(); 
    }
    
    Student temp = *a;
    *a = *b;
    *b = temp;
}

//Bubble Sort Algorithm
void bubble_sort(
    Student* arr, 
    int n, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics* metrics
) {
    if (metrics != NULL) {
        metrics->comparisons = 0;
        metrics->memory_usage = sizeof(Student);
    }
    
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0; 
        
        for (int j = 0; j < n - 1 - i; j++) {
            if (compare_and_count(
                &arr[j], 
                &arr[j+1], 
                compare_func, 
                metrics
            ) > 0) {
                
                swap_students(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }
        
        if (swapped == 0) {
            break; 
        }
    }
    
}

//Selection Sort Algorithm
void selection_sort(
    Student* arr, 
    int n, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics* metrics
) {
    if (metrics != NULL) {
        metrics->comparisons = 0;
        metrics->memory_usage = sizeof(Student);
    }

    if (n <= 1) {
        return;
    }

    int min_idx;

    for (int i = 0; i < n - 1; i++) {
        min_idx = i;
        
        for (int j = i + 1; j < n; j++) {
            if (compare_and_count(
                &arr[j], 
                &arr[min_idx], 
                compare_func, 
                metrics
            ) < 0) {
                min_idx = j;
            }
        }

        if (min_idx != i) {
            swap_students(&arr[i], &arr[min_idx]);
        }
    }
}

//Insertion Sort Algorithm
void insertion_sort(
    Student* arr, 
    int n, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics* metrics
) {
    if (metrics != NULL) {
        metrics->comparisons = 0;
        metrics->memory_usage =  sizeof(Student);
    }

    if (n <= 1) {
        return;
    }

    for (int i = 1; i < n; i++) {
        Student key = arr[i];
        int j = i - 1;

        while (j >= 0) {
            if (compare_and_count(
                &arr[j], 
                &key, 
                compare_func, 
                metrics
            ) > 0) {
                arr[j + 1] = arr[j];
                j = j - 1;
            } else {
                break;
            }
        }
        
        arr[j + 1] = key;
    }
    
}

//Shell Sort Algorithm
void shell_sort(
    Student* arr, 
    int n, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics* metrics
) {
    if (metrics != NULL) {
        metrics->comparisons = 0;
        metrics->memory_usage = sizeof(Student);
    }
    if (n <= 1) {
        return;
    }
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            Student temp = arr[i];
            int j;
            for (j = i; j >= gap; j -= gap) {
                if (compare_and_count(
                    &arr[j - gap], 
                    &temp, 
                    compare_func, 
                    metrics
                ) > 0) {
                    arr[j] = arr[j - gap];
                } else {
                    break;
                }
            }
            arr[j] = temp;
        }
    }
}

//Quick Sort Algorithm
int partition_students(
    Student* arr, 
    int low, 
    int high, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics* metrics
) {
    Student pivot = arr[high]; 
    int i = low;

    for (int j = low; j < high; j++) {
        if (compare_and_count(&arr[j], &pivot, compare_func, metrics) <= 0) {
            swap_students(&arr[i], &arr[j]);
            i++; 
        }
    }

    swap_students(&arr[i], &arr[high]);
    return i;
}

void quick_sort_recursive(
    Student* arr, 
    int low, 
    int high, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics* metrics
) {
    while (low < high) { 
        int pi = partition_students(arr, low, high, compare_func, metrics);
        
        if (pi - low < high - pi) {
            quick_sort_recursive(arr, low, pi - 1, compare_func, metrics);
            low = pi + 1;
        } else {
            quick_sort_recursive(arr, pi + 1, high, compare_func, metrics);
            high = pi - 1;
        }
    }
}

int integer_log2(int n) {
    if (n <= 1) return 0;
    int log_val = 0;
    while (n > 1) {
        n >>= 1; // n = n / 2
        log_val++;
    }
    return log_val;
}

void quick_sort(
    Student* arr, 
    int n, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics* metrics
) {
    if (metrics != NULL) {
        metrics->comparisons = 0;
        metrics->memory_usage = 0.0; 
    }

    if (!arr || n <= 1) return;
    
    quick_sort_recursive(arr, 0, n - 1, compare_func, metrics);
    int log_n = integer_log2(n);
    const double STACK_FRAME_SIZE_BYTES = 64.0; 
    double theoretical_stack_usage = STACK_FRAME_SIZE_BYTES * (double)log_n;
    record_memory_usage(metrics, theoretical_stack_usage);
}

//Heap Sort Algorithm
void heapify(
    Student* arr, 
    int n,
    int i,
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics* metrics
) {
    int largest = i; 
    int left = 2 * i + 1; 
    int right = 2 * i + 2; 

    if (left < n) {
        if (compare_and_count(
            &arr[left], 
            &arr[largest], 
            compare_func, 
            metrics
        ) > 0) {
            largest = left;
        }
    }

    if (right < n) {
        if (compare_and_count(
            &arr[right], 
            &arr[largest], 
            compare_func, 
            metrics
        ) > 0) {
            largest = right;
        }
    }

    if (largest != i) {
        swap_students(&arr[i], &arr[largest]);
        heapify(arr, n, largest, compare_func, metrics);
    }
}

void heap_sort(
    Student* arr, 
    int n, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics* metrics
) {
    if (metrics != NULL) {
        metrics->comparisons = 0;
        metrics->memory_usage = sizeof(Student);
    }

    if (n <= 1) {
        return;
    }
    
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i, compare_func, metrics);
    }

    for (int i = n - 1; i > 0; i--) {
        swap_students(&arr[0], &arr[i]);

        heapify(arr, i, 0, compare_func, metrics);
    }
}

//Merge Sort Algorithm
void merge_students(
    Student* arr, 
    int left, 
    int mid, 
    int right, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics* metrics
) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Student* L = (Student*)malloc(sizeof(Student) * n1);
    Student* R = (Student*)malloc(sizeof(Student) * n2);

    if (!L || !R) {
        if (L) free(L);
        if (R) free(R);
        return;
    }

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    
    int i = 0; 
    int j = 0; 
    int k = left;

    while (i < n1 && j < n2) {
        if (compare_and_count(
            &L[i], 
            &R[j], 
            compare_func, 
            metrics
        ) <= 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
    
}

void merge_sort_recursive(
    Student* arr, 
    int left, 
    int right, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics* metrics
) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_recursive(arr, left, mid, compare_func, metrics);
        merge_sort_recursive(arr, mid + 1, right, compare_func, metrics);
        merge_students(arr, left, mid, right, compare_func, metrics);
    }
}

void merge_sort(
    Student* arr, 
    int n, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics* metrics
) {
    if (metrics != NULL) {
        metrics->comparisons = 0;
        metrics->memory_usage = 0.0;
    }

    if (n <= 1) {
        return;
    }
    
    merge_sort_recursive(arr, 0, n - 1, compare_func, metrics);
    record_memory_usage(metrics, sizeof(Student) * n); 
}

//Radix Sort Algorithm
void counting_sort_by_digit(
    Student* arr, 
    int n, 
    int exp, 
    int (*get_key)(const Student*),
    PerformanceMetrics* metrics
) {
    Student* output = (Student*)malloc(sizeof(Student) * n);
    int count[10] = {0}; 
    
    if (!output) {
        return;
    }

    for (int i = 0; i < n; i++) {
        int key = get_key(&arr[i]);
        count[(key / exp) % 10]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int key = get_key(&arr[i]);
        output[count[(key / exp) % 10] - 1] = arr[i];
        count[(key / exp) % 10]--;
    }

    memcpy(arr, output, sizeof(Student) * n);
    free(output);
}

int get_max_key(const Student* arr, int n, int (*get_key)(const Student*)) {
    if (n == 0) return 0;
    int max = get_key(&arr[0]);
    for (int i = 1; i < n; i++) {
        int key = get_key(&arr[i]);
        if (key > max) {
            max = key;
        }
    }
    return max;
}

void radix_sort(
    Student* arr, 
    int n, 
    int (*get_key)(const Student*),
    PerformanceMetrics* metrics
) {
    if (metrics != NULL) {
        metrics->comparisons = 0; 
        metrics->memory_usage = 0.0;
    }

    if (n <= 1) {
        return;
    }

    int max_key = get_max_key(arr, n, get_key);

    for (int exp = 1; max_key / exp > 0; exp *= 10) {
        counting_sort_by_digit(arr, n, exp, get_key, metrics);
    }
    
    record_memory_usage(metrics, sizeof(Student) * n + sizeof(int) * 10); 
}

int get_id_key(const Student *s) {
    return s->id;
}
int get_total_score_key(const Student *s) {
    return s->total_score;
}

int get_gender_key(const Student *s) {
    return (int)s->gender; 
}

int get_char_key(const Student *s, int index) {
    int len = strlen(s->name);
    if (index >= len) {
        return 0;
    }
    return (int)s->name[index];
}

void counting_sort_by_char(
    Student* arr, 
    int n, 
    int index, 
    PerformanceMetrics* metrics
) {
    Student* output = (Student*)malloc(sizeof(Student) * n);
    int count[256] = {0}; 
    
    if (!output) {
        return;
    }

    for (int i = 0; i < n; i++) {
        int key = get_char_key(&arr[i], index);
        count[key]++;
    }

    for (int i = 1; i < 256; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int key = get_char_key(&arr[i], index);
        output[count[key] - 1] = arr[i];
        count[key]--;
    }

    memcpy(arr, output, sizeof(Student) * n);

    free(output);
}

void radix_sort_wrapper_id(Student* arr, int n, int (*compare_func)(const Student*, const Student*), PerformanceMetrics* metrics) {
    radix_sort(arr, n, get_id_key, metrics);
}

void radix_sort_wrapper_total(Student* arr, int n, int (*compare_func)(const Student*, const Student*), PerformanceMetrics* metrics) {
    radix_sort(arr, n, get_total_score_key, metrics);
}

void radix_sort_wrapper_gender(Student* arr, int n, int (*compare_func)(const Student*, const Student*), PerformanceMetrics* metrics) {
    int max_key = get_max_key(arr, n, get_gender_key);

    if (metrics != NULL) metrics->comparisons = 0;
    if (n <= 1) return;

    counting_sort_by_digit(arr, n, 1, get_gender_key, metrics); 
    
    record_memory_usage(metrics, sizeof(Student) * n + sizeof(int) * 10); 
}

void radix_sort_wrapper_name(Student* arr, int n, int (*compare_func)(const Student*, const Student*), PerformanceMetrics* metrics) {
    if (metrics != NULL) metrics->comparisons = 0; 
    if (n <= 1) return;

    int max_len = 0;
    for (int i = 0; i < n; i++) {
        int len = strlen(arr[i].name);
        if (len > max_len) {
            max_len = len;
        }
    }

    for (int index = max_len - 1; index >= 0; index--) {
        counting_sort_by_char(arr, n, index, metrics);
    }
    record_memory_usage(metrics, sizeof(Student) * n + sizeof(int) * 256); 
}

//Tree Sort Algorithm
TreeNode* new_node(Student data) {
    TreeNode* temp = (TreeNode*)malloc(sizeof(TreeNode));
    if (temp == NULL) {
        perror("Error allocating TreeNode");
        return NULL;
    }
    temp->data = data;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

TreeNode* insert_node(
    TreeNode* root,
    Student data,
    int (*compare_func)(const Student*, const Student*),
    PerformanceMetrics* metrics
) {
    if (root == NULL) {
        TreeNode* new = new_node(data);
        if (new != NULL && metrics != NULL) {
            metrics->memory_usage += sizeof(TreeNode);
            metrics->comparisons++;
        }
        return new;
    }
    
    typedef struct {
        TreeNode** node_ptr;
    } StackFrame;
    
    StackFrame* stack = (StackFrame*)malloc(sizeof(StackFrame) * 100000);
    if (!stack) {
        fprintf(stderr, "ERROR: Cannot allocate insertion stack\n");
        return root;
    }
    
    int stack_top = 0;
    
    stack[stack_top].node_ptr = &root;
    stack_top++;
    
    while (stack_top > 0) {
        stack_top--;
        StackFrame* frame = &stack[stack_top];
        
        TreeNode* current = *frame->node_ptr;
        
        if (current == NULL) {
            *frame->node_ptr = new_node(data);
            if (*frame->node_ptr != NULL && metrics != NULL) {
                metrics->memory_usage += sizeof(TreeNode);
            }
            break;
        }
        
        if (metrics != NULL) {
            metrics->comparisons++;
        }
        
        int cmp = compare_func(&data, &current->data);
        
        if (cmp < 0) {
            stack[stack_top].node_ptr = &current->left;
            stack_top++;
        } else {
            stack[stack_top].node_ptr = &current->right;
            stack_top++;
        }
    }
    
    free(stack);
    return root;
}

typedef struct {
    TreeNode* node;
    int phase;
} TraversalFrame;

void inorder_traversal(
    TreeNode* root, 
    Student* arr, 
    int* index, 
    PerformanceMetrics* metrics
) {
    if (root == NULL) return;
    
    TraversalFrame* stack = (TraversalFrame*)malloc(sizeof(TraversalFrame) * 100000);
    if (!stack) {
        fprintf(stderr, "ERROR: Cannot allocate traversal stack\n");
        return;
    }
    
    int stack_top = 0;
    
    stack[stack_top].node = root;
    stack[stack_top].phase = 0;
    stack_top++;
    
    while (stack_top > 0) {
        stack_top--;
        TraversalFrame* frame = &stack[stack_top];
        
        if (frame->node == NULL) continue;
        
        if (frame->phase == 0) {
            stack[stack_top].node = frame->node;
            stack[stack_top].phase = 1;
            stack_top++;
            
            if (frame->node->left != NULL) {
                stack[stack_top].node = frame->node->left;
                stack[stack_top].phase = 0;
                stack_top++;
            }
        } 
        else if (frame->phase == 1) {
            arr[*index] = frame->node->data;
            (*index)++;
            
            if (metrics != NULL) {
                metrics->comparisons++;
            }
            
            stack[stack_top].node = frame->node;
            stack[stack_top].phase = 2;
            stack_top++;
            
            if (frame->node->right != NULL) {
                stack[stack_top].node = frame->node->right;
                stack[stack_top].phase = 0;
                stack_top++;
            }
        }
    }
    
    free(stack);
}

void free_tree(TreeNode* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

void tree_sort(
    Student* arr, 
    int n, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics* metrics
) {
    if (metrics != NULL) {
        metrics->comparisons = 0;
        metrics->memory_usage = 0.0;
    }

    if (n <= 1) {
        return;
    }
    
    TreeNode* root = NULL;

    for (int i = 0; i < n; i++) {
        root = insert_node(root, arr[i], compare_func, metrics);
        
        if (root == NULL && i > 0) {
            return;
        }
        
    }
    
    Student* sorted_arr = (Student*)malloc(sizeof(Student) * n);
    if (!sorted_arr) {
        free_tree(root);
        return;
    }
    
    int index = 0;
    inorder_traversal(root, sorted_arr, &index, metrics);
    
    
    if (index == n) {
        memcpy(arr, sorted_arr, sizeof(Student) * n);
    }
    
    free(sorted_arr);
    free_tree(root);
}
Student* copy_students(const Student* source, int n) {
    if (!source || n <= 0) return NULL;
    Student* copy = (Student*)malloc(sizeof(Student) * n);
    if (copy) {
        memcpy(copy, source, sizeof(Student) * n);
    }
    return copy;
}

void run_and_average_sort(
    const Student* original_arr,
    int n,
    int (*compare_func)(const Student*, const Student*),
    void (*sort_func)(Student*, int, int (*)(const Student*, const Student*), PerformanceMetrics*),
    const char* sort_name,
    const char* comparison_name
) {
    if (n == 0) return;
    long long total_comparisons = 0;
    double total_memory_usage = 0.0;
    
    PerformanceMetrics current_metrics;
    printf("\n>>> %s 로 %s 정렬 알고리즘<<<\n", comparison_name, sort_name);
    
    for (int i = 0; i < NUM_REPETITIONS; i++) {
        current_metrics.comparisons = 0;
        Student* temp_arr = copy_students(original_arr, n);
        if (!temp_arr) {
            continue;
        }

        if (temp_arr == original_arr) {
            continue;
    }

        sort_func(temp_arr, n, compare_func, &current_metrics);

        total_comparisons += current_metrics.comparisons;
        total_memory_usage += current_metrics.memory_usage;

        free(temp_arr);
    }
    
    double avg_comparisons = (double)total_comparisons / NUM_REPETITIONS;
    double avg_memory_usage = total_memory_usage / NUM_REPETITIONS;
    printf("  평균 비교 횟수: %.2f\n", avg_comparisons);
    printf("  사용한 메모리: %.2f\n", avg_memory_usage);
    printf("----------------------------------------\n");
}

int has_duplicates(const Student* arr, int n) {
    if (n <= 1) return 0;
    Student* temp_arr = copy_students(arr, n);
    if (!temp_arr) {
        return 0; 
    }

    PerformanceMetrics dummy_metrics = {0};
    
    bubble_sort(temp_arr, n, compare_id_asc, &dummy_metrics); 
    
    int duplicates_found = 0;

    for (int i = 0; i < n - 1; i++) {
        if (temp_arr[i].id == temp_arr[i+1].id &&
            strcmp(temp_arr[i].name, temp_arr[i+1].name) == 0 &&
            temp_arr[i].gender == temp_arr[i+1].gender &&
            temp_arr[i].korean == temp_arr[i+1].korean &&
            temp_arr[i].english == temp_arr[i+1].english &&
            temp_arr[i].math == temp_arr[i+1].math) 
        {
            duplicates_found = 1;
            break; 
        }
    }

    free(temp_arr);
    return duplicates_found;
}

typedef struct {
    const char* name;
    int (*compare_func)(const Student*, const Student*);
    int is_stable_only; // GENDER 기준은 Stable 정렬만을 사용
} TestCriteria;

typedef struct {
    const char* name;
    void (*sort_func)(Student*, int, int (*)(const Student*, const Student*), PerformanceMetrics*);
    int is_stable; // 1 Stable, 0 Unstable
    int requires_unique_data; // 중복되지 않는 데이테 필요 (Heap, Tree)
} SortAlgorithm;

TestCriteria criteria[] = {
    {"ID 기준 오름차순", compare_id_asc, 0}, 
    {"ID 기준 내림차순", compare_id_desc, 0},
    {"NAME 기준 오름차순", compare_name_asc, 0},
    {"NAME 기준 내림차순", compare_name_desc, 0},
    {"GENDER 기준 오름차순", compare_gender_asc, 1}, // 1: Stable ONLY
    {"GENDER 기준 내림차순", compare_gender_desc, 1}, // 1: Stable ONLY
    {"3가지 GRADE의 합 기준 오름차순", compare_total_score_asc, 0},
    {"3가지 GRADE의 합 기준 내림차순", compare_total_score_desc, 0}
};
const int NUM_CRITERIA = sizeof(criteria) / sizeof(criteria[0]);

SortAlgorithm algorithms[] = {
    {"Bubble Sort", bubble_sort, 1, 0},
    {"Selection Sort", selection_sort, 0, 0},     
    {"Insertion Sort", insertion_sort, 1, 0},    
    {"Shell Sort", shell_sort, 0, 0},            
    {"Quick Sort", quick_sort, 0, 0},
    {"Heap Sort", heap_sort, 0, 1},              // Unique=1
    {"Merge Sort", merge_sort, 1, 0},            
    {"Radix Sort", radix_sort_wrapper_id, 1, 0},
    {"Tree Sort", tree_sort, 0, 1}               // Unique=1
};
const int NUM_ALGORITHMS = sizeof(algorithms) / sizeof(algorithms[0]);

int main() {
    int student_count = 0;
    const char* filename = "C:\\Users\\lastg\\Downloads\\dataset_id_ascending.csv"; 

    Student* all_students = load_students(filename, &student_count);

    printf("----------------------------------------\n");
    if (!all_students) {
        return 1;
    }
    
    int data_has_duplicates = has_duplicates(all_students, student_count);

    printf("\n======================================================\n");
    if (data_has_duplicates) {
        printf("중복 데이터가 있습니다\n");
    } else {
        printf("중복 데이터가 없습니다\n");
    }
    printf("======================================================\n\n");

    for (int a = 0; a < NUM_ALGORITHMS; a++) {
        for (int c = 0; c < NUM_CRITERIA; c++) {
            
            if (data_has_duplicates && algorithms[a].requires_unique_data) {
                printf("중복 데이터가 있으니까 %s 건너뜁니다\n", 
                       algorithms[a].name);
                continue; 
            }
            
            if (criteria[c].is_stable_only && !algorithms[a].is_stable) {
                printf("%s 는 Stable 정렬 알고리즘 아닙니다. %s 건너뜁니다. ---\n", 
                       algorithms[a].name, criteria[c].name);
                continue; 
            }
            
            void (*current_sort_func)(Student*, int, int (*)(const Student*, const Student*), PerformanceMetrics*) = algorithms[a].sort_func;
            
            if (strcmp(algorithms[a].name, "Radix Sort") == 0) {
                if (strstr(criteria[c].name, "Name") != NULL) {
                    current_sort_func = radix_sort_wrapper_name;
                } else if (strstr(criteria[c].name, "GENDER") != NULL) {
                    current_sort_func = radix_sort_wrapper_gender;
                } else if (strstr(criteria[c].name, "Tổng điểm") != NULL) {
                    current_sort_func = radix_sort_wrapper_total;
                } else { // ID
                    current_sort_func = radix_sort_wrapper_id;
                }
            }

            run_and_average_sort(
                all_students, 
                student_count, 
                criteria[c].compare_func, 
                current_sort_func,
                algorithms[a].name, 
                criteria[c].name
            );
        }
    }

    free(all_students);

    return 0;
}