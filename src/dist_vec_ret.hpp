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

        std::shared_ptr<index_t> get_index() const;

        static void stemming(std::string &word);

    private:
        bool isindexed_ = false;

        std::unique_ptr<std::istream> input_stream_;
        std::shared_ptr<index_t> last_generated_index_;
    };

    using global_weight_t = index_t;
    class weighter {
    public:
        explicit weighter(global_weight_t &global_weight, std::shared_ptr<index_t> count_index) :
                global_weight_(global_weight), count_index_(count_index) {
        }

        void local_weighting();

        void local_weighting(index_t &for_debug_purposes) {
            local_weighting();
            for_debug_purposes = local_weigths_;
        }

        void global_weighting();

        index_t get_weight() const;

    private:
        global_weight_t &global_weight_;
        std::shared_ptr<index_t> count_index_;

        index_t local_weigths_;
        index_t global_weights_;

    };

}

#endif
