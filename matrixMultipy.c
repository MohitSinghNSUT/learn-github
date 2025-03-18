#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 2000

void multiply_matrices(int **A, int **B, int **C) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int **A = (int **)malloc(N * sizeof(int *));
    int **B = (int **)malloc(N * sizeof(int *));
    int **C = (int **)malloc(N * sizeof(int *));
    
    for (int i = 0; i < N; i++) {
        A[i] = (int *)malloc(N * sizeof(int));
        B[i] = (int *)malloc(N * sizeof(int));
        C[i] = (int *)malloc(N * sizeof(int));
    }
    

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 100; 
            B[i][j] = rand() % 100;
        }
    }

    clock_t start, end;
    double cpu_time_used;

    start = clock(); 

    multiply_matrices(A, B, C);

    end = clock();  

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("Matrix multiplication completed.\n");
    printf("Time taken: %f seconds\n", cpu_time_used);
    
    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
