/*! @file dist_vec_ret.cpp
 *
 */

#include "dist_vec_ret.hpp"

#include <sstream>
#include <stdexcept>
#include <algorithm>

namespace information_retrieval{

    word_counter::word_counter(std::string query) {
        this->input_stream_ = std::make_unique<std::stringstream>(query);
    }


    void word_counter::update_index() {
        if (isindexed())
            return;

        index_t new_index;
        size_t new_word_count;
        for(std::string word; *input_stream_ >> word;){
            stemming(word);
            new_index[word] += 1;
            new_word_count += 1;
        }

        this->last_generated_index_ = new_index;
        this->word_count_ = new_word_count;
        this->isindexed_ = true;
    }

    index_t word_counter::get_index() const {
        if (!this->isindexed_)
            throw std::logic_error("Call update_index at least once before trying to get the index");
        return this->last_generated_index_;
    }

    size_t word_counter::get_word_count() const {
        if (!this->isindexed_)
            throw std::logic_error("Call update_index at least once before trying to get the index");
        return this->word_count_;
    }

    void word_counter::stemming(std::string &word) {
        std::for_each(word.begin(),word.end(),[](char &c){c = std::tolower(c);});
    }
}
