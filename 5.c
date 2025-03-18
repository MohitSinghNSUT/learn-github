#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


int partition(int arr[], int low, int high) {
    int pivot = arr[high];  // choose the last element as pivot
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i+1], &arr[high]);
    return i + 1;
}


void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}


void k_way_merge(int data[], int sorted[], int N, int local_n, int num_procs) {
    // Create an index array for each subarray block.
    int *indices = malloc(num_procs * sizeof(int));
    for (int i = 0; i < num_procs; i++) {
        indices[i] = i * local_n;
    }
    
    // Perform a simple k-way merge.
    for (int i = 0; i < N; i++) {
        int min_val = 0x7fffffff; // a large integer (INT_MAX)
        int min_idx = -1;
        // For each block (from each process) check if the next available element is the smallest.
        for (int j = 0; j < num_procs; j++) {
            int start = j * local_n;
            int end = start + local_n;
            if (indices[j] < end && data[indices[j]] < min_val) {
                min_val = data[indices[j]];
                min_idx = j;
            }
        }
        sorted[i] = min_val;
        indices[min_idx]++;
    }
    free(indices);
}

int main(int argc, char *argv[]) {
    int rank, num_procs;
    const int N = 12;  
    int *data = NULL;  

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    
    if (N % num_procs != 0) {
        if (rank == 0)
            fprintf(stderr, "Error: N (%d) must be divisible by number of processes (%d)\n", N, num_procs);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    int local_n = N / num_procs;


    int *local_data = malloc(local_n * sizeof(int));
    if (local_data == NULL) {
        fprintf(stderr, "Error: unable to allocate local_data\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

   
    if (rank == 0) {
        data = malloc(N * sizeof(int));
        if (data == NULL) {
            fprintf(stderr, "Error: unable to allocate data\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        
        for (int i = 0; i < N; i++) {
            data[i] = rand() % 100;  
        }

        
        printf("Unsorted array:\n");
        for (int i = 0; i < N; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
    }

    
    MPI_Scatter(data, local_n, MPI_INT, local_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    
    quicksort(local_data, 0, local_n - 1);

    
    MPI_Gather(local_data, local_n, MPI_INT, data, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        
        int *sorted = malloc(N * sizeof(int));
        if (sorted == NULL) {
            fprintf(stderr, "Error: unable to allocate sorted array\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        k_way_merge(data, sorted, N, local_n, num_procs);

        
        printf("Sorted array:\n");
        for (int i = 0; i < N; i++) {
            printf("%d ", sorted[i]);
        }
        printf("\n");

        free(sorted);
        free(data);
    }

    free(local_data);
    MPI_Finalize();
    return 0;
}
// mpirun -np 2 ./mpi_quicksort