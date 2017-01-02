/*! @file mpi_main.cpp
 *
 */

#include <mpi.h>
#include <iostream>
#include <exception>
#include <cstdlib>

#include "mpi_exception.hpp"


[[noreturn]] static void terminate_with_mpi_abort() {
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    abort();
}

int main(int argc, char *argv[]) {
    if (MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, NULL) != MPI_SUCCESS)
        return EXIT_FAILURE;

    std::set_terminate(&terminate_with_mpi_abort);


    MPI_CALL_AND_CHECK(MPI_Finalize());

    return 0;
}

/*MPI_THREAD_SINGLE Only one thread will execute.
MPI_THREAD_FUNNELED The process may be multi-threaded, but the application must ensure that only the main thread makes MPI calls (for the definition of main thread, see MPI_IS_THREAD_MAIN on page 490).
MPI_THREAD_SERIALIZED The process may be multi-threaded, and multiple threads may make MPI calls, but only one at a time: MPI calls are not made concurrently from two distinct threads (all MPI calls are “serialized”).
MPI_THREAD_MULTIPLE Multiple threads may call MPI, with no restrictions.
 */
