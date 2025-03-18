#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 10 // Define the size of the array

// Function to swap elements
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to partition the array
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// QuickSort function
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Merge two sorted subarrays
void merge(int arr[], int temp[], int low, int mid, int high) {
    int i = low, j = mid + 1, k = low;

    while (i <= mid && j <= high) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= high) {
        temp[k++] = arr[j++];
    }

    for (i = low; i <= high; i++) {
        arr[i] = temp[i];
    }
}

// Function to perform parallel Quick Sort using MPI
void parallelQuickSort(int arr[], int low, int high, int rank, int size) {
    if (rank == 0) {
        quickSort(arr, low, high);  // Perform quicksort on the root process
    }

    // Divide and conquer using MPI
    int segment_size = (high - low + 1) / size;
    int start = low + rank * segment_size;
    int end = (rank == size - 1) ? high : (start + segment_size - 1);

    quickSort(arr, start, end);

    // Gather sorted data from all processes
    MPI_Gather(&arr[start], segment_size, MPI_INT, arr, segment_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Root process merges the results
    if (rank == 0) {
        int temp[SIZE];
        int mid = size / 2;
        for (int i = 1; i < size; i++) {
            merge(arr, temp, 0, mid, SIZE - 1);
        }
    }
}

int main(int argc, char *argv[]) {
    int arr[SIZE];
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Generate random data for the array on process 0
    if (rank == 0) {
        for (int i = 0; i < SIZE; i++) {
            arr[i] = rand() % 10;  // Random values
        }
    }

    // Start the parallel quicksort
    parallelQuickSort(arr, 0, SIZE - 1, rank, size);

    // Root process prints the sorted array
    if (rank == 0) {
        printf("Sorted Array: \n");
        for (int i = 0; i < SIZE; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
