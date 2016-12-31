/*! @file mpi_main.cpp
 *
 */

#include <mpi.h>
#include <iostream>
#include <exception>
#include <cstdlib>


[[noreturn]] static void terminate_with_mpi_abort() {
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    abort();
}

int main(int argc, char *argv[])
{
    int supported = 0;
    if (MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &supported) != MPI_SUCCESS)//MPI_THREAD_SINGLE
        return EXIT_FAILURE;
    std::cout << "Mutiple is " << MPI_THREAD_MULTIPLE << " supported is " << supported << std::endl;

    MPI_Finalize();

    return 0;
}