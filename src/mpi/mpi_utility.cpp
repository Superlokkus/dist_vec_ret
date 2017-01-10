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

    std::vector<std::string> file_paths;
    for (const auto &entry : fileapi::recursive_directory_iterator(path)) {
        if (!fileapi::is_regular_file(entry)) {
            continue;
        }
        file_paths.push_back(entry.path().generic_string());
    }

    //To lazy to use div for this one thing
    const unsigned long long per_node = file_paths.size() / (count_processes - 1);
    const unsigned long long remainder = file_paths.size() % (count_processes - 1);
    std::vector<unsigned long long> count_to_rank(count_processes, 0);
    std::fill(count_to_rank.begin() + 1, count_to_rank.end(), per_node);
    std::fill_n(count_to_rank.begin() + 1, remainder, per_node + 1);

    unsigned long long my_count = 0;
    MPI_CALL_AND_CHECK(MPI_Scatter(count_to_rank.data(), 1, MPI_UNSIGNED_LONG_LONG, &my_count,
                                   1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD));

    for (unsigned long long i = 0; i < file_paths.size(); ++i) {
        MPI_CALL_AND_CHECK(MPI_Send(file_paths[i].c_str(), file_paths[i].size() + 1,
                                    MPI_CHAR, (i % (count_processes - 1)) + 1, 0, MPI_COMM_WORLD));
    }
}

void mpi_query_cli_node_main() {
    std::cout << "Querying" << std::endl;
    std::string query;
    std::cin >> query;
}

void mpi_serving_node_main() {
    unsigned long long my_count = 0;
    MPI_CALL_AND_CHECK(
            MPI_Scatter(NULL, 0, MPI_UNSIGNED_LONG_LONG, &my_count, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD));
    std::cout << "I got " << my_count << " files to index" << std::endl;
    std::vector<std::string> file_paths;
    for (unsigned long long i = 0; i < my_count; ++i) {
        MPI_Status status = {};
        MPI_CALL_AND_CHECK(MPI_Probe(0, 0, MPI_COMM_WORLD, &status));
        int count = 0;
        MPI_Get_count(&status, MPI_CHAR, &count);
        std::vector<char> cstring(count);
        MPI_CALL_AND_CHECK(MPI_Recv(cstring.data(), count, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status));
        file_paths.emplace_back(cstring.data());
    }
    std::cout << "Got all file paths to index" << std::endl;
}
