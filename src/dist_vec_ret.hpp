/*! @file dist_vec_ret.hpp
 *
 */

#ifndef INFORMATION_RETRIEVAL_DIST_VEC_RET_HPP
#define INFORMATION_RETRIEVAL_DIST_VEC_RET_HPP

#include <string>
#include <memory>
#include <map>
#include <cstdint>


namespace information_retrieval {

    using index_t = std::map<std::string,uint_fast32_t>;

    class word_counter {
    public:

        explicit word_counter(std::string query);

        explicit word_counter(std::unique_ptr<std::istream> &&input_stream) : input_stream_(std::move(input_stream)) {

        }

        bool isindexed() const noexcept {
            return this->isindexed_;
        }

        void update_index();
        index_t get_index() const;
        size_t get_word_count() const;

        static void stemming(std::string &word);

    private:
        bool isindexed_ = false;

        std::unique_ptr<std::istream> input_stream_;
        index_t last_generated_index_;
        size_t word_count_;
    };

    using global_weight_t = index_t;
    class weighter {
        explicit weighter(const global_weight_t& global_weight_t) {
        }

    };

}

#endif
