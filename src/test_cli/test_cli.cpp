/*! @file test_cli.cpp
 *
 */

#include <iostream>

#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <boost/filesystem.hpp>

namespace fileapi = boost::filesystem;

#include <dist_vec_ret.hpp>


int main(int argc, char *argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage " << argv[0] << " <Path_to_index>" << std::endl;
            return EXIT_FAILURE;
        }
        if (!fileapi::exists(argv[1])) {
            std::cerr << "Path " << argv[1] << " doesn't exist" << std::endl;
            return EXIT_FAILURE;
        }
        fileapi::path path_to_index{argv[1]};
        for (const auto &entry : path_to_index) {
            std::cout << entry.filename() << "\n";
        }

    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}