from mpi4py import MPI

# Get the communicator (a group of processes that can communicate)
comm = MPI.COMM_WORLD

# Get the rank (ID) of the current process in the communicator
rank = comm.Get_rank()

# Get the total number of processes in the communicator
size = comm.Get_size()

# Print a message from each process
print(f"Hello World from process {rank} of {size}")