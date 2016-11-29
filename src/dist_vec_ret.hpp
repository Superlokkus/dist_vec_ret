/*! @file dist_vec_ret.hpp
 *
 */

#ifndef INFORMATION_RETRIEVAL_DIST_VEC_RET_HPP
#define INFORMATION_RETRIEVAL_DIST_VEC_RET_HPP

#include <string>
#include <memory>
#include <map>
#include <set>
#include <cstdint>
#include <boost/uuid/uuid.hpp>

namespace information_retrieval {

    using string_t = std::string;
    using count_index_t = std::map<string_t, uint_fast32_t>;
    using weight_index_t = std::map<string_t, double>;

    class word_counter {
    public:

        explicit word_counter(std::string query);

        explicit word_counter(std::unique_ptr<std::istream> &&input_stream) : input_stream_(std::move(input_stream)) {

        }

        bool isindexed() const noexcept {
            return this->isindexed_;
        }

        void update_index();

        std::shared_ptr<count_index_t> get_index() const;

        static void stemming(std::string &word);

    private:
        bool isindexed_ = false;

        std::unique_ptr<std::istream> input_stream_;
        std::shared_ptr<count_index_t> last_generated_index_;
    };

    class global_weight_t {
    public:
        using count_t = uint_fast64_t;
        using document_id_t = boost::uuids::uuid;

        count_t get_total_document_count() const;

        count_t get_document_count_with(string_t word) const;

        count_t get_document_count_with(std::set<string_t> word) const;

        void update_document(const document_id_t &document_id, count_index_t);

        void remove_document(const document_id_t &document_id);
    };

    class weighter {
    public:
        explicit weighter(global_weight_t &global_weight, std::shared_ptr<count_index_t> count_index) :
                global_weight_(global_weight), count_index_(count_index) {
        }

        void local_weighting();

        void local_weighting(weight_index_t &for_debug_purposes) {
            local_weighting();
            for_debug_purposes = local_weights_;
        }

        void global_weighting();

        weight_index_t get_weight() const;

    private:
        global_weight_t &global_weight_;
        std::shared_ptr<count_index_t> count_index_;

        weight_index_t local_weights_;
        weight_index_t global_weights_;

    };

}

#endif
