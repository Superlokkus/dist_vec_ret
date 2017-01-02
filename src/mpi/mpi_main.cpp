/*! @file mpi_main.cpp
 *
 */

#include <mpi.h>
#include <iostream>
#include <exception>
#include <cstdlib>

#include "mpi_exception.hpp"
#include "mpi_utility.hpp"


[[noreturn]] static void terminate_with_mpi_abort() {
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    //Non-reachable but some got some mistrust towards MPI
    std::abort();
}

[[noreturn]] static void mpi_thow_error_handler(MPI_Comm *, int *, ...) {
    terminate_with_mpi_abort();
}

int main(int argc, char *argv[]) {
    int provided = 0;
    if (MPI_Init_thread(&argc, &argv, MPI_THREAD_SERIALIZED, &provided)
        != MPI_SUCCESS || provided != MPI_THREAD_SERIALIZED) {
        std::cerr << "MPI_Init_thread failed" << std::endl;
        return EXIT_FAILURE;
    }

    std::set_terminate(&terminate_with_mpi_abort);

    MPI_Errhandler handler = {0};
    MPI_CALL_AND_CHECK(MPI_Comm_create_errhandler(&mpi_thow_error_handler, &handler));
    MPI_CALL_AND_CHECK(MPI_Comm_set_errhandler(MPI_COMM_WORLD, handler));

    try {
        int my_rank = 0, process_count = 0;
        MPI_CALL_AND_CHECK(MPI_Comm_rank(MPI_COMM_WORLD, &my_rank));
        MPI_CALL_AND_CHECK(MPI_Comm_size(MPI_COMM_WORLD, &process_count));
        if (my_rank == 0) {
            organize_serving_nodes(process_count);
            mpi_query_cli_node_main();
        } else {
            mpi_serving_node_main();
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }

    MPI_CALL_AND_CHECK(MPI_Finalize());

    return 0;
}

/*MPI_THREAD_SINGLE Only one thread will execute.
MPI_THREAD_FUNNELED The process may be multi-threaded, but the application must ensure that only the main thread makes MPI calls (for the definition of main thread, see MPI_IS_THREAD_MAIN on page 490).
MPI_THREAD_SERIALIZED The process may be multi-threaded, and multiple threads may make MPI calls, but only one at a time: MPI calls are not made concurrently from two distinct threads (all MPI calls are “serialized”).
MPI_THREAD_MULTIPLE Multiple threads may call MPI, with no restrictions.
 */
