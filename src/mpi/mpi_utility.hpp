/*! @file mpi_utility.hpp
 *
 */

#ifndef DIST_VEC_RET_MPI_UTILITY_HPP
#define DIST_VEC_RET_MPI_UTILITY_HPP

#include <mpi.h>

void organize_serving_nodes(const int count_processes);

void mpi_query_cli_node_main();

void mpi_serving_node_main();


#endif //DIST_VEC_RET_MPI_UTILITY_HPP
