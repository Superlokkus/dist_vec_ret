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
    int provided = 0;
    if (MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided) != MPI_SUCCESS)
        return EXIT_FAILURE;

    std::set_terminate(&terminate_with_mpi_abort);



    MPI_Finalize();

    return 0;
}