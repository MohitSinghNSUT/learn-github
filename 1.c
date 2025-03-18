#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 5000

int main() {
    
    double **A = malloc(N * sizeof(double*));
    double **B = malloc(N * sizeof(double*));
    double **C = malloc(N * sizeof(double*));
    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    for (int i = 0; i < N; i++) {
        A[i] = malloc(N * sizeof(double));
        B[i] = malloc(N * sizeof(double));
        C[i] = malloc(N * sizeof(double));
        if (A[i] == NULL || B[i] == NULL || C[i] == NULL) {
            printf("Memory allocation failed\n");
            return 1;
        }
    }
    
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand()%10;
            B[i][j] = rand()%10;
            C[i][j] = 0.0;
        }
    }
    
    
    clock_t start = clock();
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            for (int j = 0; j < N; j++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Matrix multiplication completed in %f seconds (CPU time).\n", cpu_time_used);
    
    
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