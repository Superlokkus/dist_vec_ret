/*! @file mpi_utility.cpp
 *
 */

#include "mpi_utility.hpp"

#include <iostream>

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

#include "mpi_exception.hpp"
#include <algorithm>
#include <time_utility.hpp>

#include <dist_vec_ret_manager.hpp>
#include <boost/mpi.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/utility.hpp>
#include "mpi_global_weight_state.hpp"


void organize_serving_nodes(const int count_processes, const char *path) {
    namespace fileapi = boost::filesystem;
    if (!fileapi::exists(path)) {
        throw std::runtime_error(std::string{"Path "} + path + " doesn't exist");
    }
    std::cout << "Scanning directory" << std::endl;
    auto indexing_timer = information_retrieval::time_utility{"Indexing"};
    std::vector<std::string> file_paths;
    for (const auto &entry : fileapi::recursive_directory_iterator(path)) {
        if (!fileapi::is_regular_file(entry)) {
            continue;
        }
        file_paths.push_back(entry.path().generic_string());
    }
    indexing_timer.checkpoint("Scanning directory recursively");

    //To lazy to use div for this one thing
    const unsigned long long per_node = file_paths.size() / (count_processes - 1);
    const unsigned long long remainder = file_paths.size() % (count_processes - 1);
    std::vector<unsigned long long> count_to_rank(count_processes, 0);
    std::fill(count_to_rank.begin() + 1, count_to_rank.end(), per_node);
    std::fill_n(count_to_rank.begin() + 1, remainder, per_node + 1);
    indexing_timer.checkpoint("Calculating distribution of files on nodes");
    unsigned long long my_count = 0;
    MPI_CALL_AND_CHECK(MPI_Scatter(count_to_rank.data(), 1, MPI_UNSIGNED_LONG_LONG, &my_count,
                                   1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD));
    indexing_timer.checkpoint("Broadcasting file counter to nodes");
    for (unsigned long long i = 0; i < file_paths.size(); ++i) {
        MPI_CALL_AND_CHECK(MPI_Send(file_paths[i].c_str(), file_paths[i].size() + 1,
                                    MPI_CHAR, (i % (count_processes - 1)) + 1, 0, MPI_COMM_WORLD));
    }
    indexing_timer.checkpoint("Sending file paths to every node accodingly");
    std::cout << "Wating for all nodes to finish indexing..." << std::endl;
    MPI_CALL_AND_CHECK(MPI_Barrier(MPI_COMM_WORLD));
    indexing_timer.checkpoint("Waiting on all nodes to finish indexing");
    indexing_timer.stop();
    std::cout << indexing_timer << "\n";
    std::cout << "Looks like everyone is ready" << std::endl;
}

information_retrieval::dist_vec_ret_manager::simple_result get_unsorted_results(std::string &query) {
    boost::mpi::communicator world{};
    boost::mpi::broadcast(world, query, 0);
    std::vector<information_retrieval::dist_vec_ret_manager::simple_result> result_vec{};
    information_retrieval::dist_vec_ret_manager::simple_result results{};
    boost::mpi::gather(world, results, result_vec, 0);
    for (auto &result_from_node : result_vec) {
        std::move(result_from_node.begin(), result_from_node.end(), std::back_inserter(results));
    }

    return results;
}

void mpi_query_cli_node_main() {
    std::cout << "Query: " << std::endl;
    for (std::string query; std::getline(std::cin, query); std::cout << "\nQuery: ") {
        auto query_timer = information_retrieval::time_utility{"Query"};
        auto results = get_unsorted_results(query);
        query_timer.checkpoint("Getting results");
        const auto sort_crit = [](const decltype(results)::value_type &l, const decltype(results)::value_type &r)
                -> bool { return std::get<0>(l) < std::get<0>(r); };
        std::sort(results.begin(), results.end(), sort_crit);
        query_timer.checkpoint("Sorting results");
        query_timer.stop();
        for (const auto &doc : results) {
            std::cout << std::get<1>(doc) << ": " << std::get<0>(doc) << "\n";
        }
        std::cout << query_timer << std::endl;
    }
}

std::unique_ptr<information_retrieval::dist_vec_ret_manager> indexing() {
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

    auto global_state = std::make_shared<information_retrieval::mpi_global_weight_state_t>();
    auto manager =
            std::unique_ptr<information_retrieval::dist_vec_ret_manager>
                    {new information_retrieval::dist_vec_ret_manager(global_state)};
    for (const auto &file : file_paths) {
        auto path = boost::filesystem::path{file};
        manager->add_document(path, path);
    }
    //Signal finshed indexing
    MPI_CALL_AND_CHECK(MPI_Barrier(MPI_COMM_WORLD));
    return manager;
}

void mpi_serving_node_main() {
    auto manager = indexing();
    auto world = boost::mpi::communicator{};
    while (true) {
        std::string query;
        boost::mpi::broadcast(world, query, 0);
        auto my_results = manager->find_match_for_simple(query);
        boost::mpi::gather(world, my_results, 0);
    }
}
