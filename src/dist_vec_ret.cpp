/*! @file dist_vec_ret.cpp
 *
 */

#include "dist_vec_ret.hpp"

#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <numeric>

namespace information_retrieval {

    word_counter::word_counter(std::string query) {
        this->input_stream_ = std::make_unique<std::stringstream>(query);
    }


    void word_counter::update_index() {
        if (isindexed())
            return;

        auto new_index = std::make_shared<count_index_t>();
        for (std::string word; *input_stream_ >> word;) {
            stemming(word);
            (*new_index)[word] += 1;
        }

        this->last_generated_index_ = new_index;
        this->isindexed_ = true;
    }

    std::shared_ptr<count_index_t> word_counter::get_index() const {
        if (!this->isindexed_)
            throw std::logic_error("Call update_index at least once before trying to get the index");
        return this->last_generated_index_;
    }


    void word_counter::stemming(std::string &word) {
        std::for_each(word.begin(), word.end(), [](char &c) { c = std::tolower(c); });
    }

    count_index_t weighter::get_weight() const {
        count_index_t final_weights;
        const auto local_global_correlation = [](const count_index_t::mapped_type &local,
                                                 const count_index_t::mapped_type &global) ->
                count_index_t::mapped_type {
            return local * global;
        };

        std::transform(this->local_weigths_.cbegin(), this->local_weigths_.cend(), this->global_weights_.cbegin(),
                       std::inserter(final_weights, final_weights.end()), [&local_global_correlation]
                               (const count_index_t::value_type &local, const count_index_t::value_type &global) {
                    return std::make_pair(local.first, local_global_correlation(local.second, global.second));
                });
        return information_retrieval::count_index_t();
    }

    void weighter::local_weighting() {
        const uint_fast64_t word_count = std::accumulate(count_index_->cbegin(), count_index_->cend(), 0,
                                                         [](const uint_fast64_t &prev,
                                                            const count_index_t::value_type &next) {
                                                             return prev + next.second;
                                                         });
        weigth_index_t local_weigths;
        std::transform(count_index_->cbegin(), count_index_->cend(),
                       std::inserter(local_weigths, local_weigths.end()),
                       [&word_count](const weigth_index_t::value_type &word) {
                           return std::make_pair(word.first, word.second / word_count);
                       });
        this->local_weigths_ = local_weigths;
    }

    void weighter::global_weighting() {

    }

}
