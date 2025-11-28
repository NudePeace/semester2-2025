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
} PerformanceMetrics;

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

Student* copy_students(const Student* source, int n) {
    if (!source || n <= 0) return NULL;
    Student* copy = (Student*)malloc(sizeof(Student) * n);
    if (copy) {
        memcpy(copy, source, sizeof(Student) * n);
    }
    return copy;
}

//Shell Sort Sedgewick
void shell_sort(
    Student* arr, 
    int n, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics *metrics
) {
    if (n <= 1) {
        metrics->comparisons = 0;
        return;
    }

    metrics->comparisons = 0;
    int gaps[] = {1, 5, 19, 41, 109, 209, 505, 929, 2161, 3905, 7193, 13321, 24781, 46169, 86161, 161281, 302521, 568385, 1068697, 2009281};
    int num_gaps = sizeof(gaps) / sizeof(int);
    
    int k = 0;
    while (k < num_gaps && gaps[k] < n) {
        k++;
    }
    k--; 

    for (int g = k; g >= 0; g--) {
        int gap = gaps[g];
        if (gap == 0) continue; 
        
        for (int i = gap; i < n; i++) {
            Student temp = arr[i];
            int j;
            
            for (j = i; j >= gap && compare_and_count(&arr[j - gap], &temp, compare_func, metrics) > 0; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}


//Quick Sort Median of Three
void swap(Student* a, Student* b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

int get_median_index(
    Student* arr, 
    int a, 
    int b, 
    int c, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics *metrics
) {
    if (compare_and_count(&arr[a], &arr[b], compare_func, metrics) > 0) {
        swap(&arr[a], &arr[b]);
    }
    if (compare_and_count(&arr[a], &arr[c], compare_func, metrics) > 0) {
        swap(&arr[a], &arr[c]);
    }
    if (compare_and_count(&arr[b], &arr[c], compare_func, metrics) > 0) {
        swap(&arr[b], &arr[c]);
    }
    return b;
}

int partition(
    Student* arr, 
    int low, 
    int high, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics *metrics
) {
    int mid = low + (high - low) / 2;
    int pivot_index = get_median_index(arr, low, mid, high, compare_func, metrics);
    
    swap(&arr[pivot_index], &arr[high]);

    Student pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (compare_and_count(&arr[j], &pivot, compare_func, metrics) <= 0) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quicksort_recursive(
    Student* arr, 
    int low, 
    int high, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics *metrics
) {
    if (low < high) {
        int pi = partition(arr, low, high, compare_func, metrics);

        quicksort_recursive(arr, low, pi - 1, compare_func, metrics);
        quicksort_recursive(arr, pi + 1, high, compare_func, metrics);
    }
}

void quick_sort(
    Student* arr, 
    int n, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics *metrics
) {
    if (n <= 1) {
        metrics->comparisons = 0;
        return;
    }
    
    quicksort_recursive(arr, 0, n - 1, compare_func, metrics);
}

typedef struct AVLNode {
    Student data;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

int get_height(AVLNode* node) {
    if (node == NULL) return 0;
    return node->height;
}

int get_balance_factor(AVLNode* node) {
    if (node == NULL) return 0;
    return get_height(node->left) - get_height(node->right);
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

AVLNode* new_node(Student s) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    if (!node) {
        perror("Memory allocation failed for AVL node");
        return NULL;
    }
    node->data = s;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode* right_rotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;

    return x;
}

AVLNode* left_rotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;

    return y;
}

AVLNode* insert_avl(
    AVLNode* node, 
    Student s, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics *metrics
) {
    if (node == NULL) {
        return new_node(s);
    }
    
    int cmp = compare_and_count(&s, &node->data, compare_func, metrics);

    if (cmp < 0) { 
        node->left = insert_avl(node->left, s, compare_func, metrics);
    } else if (cmp > 0) {
        node->right = insert_avl(node->right, s, compare_func, metrics);
    } else {
        return node; 
    }

    node->height = 1 + max(get_height(node->left), get_height(node->right));

    int balance = get_balance_factor(node);

    if (balance > 1) {
        int left_balance = get_balance_factor(node->left); 
        
        if (left_balance >= 0) {
            return right_rotate(node);
        } else { 
            node->left = left_rotate(node->left);
            return right_rotate(node);
        }
    }

    if (balance < -1) {
        int right_balance = get_balance_factor(node->right);
        
        if (right_balance <= 0) {
            return left_rotate(node);
        } else {
            node->right = right_rotate(node->right);
            return left_rotate(node);
        }
    }

    return node;
}

void inorder_traversal(AVLNode* root, Student* arr, int* index) {
    if (root != NULL) {
        inorder_traversal(root->left, arr, index);
        arr[(*index)++] = root->data;
        inorder_traversal(root->right, arr, index);
    }
}

void free_avl_tree(AVLNode* root) {
    if (root != NULL) {
        free_avl_tree(root->left);
        free_avl_tree(root->right);
        free(root);
    }
}

void tree_sort(
    Student* arr, 
    int n, 
    int (*compare_func)(const Student*, const Student*), 
    PerformanceMetrics *metrics
) {
    if (n <= 1) {
        metrics->comparisons = 0;
        return;
    }
    
    AVLNode* root = NULL;
    metrics->comparisons = 0;
    
    for (int i = 0; i < n; i++) {
        root = insert_avl(root, arr[i], compare_func, metrics);
    }

    int index = 0;
    inorder_traversal(root, arr, &index);
    free_avl_tree(root);
    
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

        free(temp_arr);
    }
    
    double avg_comparisons = (double)total_comparisons / NUM_REPETITIONS;
    printf("  평균 비교 횟수: %.2f\n", avg_comparisons);
    printf("----------------------------------------\n");
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
    {"Shell Sort", shell_sort, 0, 0},         
    {"Quick Sort", quick_sort, 0, 0},
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

    for (int a = 0; a < NUM_ALGORITHMS; a++) {
        for (int c = 0; c < NUM_CRITERIA; c++) {
            
            if (criteria[c].is_stable_only && !algorithms[a].is_stable) {
                printf("%s 는 Stable 정렬 알고리즘 아닙니다. %s 건너뜁니다. ---\n", 
                       algorithms[a].name, criteria[c].name);
                continue; 
            }
            
            void (*current_sort_func)(Student*, int, int (*)(const Student*, const Student*), PerformanceMetrics*) = algorithms[a].sort_func;

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