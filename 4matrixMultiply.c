#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000  // Matrix size (N x N), you can change this value

void multiply_matrices(int *A, int *B, int *C, int start_row, int end_row, int size) {
    // Matrix multiplication for rows between start_row and end_row
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < size; j++) {
            C[i * size + j] = 0;
            for (int k = 0; k < size; k++) {
                C[i * size + j] += A[i * size + k] * B[k * size + j];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, size, rows_per_process, start_row, end_row;
    int *A, *B, *C, *local_C;

    MPI_Init(&argc, &argv);   // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Get the rank of the process
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Get the total number of processes

    if (N % size != 0) {
        if (rank == 0) {
            printf("Matrix size is not divisible by the number of processes!\n");
        }
        MPI_Finalize();
        return 0;
    }

    rows_per_process = N / size;  // Rows per process

    // Allocate memory for the matrices and the local result matrix
    if (rank == 0) {
        A = (int *)malloc(N * N * sizeof(int));
        B = (int *)malloc(N * N * sizeof(int));
        C = (int *)malloc(N * N * sizeof(int));

        // Initialize matrices A and B with random values
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i * N + j] = rand() % 100;
                B[i * N + j] = rand() % 100;
            }
        }
    }

    // Each process allocates memory for its local result matrix
    local_C = (int *)malloc(rows_per_process * N * sizeof(int));

    // Scatter rows of matrix A to all processes
    MPI_Scatter(A, rows_per_process * N, MPI_INT, local_C, rows_per_process * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast matrix B to all processes
    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Compute the local result for matrix C
    start_row = rank * rows_per_process;
    end_row = (rank + 1) * rows_per_process;

    multiply_matrices(local_C, B, local_C, start_row, end_row, N);

    // Gather the local result matrices from all processes to the root process
    MPI_Gather(local_C, rows_per_process * N, MPI_INT, C, rows_per_process * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Print the result matrix C if rank is 0
    if (rank == 0) {
        printf("Matrix multiplication completed!\n");
        // Optionally print the result
        /*
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", C[i * N + j]);
            }
            printf("\n");
        }
        */

        // Free allocated memory
        free(A);
        free(B);
        free(C);
    }

    // Free local result matrix
    free(local_C);

    MPI_Finalize();  // Finalize MPI
    return 0;
}
