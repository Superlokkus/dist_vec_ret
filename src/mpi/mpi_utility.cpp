/*! @file mpi_utility.cpp
 *
 */

#include "mpi_utility.hpp"

#include <iostream>


void organize_serving_nodes(const int count_processes) {
    std::cout << "Organizing" << std::endl;
}

void mpi_query_cli_node_main() {
    std::cout << "Querying" << std::endl;
    std::string query;
    std::cin >> query;
}

void mpi_serving_node_main() {
    std::cout << "Serving" << std::endl;
}
