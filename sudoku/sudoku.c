#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9

// Sudoku board. Modify this to test your checker!
// 0 == empty cell; 1-9 is the filled in digit.
int board[SIZE][SIZE] = {
    {1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 2, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 4, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 5, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 6, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 7, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 8, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 9},
};

bool row_check[SIZE];
bool col_check[SIZE];
bool box_check[SIZE];

void printBoard(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%5d", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Checks the given row for duplicate numbers, and updates the row_check
// value for that row appropriately. If no number is repeated in that row,
// row_check[row] will be set to true; otherwise, it will be false.
void* checkRow(void* args) {
    int row = *((int*)args);
    bool seen[SIZE] = {false};
    for (int j = 0; j < SIZE; j++) {
        int num = board[row][j];
        if (num != 0) {
            if (seen[num - 1]) {
                row_check[row] = false;
                pthread_exit(NULL);
            } else {
                seen[num - 1] = true;
            }
        }
    }
    row_check[row] = true;
    pthread_exit(NULL);
}

// Checks the given col for duplicate numbers, and updates the col_check
// value for that col appropriately. If no number is repeated in that col,
// col_check[col] will be set to true; otherwise, it will be false.
void* checkCol(void* args) {
    int col = *((int*)args);
    bool seen[SIZE] = {false};
    for (int i = 0; i < SIZE; i++) {
        int num = board[i][col];
        if (num != 0) {
            if (seen[num - 1]) {
                col_check[col] = false;
                pthread_exit(NULL);
            } else {
                seen[num - 1] = true;
            }
        }
    }
    col_check[col] = true;
    pthread_exit(NULL);
}

// Checks the given 3x3 box for duplicate numbers, and updates the box_check
// value for that box appropriately. If no number is repeated in that box,
// box_check[box] will be set to true; otherwise, it will be false.
void* checkBox(void* args) {
    int box = *((int*)args);
    bool seen[SIZE] = {false};
    int startRow = (box / 3) * 3;
    int startCol = (box % 3) * 3;

    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            int num = board[i][j];
            if (num != 0) {
                if (seen[num - 1]) {
                    box_check[box] = false;
                    pthread_exit(NULL);
                } else {
                    seen[num - 1] = true;
                }
            }
        }
    }
    box_check[box] = true;
    pthread_exit(NULL);
}

int main() {
    // 1. Print the board.
    printf("Board:\n");
    printBoard(board);

    // 2. Create pthread_t objects for our threads.
    pthread_t row_threads[SIZE];
    pthread_t col_threads[SIZE];
    pthread_t box_threads[SIZE];

    // 3. Create a thread for each cell of each matrix operation.
    int row_args[SIZE];
    int col_args[SIZE];
    int box_args[SIZE];

    for (int i = 0; i < SIZE; i++) {
        row_args[i] = i;
        pthread_create(&row_threads[i], NULL, checkRow, (void*)&row_args[i]);

        col_args[i] = i;
        pthread_create(&col_threads[i], NULL, checkCol, (void*)&col_args[i]);

        box_args[i] = i;
        pthread_create(&box_threads[i], NULL, checkBox, (void*)&box_args[i]);
    }

    // 4. Wait for all threads to finish.
    for (int i = 0; i < SIZE; i++) {
        pthread_join(row_threads[i], NULL);
        pthread_join(col_threads[i], NULL);
        pthread_join(box_threads[i], NULL);
    }

    // 5. Print the results.
    printf("Results:\n");
    bool all_rows_passed = true;
    printf("Rows:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!row_check[i]) {
            printf("Row %i did not pass\n", i);
            all_rows_passed = false;
        }
    }
    if (all_rows_passed) {
        printf("All rows passed!\n");
    }

    bool all_cols_passed = true;
    printf("Cols:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!col_check[i]) {
            printf("Col %i did not pass\n", i);
            all_cols_passed = false;
        }
    }
    if (all_cols_passed) {
        printf("All cols passed!\n");
    }

    bool all_boxes_passed = true;
    printf("Boxes:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!box_check[i]) {
            printf("Box %i did not pass\n", i);
            all_boxes_passed = false;
        }
    }
    if (all_boxes_passed) {
        printf("All boxes passed!\n");
    }

    return 0;
}
