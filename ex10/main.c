#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LEN 200
#define RANDOM_MAX 1000000LL
#define MAX_TRIES 100 

long long* load_product_scores(const char* filename, int* out_count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open file");
        return NULL;
    }

    char line[MAX_LINE_LEN];
    int capacity = 10;
    int count = 0;
    long long* product_arr = malloc(sizeof(long long) * capacity);

    if (!product_arr) {
        perror("Memory allocation failed");
        fclose(fp);
        return NULL;
    }

    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp)) {
        if (count >= capacity) {
            capacity *= 2;
            long long* temp = realloc(product_arr, sizeof(long long) * capacity);
            if (!temp) {
                perror("Reallocation failed");
                free(product_arr);
                fclose(fp);
                return NULL;
            }
            product_arr = temp;
        }

        int korean = 0, english = 0, math = 0;
        char* token = NULL;

        strtok(line, ","); // ID
        strtok(NULL, ","); // Name
        strtok(NULL, ","); // Gender

        token = strtok(NULL, ",");
        if (token) korean = atoi(token);

        token = strtok(NULL, ",");
        if (token) english = atoi(token);

        token = strtok(NULL, ",");
        if (token) math = atoi(token);

        long long product_score = (long long)korean * english * math; 
        product_arr[count++] = product_score;
    }

    fclose(fp);

    long long* tight = realloc(product_arr, sizeof(long long) * count);
    if (!tight) {
        fprintf(stderr, "Warning: Tight reallocation failed, using original memory.\n");
        *out_count = count;
        return product_arr;
    }

    product_arr = tight;

    *out_count = count;
    return product_arr;
}

long long sequential_search(const long long arr[], int size, long long key) {
    long long comparisons = 0;
    for (int i = 0; i < size; i++) {
        comparisons++; 
        if (arr[i] == key) {
            return comparisons;  
        }
    }
    return -1;
}

// Swap
void swap(long long* a, long long* b) {
    long long t = *a;
    *a = *b;
    *b = t;
}

// Partition
int partition(long long arr[], int low, int high, long long* comparisons) {
    long long pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        (*comparisons)++;
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Quick Sort
void quick_sort_manual(long long arr[], int low, int high, long long* comparisons) {
    if (low < high) {
        int pi = partition(arr, low, high, comparisons);
        quick_sort_manual(arr, low, pi - 1, comparisons);
        quick_sort_manual(arr, pi + 1, high, comparisons);
    }
}

// Binary Search
long long binary_search(const long long arr[], int size, long long key) {
    long long comparisons = 0;
    int low = 0;
    int high = size - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        comparisons++;
        if (arr[mid] == key) {
            return comparisons;
        }

        if (arr[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return comparisons;
}

int main() {
    srand(time(NULL));  

    const char* filename = "C:\\Users\\lastg\\Downloads\\dataset_id_ascending.csv";
    int num_students = 0;
    long long* original_scores = load_product_scores(filename, &num_students);

    int attempts = 0;
    long long search_key;
    long long seq_comparisons = 0;
    int found = 0;

    while (!found && attempts < MAX_TRIES) {
        search_key = ((long long)rand() << 32) | rand(); 
        search_key = search_key % RANDOM_MAX;

        
        seq_comparisons = sequential_search(original_scores, num_students, search_key);
        
        if (seq_comparisons != -1) {
            printf("랜덤 수: %lld (0 ~ %lld)\n", search_key, RANDOM_MAX);
            found = 1;
        } 
        attempts++;
    }

    if (!found) {
        printf("랜덤 수 없습니다\n", attempts);
        free(original_scores);
        return 1; 
    }

    long long* sorted_scores = malloc(sizeof(long long) * num_students);
    if (!sorted_scores) {
        perror("Memory allocation failed for sorted array");
        free(original_scores);
        return 1;
    }
    memcpy(sorted_scores, original_scores, sizeof(long long) * num_students);

    long long sort_comparisons = 0;
    // Quick Sort
    quick_sort_manual(sorted_scores, 0, num_students - 1, &sort_comparisons);
    printf("Quick Sort 비교 횟수: %lld\n", sort_comparisons);

    // Binary Search
    long long bin_comparisons = binary_search(sorted_scores, num_students, search_key);
    printf("Binary Search 비교 횟수: %lld\n", bin_comparisons);

    long long total_comparisons_binary = sort_comparisons + bin_comparisons;

    printf("\n결론:\n");
    printf("Sequential Search: %lld 비교 횟수\n", seq_comparisons);
    printf("Quick Sort + Binary Search: %lld 비교 횟수\n", total_comparisons_binary);

    free(original_scores);
    free(sorted_scores);

    return 0;
}
