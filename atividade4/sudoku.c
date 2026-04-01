#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define N 9

volatile int found = 0;

void print(int arr[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%d ", arr[i][j]);
        printf("\n");
    }
}

int isSafe(int grid[N][N], int row, int col, int num) {
    for (int x = 0; x < N; x++)
        if (grid[row][x] == num)
            return 0;

    for (int x = 0; x < N; x++)
        if (grid[x][col] == num)
            return 0;

    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j + startCol] == num)
                return 0;

    return 1;
}

int solveSudokuSeq(int grid[N][N], int row, int col) {
    if (found) return 0;

    if (row == N - 1 && col == N)
        return 1;

    if (col == N) { row++; col = 0; }

    if (grid[row][col] > 0)
        return solveSudokuSeq(grid, row, col + 1);

    for (int num = 1; num <= N; num++) {
        if (found) return 0;
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num;
            if (solveSudokuSeq(grid, row, col + 1))
                return 1;
            grid[row][col] = 0;
        }
    }
    return 0;
}

int solution[N][N];

void solveSudokuParallel(int grid[N][N], int row, int col) {

    while (row < N && grid[row][col] > 0) {
        col++;
        if (col == N) { col = 0; row++; }
    }

    if (row == N) {
        #pragma omp critical
        {
            if (!found) {
                found = 1;
                memcpy(solution, grid, sizeof(solution));
            }
        }
        return;
    }

    for (int num = 1; num <= N; num++) {
        if (found) break;

        if (isSafe(grid, row, col, num)) {

            int localGrid[N][N];
            memcpy(localGrid, grid, sizeof(localGrid));
            localGrid[row][col] = num;

            int nextCol = col + 1;
            int nextRow = row;
            if (nextCol == N) { nextCol = 0; nextRow++; }

            #pragma omp task firstprivate(localGrid, nextRow, nextCol)
            {
                if (!found) {
                    if (solveSudokuSeq(localGrid, nextRow, nextCol)) {
                        #pragma omp critical
                        {
                            if (!found) {
                                found = 1;
                                memcpy(solution, localGrid, sizeof(solution));
                            }
                        }
                    }
                }
            }
        }
    }

    #pragma omp taskwait
}

int main() {
    int grid[N][N] = {
        { 5, 3, 0, 0, 7, 0, 0, 0, 0 },
        { 6, 0, 0, 1, 9, 5, 0, 0, 0 },
        { 0, 9, 8, 0, 0, 0, 0, 6, 0 },
        { 8, 0, 0, 0, 6, 0, 0, 0, 3 },
        { 4, 0, 0, 8, 0, 3, 0, 0, 1 },
        { 7, 0, 0, 0, 2, 0, 0, 0, 6 },
        { 0, 6, 0, 0, 0, 0, 2, 8, 0 },
        { 0, 0, 0, 4, 1, 9, 0, 0, 5 },
        { 0, 0, 0, 0, 8, 0, 0, 7, 9 }
    };

    #pragma omp parallel
    {
        #pragma omp single
        {
            solveSudokuParallel(grid, 0, 0);
        }
    }

    if (found)
        print(solution);
    else
        printf("No solution exists\n");

    return 0;
}