/*! @file mpi_utility.cpp
 *
 */

#include "mpi_utility.hpp"

#include <iostream>

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

#include "mpi_exception.hpp"
#include <algorithm>


void organize_serving_nodes(const int count_processes, const char *path) {
    namespace fileapi = boost::filesystem;
    if (!fileapi::exists(path)) {
        throw std::runtime_error(std::string{"Path "} + path + " doesn't exist");
    }

    std::vector<char> file_paths;
    std::vector<int> displacements;
    for (const auto &entry : fileapi::recursive_directory_iterator(path)) {
        if (!fileapi::is_regular_file(entry)) {
            continue;
        }
        const auto string = entry.path().generic_string();
        const auto to_copy = string.size() + 1;
        file_paths.insert(file_paths.end(), to_copy, '\0');
        memcpy(file_paths.data() + file_paths.size() - to_copy, string.data(), to_copy);
        displacements.push_back(to_copy);
    }

    //To lazy to use div for this one thing
    const int per_node = file_paths.size() / (count_processes - 1);
    const int remainder = file_paths.size() % (count_processes - 1);
    std::vector<int> count_to_rank(count_processes, 0);
    std::fill(count_to_rank.begin() + 1, count_to_rank.end(), per_node);
    std::fill_n(count_to_rank.begin() + 1, remainder, per_node + 1);

    int my_count = 0;
    MPI_CALL_AND_CHECK(MPI_Scatter(count_to_rank.data(), 1, MPI_INT, &my_count, 1, MPI_INT, 0, MPI_COMM_WORLD));

}

void mpi_query_cli_node_main() {
    std::cout << "Querying" << std::endl;
    std::string query;
    std::cin >> query;
}

void mpi_serving_node_main() {
    int my_count = 0;
    MPI_CALL_AND_CHECK(MPI_Scatter(NULL, 0, MPI_INT, &my_count, 1, MPI_INT, 0, MPI_COMM_WORLD));
    std::cout << "I got " << my_count << " to count" << std::endl;

}
