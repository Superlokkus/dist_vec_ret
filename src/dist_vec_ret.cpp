/*! @file dist_vec_ret.cpp
 *
 */

#include "dist_vec_ret.hpp"

#include <sstream>

namespace information_retrieval{

    indexer::indexer(std::string query) {
        this->input_stream_ = std::make_unique<std::stringstream>(query);
    }


    void indexer::index() {
        if (isindexed())
            return;

        this->isindexed_ = true;
    }

}
