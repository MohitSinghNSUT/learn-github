#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, size;
    const int N = 100; //  array size.
    int *array = NULL;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int local_n = N / size;
    int *local_array = malloc(local_n * sizeof(int));
    
   
    if (rank == 0) {
        array = malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            array[i] = 1; 
        }
    }
    
    
    MPI_Scatter(array, local_n, MPI_INT, local_array, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    
    
    int local_sum = 0;
    for (int i = 0; i < local_n; i++) {
        local_sum += local_array[i];
    }
    
    int global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        printf("Global sum = %d\n", global_sum);
        free(array);
    }

    printf("-- %d -- %d -- %d\n" , local_sum , rank,size);
    
    free(local_array);
    MPI_Finalize();
    return 0;
}