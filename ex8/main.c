#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DATA_SIZE 10000
#define MAX_VALUE 1000000
#define NUM_TRIALS 100

void generateRandomData(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % (MAX_VALUE + 1);
    }
}

// Insertion Sort
long long insertionSort(int arr[], int size) {
    long long comparisons = 0;
    for (int i = 1; i < size; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= 0) {
            comparisons++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
    return comparisons;
}

// Basic Shell Sort 
long long shellSortBasic(int arr[], int size) {
    long long comparisons = 0;
    
    for (int gap = size / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < size; i++) {
            int temp = arr[i];
            int j;
            
            for (j = i; j >= gap; j -= gap) {
                comparisons++;
                if (arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                } else {
                    break;
                }
            }
            arr[j] = temp;
        }
    }
    return comparisons;
}

// Shell Sort Knuth's sequence (3^k - 1)
long long shellSortKnuth(int arr[], int size) {
    long long comparisons = 0;
    
    int gap = 1;
    while (gap < size / 3) {
        gap = 3 * gap + 1;
    }
    
    while (gap > 0) {
        for (int i = gap; i < size; i++) {
            int temp = arr[i];
            int j;
            
            for (j = i; j >= gap; j -= gap) {
                comparisons++;
                if (arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                } else {
                    break;
                }
            }
            arr[j] = temp;
        }
        gap /= 3;
    }
    return comparisons;
}

// Shell Sort Sedgewick's sequence
long long shellSortSedgewick(int arr[], int size) {
    long long comparisons = 0;
    
    int gaps[20];
    int k = 0;
    
    gaps[k++] = 1;
    for (int i = 1; k < 20; i++) {
        int gap1 = (1 << (2*i)) + 3 * (1 << (i-1)) + 1; 
        if (gap1 >= size) break;
        gaps[k++] = gap1;
    }
    
    for (int g = k - 1; g >= 0; g--) {
        int gap = gaps[g];
        for (int i = gap; i < size; i++) {
            int temp = arr[i];
            int j;
            
            for (j = i; j >= gap; j -= gap) {
                comparisons++;
                if (arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                } else {
                    break;
                }
            }
            arr[j] = temp;
        }
    }
    return comparisons;
}

int main() {
    srand(time(NULL));
    
    int *originalData = (int*)malloc(DATA_SIZE * sizeof(int));
    int *tempData = (int*)malloc(DATA_SIZE * sizeof(int));
    
    long long totalInsertionComparisons = 0;
    long long totalShellBasicComparisons = 0;
    long long totalShellKnuthComparisons = 0;
    long long totalShellSedgewickComparisons = 0;
    
    //Insertion Sort
    for (int trial = 0; trial < NUM_TRIALS; trial++) {
        generateRandomData(originalData, DATA_SIZE);
        memcpy(tempData, originalData, DATA_SIZE * sizeof(int));
        totalInsertionComparisons += insertionSort(tempData, DATA_SIZE);

    }
    
    //Shell Sort (Basic)
    for (int trial = 0; trial < NUM_TRIALS; trial++) {
        generateRandomData(originalData, DATA_SIZE);
        memcpy(tempData, originalData, DATA_SIZE * sizeof(int));
        totalShellBasicComparisons += shellSortBasic(tempData, DATA_SIZE);
    }
    
    //Shell Sort (Knuth)
    for (int trial = 0; trial < NUM_TRIALS; trial++) {
        generateRandomData(originalData, DATA_SIZE);
        memcpy(tempData, originalData, DATA_SIZE * sizeof(int));
        totalShellKnuthComparisons += shellSortKnuth(tempData, DATA_SIZE);
    }
    
    //Shell Sort (Sedgewick)
    for (int trial = 0; trial < NUM_TRIALS; trial++) {
        generateRandomData(originalData, DATA_SIZE);
        memcpy(tempData, originalData, DATA_SIZE * sizeof(int));
        totalShellSedgewickComparisons += shellSortSedgewick(tempData, DATA_SIZE);
    }
    
    long long avgInsertion = totalInsertionComparisons / NUM_TRIALS;
    long long avgShellBasic = totalShellBasicComparisons / NUM_TRIALS;
    long long avgShellKnuth = totalShellKnuthComparisons / NUM_TRIALS;
    long long avgShellSedgewick = totalShellSedgewickComparisons / NUM_TRIALS;
    
    printf("1. Insertion Sort:                    %lld 비교 횟수\n", avgInsertion);
    printf("2. Shell Sort (Basic):                %lld 비교 횟수\n", avgShellBasic);
    printf("3. Shell Sort (Knuth):                %lld 비교 횟수\n", avgShellKnuth);
    printf("4. Shell Sort (Sedgewick):            %lld 비교 횟수\n\n", avgShellSedgewick);
    
    long long minComparisons = avgInsertion;
    const char* bestAlgorithm = "Insertion Sort";
    
    if (avgShellBasic < minComparisons) {
        minComparisons = avgShellBasic;
        bestAlgorithm = "Shell Sort (Basic)";
    }
    if (avgShellKnuth < minComparisons) {
        minComparisons = avgShellKnuth;
        bestAlgorithm = "Shell Sort (Knuth)";
    }
    if (avgShellSedgewick < minComparisons) {
        minComparisons = avgShellSedgewick;
        bestAlgorithm = "Shell Sort (Sedgewick)";
    }
    
    printf("비교 횟수가 가장 낮은 알고리즘: %s\n", bestAlgorithm);
    printf("비교 횟수: %lld\n", minComparisons);
    
    free(originalData);
    free(tempData);
    
    return 0;
}