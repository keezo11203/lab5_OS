#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 3  // Adjust the matrix size as needed

int matA[MAX][MAX];
int matB[MAX][MAX];

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX];
int matProductResult[MAX][MAX];

// Structure to hold the thread information
typedef struct {
    int row;
    int col;
} ThreadInfo;

void fillMatrix(int matrix[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to compute the sum of matrices in parallel
void* computeSum(void* args) {
    ThreadInfo* info = (ThreadInfo*)args;
    int row = info->row;
    int col = info->col;
    matSumResult[row][col] = matA[row][col] + matB[row][col];
    free(info);
    return NULL;
}

// Function to compute the difference of matrices in parallel
void* computeDiff(void* args) {
    ThreadInfo* info = (ThreadInfo*)args;
    int row = info->row;
    int col = info->col;
    matDiffResult[row][col] = matA[row][col] - matB[row][col];
    free(info);
    return NULL;
}

// Function to compute the product of matrices in parallel
void* computeProduct(void* args) {
    ThreadInfo* info = (ThreadInfo*)args;
    int row = info->row;
    int col = info->col;
    matProductResult[row][col] = 0;
    for (int k = 0; k < MAX; k++) {
        matProductResult[row][col] += matA[row][k] * matB[k][col];
    }
    free(info);
    return NULL;
}

int main(int argc, char* argv[]) {
    srand(time(0));

    // 0. Get the matrix size from the command line and assign it to MAX
    // Assuming command line argument is provided. You might want to add error handling.

    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);

    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);

    // 3. Create pthread_t objects for our threads.
    pthread_t threads[MAX][MAX];

    // 4. Create a thread for each cell of each matrix operation.
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            ThreadInfo* info = malloc(sizeof(ThreadInfo));
            info->row = i;
            info->col = j;

            // Create threads for each operation
            pthread_create(&threads[i][j], NULL, computeSum, (void*)info);
            // Uncomment the lines below for subtraction and multiplication
            // pthread_create(&threads[i][j], NULL, computeDiff, (void*)info);
            // pthread_create(&threads[i][j], NULL, computeProduct, (void*)info);
        }
    }

    // 5. Wait for all threads to finish.
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }

    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);

    return 0;
}
