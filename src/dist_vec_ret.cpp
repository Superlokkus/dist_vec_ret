/*! @file dist_vec_ret.cpp
 *
 */

#include "dist_vec_ret.hpp"

#include <sstream>
#include <stdexcept>

namespace information_retrieval{

    indexer::indexer(std::string query) {
        this->input_stream_ = std::make_unique<std::stringstream>(query);
    }


    void indexer::update_index() {
        if (isindexed())
            return;

        index_t new_index;
        for(std::string word; *input_stream_ >> word;){
            //TODO Stemming
            new_index[word] += 1;
        }

        this->last_generated_index_ = new_index;
        this->isindexed_ = true;
    }

    index_t indexer::get_index() const {
        if (!this->isindexed_)
            throw std::logic_error("Call update_index at least once before trying to get the index");
        return this->last_generated_index_;
    }

}
