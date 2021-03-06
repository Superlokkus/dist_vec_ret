/*! @file dist_vec_ret.cpp
 *
 */

#include "dist_vec_ret.hpp"

#include <sstream>
#include <stdexcept>
#include <cmath>
#include <numeric>

namespace information_retrieval {

    word_counter::word_counter(std::string query)
            : input_stream_(
            new std::stringstream(query)) { //no make_unique thanks to damn old gcc4.8.5. on target system
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
        for (auto char_it = word.begin(), end = word.end(); char_it != end; ++char_it) {
            if (ispunct(*char_it)) {
                char_it = word.erase(char_it);
                end = word.end();
                if (char_it == end) {
                    break;
                }
            }
            *char_it = std::tolower(*char_it);
        }
    }

    std::shared_ptr<weight_index_t> weighter::get_weight() {
        if (this->ischached()) {
            return this->final_weight_;
        }
        if (!local_weights_) {
            this->local_weighting();
        }
        if (!global_weights_) {
            this->global_weighting();
        }


        auto final_weights = std::make_shared<weight_index_t>();
        const auto local_global_correlation = [](const weight_index_t::mapped_type &local,
                                                 const weight_index_t::mapped_type &global) ->
                weight_index_t::mapped_type {
            return local * global;
        };

        std::transform(this->local_weights_->cbegin(), this->local_weights_->cend(), this->global_weights_->cbegin(),
                       std::inserter(*final_weights, final_weights->end()), [&local_global_correlation]
                               (const weight_index_t::value_type &local, const weight_index_t::value_type &global) {
                    return std::make_pair(local.first, local_global_correlation(local.second, global.second));
                });

        return this->final_weight_ = std::move(final_weights);
    }

    void weighter::local_weighting() {
        if (local_weights_) {
            return;
        }
        const uint_fast64_t word_count = std::accumulate(count_index_->cbegin(), count_index_->cend(), 0,
                                                         [](const uint_fast64_t &prev,
                                                            const count_index_t::value_type &next) {
                                                             return prev + next.second;
                                                         });
        std::unique_ptr<weight_index_t> local_weigths{new weight_index_t()};
        std::transform(count_index_->cbegin(), count_index_->cend(),
                       std::inserter(*local_weigths, local_weigths->end()),
                       [&word_count](const weight_index_t::value_type &word) {
                           return std::make_pair(word.first, word.second / word_count);
                       });
        this->local_weights_ = std::move(local_weigths);
    }

    void weighter::global_weighting() {
        if (global_weights_) {
            return;
        }

        std::unique_ptr<weight_index_t> global_weights{new weight_index_t()};

        const auto global_weight_func = [this](const weight_index_t::value_type &word) {
            const weight_index_t::mapped_type document_count{static_cast<weight_index_t::mapped_type>
                                                             (global_weight_state_->get_total_document_count())};
            const weight_index_t::mapped_type document_count_with_word{static_cast<weight_index_t::mapped_type>
                                                                       (global_weight_state_->get_document_count_with(
                            word.first))};
            weight_index_t::mapped_type weight = (document_count > 0 && document_count_with_word > 0) ?
                                                 std::log(
                                                         document_count
                                                         /
                                                         document_count_with_word
                                                 )
                                                                                                      : 0;
            return std::make_pair(word.first, std::move(weight));
        };

        std::transform(count_index_->cbegin(), count_index_->cend(),
                       std::inserter(*global_weights, global_weights->end()),
                       global_weight_func);

        this->global_weights_ = std::move(global_weights);
    }


    global_weight_state_t::count_t global_weight_state_t::local_get_total_document_count() const {
        return this->words_per_document_.size();
    }

    global_weight_state_t::count_t global_weight_state_t::local_get_document_count_with(const string_t &word) const {
        return (document_count_per_word_.count(word) != 0) ? this->document_count_per_word_.at(word) : 0;
    }

    global_weight_state_t::count_t
    global_weight_state_t::local_get_document_count_with(const std::set<string_t> &word) const {
        count_t count = 0;
        for (const auto &w : word) {
            const auto document_count_for_this_word = this->document_count_per_word_.find(w);
            count += (document_count_for_this_word != document_count_per_word_.cend())
                     ? document_count_for_this_word->second : 0;
        }
        return count;
    }

    void global_weight_state_t::local_update_document(const global_weight_state_t::document_id_t &document_id,
                                                      const count_index_t &count_index) {
        if (this->words_per_document_.count(document_id) != 0) {
            this->remove_document(document_id);
        }
        std::transform(count_index.cbegin(), count_index.cend(),
                       std::inserter(words_per_document_[document_id], words_per_document_[document_id].end()),
                       [](const count_index_t::value_type &word_count_par) {
                           return word_count_par.first;
                       });

        for (const auto &word : words_per_document_[document_id]) {
            ++document_count_per_word_[word];
        }

    }

    void global_weight_state_t::local_remove_document(const global_weight_state_t::document_id_t &document_id) {
        for (const auto &word : this->words_per_document_.at(document_id)) {
            this->document_count_per_word_.at(word) -= 1;
        }
        this->words_per_document_.erase(document_id);
    }

    global_weight_state_t::count_t global_weight_state_t::get_total_document_count() const {
        return local_get_total_document_count() + remote_get_total_document_count();
    }

    global_weight_state_t::count_t global_weight_state_t::get_document_count_with(const string_t &word) const {
        return local_get_document_count_with(word) + remote_get_document_count_with(word);
    }

    global_weight_state_t::count_t
    global_weight_state_t::get_document_count_with(const std::set<string_t> &word) const {
        return local_get_document_count_with(word) + remote_get_document_count_with(word);
    }

    void global_weight_state_t::update_document(const global_weight_state_t::document_id_t &document_id,
                                                const count_index_t &count_index) {
        this->remote_update_document(document_id, count_index);
        this->local_update_document(document_id, count_index);
    }

    void global_weight_state_t::remove_document(const global_weight_state_t::document_id_t &document_id) {
        this->remote_remove_document(document_id);
        this->local_remove_document(document_id);
    }

    global_weight_state_t::count_t global_weight_state_t::remote_get_total_document_count() const {
        return 0;
    }

    global_weight_state_t::count_t global_weight_state_t::remote_get_document_count_with(const string_t &word) const {
        return 0;
    }

    global_weight_state_t::count_t
    global_weight_state_t::remote_get_document_count_with(const std::set<string_t> &word) const {
        return 0;
    }

    void global_weight_state_t::remote_update_document(const global_weight_state_t::document_id_t &document_id,
                                                       const count_index_t &count_index) {
    }

    void global_weight_state_t::remote_remove_document(const global_weight_state_t::document_id_t &document_id) {
    }

}
