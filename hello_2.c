#include<stdio.h>
#include<mpi.h>
int main(int argc,char **argv){
    MPI_Init(&argc,&argv);
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    printf("hello from %d\n",rank);
    MPI_Finalize();
    return 0;
}
// mpicc hello_2.c -o hello_2
// mpirun --oversubscribe -np 4 ./hello_2