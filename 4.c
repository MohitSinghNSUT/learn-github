#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4  

int main(int argc, char *argv[]) {
    int rank, size, i, j, k;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int rows_per_proc = N / size;
    
    // Allocate memory for matrices.
    int *A = NULL;
    int *B = malloc(N * N * sizeof(int));
    int *C = NULL;
    
    
    int *subA = malloc(rows_per_proc * N * sizeof(int));
    int *subC = malloc(rows_per_proc * N * sizeof(int));
    
    if (rank == 0) {
        A = malloc(N * N * sizeof(int));
        C = malloc(N * N * sizeof(int));
       
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                A[i * N + j] = i + j;
                B[i * N + j] = i - j;
            }
        }
    }
    
    // Scatter rows of A to all processes.
    MPI_Scatter(A, rows_per_proc * N, MPI_INT, subA, rows_per_proc * N, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Broadcast matrix B to all processes.
    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);
    
    
    for (i = 0; i < rows_per_proc; i++) {
        for (j = 0; j < N; j++) {
            subC[i * N + j] = 0;
            for (k = 0; k < N; k++) {
                subC[i * N + j] += subA[i * N + k] * B[k * N + j];
            }
        }
    }
    
    
    MPI_Gather(subC, rows_per_proc * N, MPI_INT, C, rows_per_proc * N, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        printf("Result matrix C:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%d ", C[i * N + j]);
            }
            printf("\n");
        }
        free(A);
        free(C);
    }
    
    free(B);
    free(subA);
    free(subC);
    
    MPI_Finalize();
    return 0;
}