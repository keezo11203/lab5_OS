#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100

int array[SIZE];

void fillArrayWithRandomNumbers(int arr[SIZE]) {
    for (int i = 0; i < SIZE; i++)
        arr[i] = rand() % 100;
}

void printArray(int arr[SIZE]) {
    for (int i = 0; i < SIZE; i++)
        printf("%5d", arr[i]);
    printf("\n");
}

typedef struct StartEndIndexes {
    int start;
    int end;
} StartEndIndexes;

// Merges two subarrays of array[].
// First subarray is array[start..mid]
// Second subarray is array[mid+1..end]
void merge(int arr[], int start, int mid, int end) {
    int n1 = mid - start + 1;
    int n2 = end - mid;

    // Create temporary arrays
    int L[n1], R[n2];

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[start + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[start..end]
    int i = 0;    // Initial index of first subarray
    int j = 0;    // Initial index of second subarray
    int k = start; // Initial index of merged subarray

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Runs mergesort on the array segment described in the argument.
// Spawns two threads to mergesort each half of the array segment,
// and then merges the results.
void *mergeSort(void *args) {
    StartEndIndexes *sei = (StartEndIndexes *)args;
    int start = sei->start;
    int end = sei->end;

    if (start < end) {
        // Same as (start + end) / 2, but avoids overflow for large start and end
        int mid = start + (end - start) / 2;

        // Create two threads for the left and right halves
        pthread_t left_thread, right_thread;

        StartEndIndexes left_sei;
        left_sei.start = start;
        left_sei.end = mid;

        StartEndIndexes right_sei;
        right_sei.start = mid + 1;
        right_sei.end = end;

        pthread_create(&left_thread, NULL, mergeSort, &left_sei);
        pthread_create(&right_thread, NULL, mergeSort, &right_sei);

        // Wait for the threads to finish
        pthread_join(left_thread, NULL);
        pthread_join(right_thread, NULL);

        // Merge the sorted halves
        merge(array, start, mid, end);
    }

    pthread_exit(NULL);
}

int main() {
    srand(time(0));
    StartEndIndexes sei;
    sei.start = 0;
    sei.end = SIZE - 1;

    // 1. Fill array with random numbers.
    fillArrayWithRandomNumbers(array);

    // 2. Print the array.
    printf("Unsorted array: ");
    printArray(array);

    // 3. Create a thread for merge sort.
    pthread_t sort_thread;
    pthread_create(&sort_thread, NULL, mergeSort, &sei);

    // 4. Wait for mergesort to finish.
    pthread_join(sort_thread, NULL);

    // 5. Print the sorted array.
    printf("Sorted array:   ");
    printArray(array);

    return 0;
}
