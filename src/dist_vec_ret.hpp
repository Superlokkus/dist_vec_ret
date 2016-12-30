/*! @file dist_vec_ret.hpp
 *
 */

#ifndef INFORMATION_RETRIEVAL_DIST_VEC_RET_HPP
#define INFORMATION_RETRIEVAL_DIST_VEC_RET_HPP

#include <string>
#include <memory>
#include <map>
#include <set>
#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <istream>
#include <boost/uuid/uuid.hpp>

namespace information_retrieval {

    using string_t = std::string;
    using count_index_t = std::map<string_t, uint_fast32_t>;
    using weight_index_t = std::map<string_t, double>;

    /*! Class for calculating a stemmed word count vector, heavy lifting is done in @ref update_index
     *
     */
    class word_counter {
    public:
        explicit word_counter(std::string query);

        explicit word_counter(std::shared_ptr<std::istream> input_stream) : input_stream_(std::move(input_stream)) {}

        /*!
         * @returns True if @ref update_index has been called at least once, otherweise false
         */
        bool isindexed() const noexcept {
            return this->isindexed_;
        }

        void update_index();

        /*! @throws std::logic_error If @isindexed retunrs false
         *
         * @return The word count index
         */
        std::shared_ptr<count_index_t> get_index() const;

        static void stemming(std::string &word);
    private:
        bool isindexed_ = false;

        std::shared_ptr<std::istream> input_stream_;
        std::shared_ptr<count_index_t> last_generated_index_;
    };

    /*! This class provides all data needed for global weighting
     */
    class global_weight_state_t {
    public:
        using count_t = uint_fast64_t;
        using document_id_t = boost::uuids::uuid;

        explicit global_weight_state_t() = default;

        count_t get_total_document_count() const;
        count_t get_document_count_with(const string_t &word) const;
        count_t get_document_count_with(const std::set<string_t> &word) const;

        void update_document(const document_id_t &document_id, const count_index_t &count_index);
        void remove_document(const document_id_t &document_id);

    private:
        /* Interesting profiling question: One could get rid of _per_word_ but would raise complexity of get_document_count_with from
        O(log(n)) to O(n * m), where n = number of words, and m = number of documents, but memory footprint vs time,
         and removing would be really heavy
         */
        std::map<count_index_t::key_type, count_t> document_count_per_word_;
        std::map<document_id_t, std::set<count_index_t::key_type>> words_per_document_;

    };

    /*! This class takes all data for weighting a given word vector. The heavy lifting aka weighting is then done in
     * @ref local_weighting and @ref global_weighting which can be called independently but must be called
     * before @ref get_weight
     */
    class weighter {
    public:
        explicit weighter(std::shared_ptr<global_weight_state_t> global_weight_state,
                          std::shared_ptr<count_index_t> count_index) :
                global_weight_state_(global_weight_state), count_index_(count_index) {
        }

        void local_weighting();
        void local_weighting(weight_index_t &for_debug_purposes) {
            local_weighting();
            for_debug_purposes = *local_weights_;
        }

        void global_weighting();
        void global_weighting(weight_index_t &for_debug_purposes) {
            global_weighting();
            for_debug_purposes = *global_weights_;
        }

        bool ischached() const {
            return final_weight_.operator bool();
        }

        /*! @brief Returns the final weight vector and calculates it if not already done
         *
         * @returns The final weight vector
         */
        std::shared_ptr<weight_index_t> get_weight();

    private:
        std::shared_ptr<global_weight_state_t> global_weight_state_;
        std::shared_ptr<count_index_t> count_index_;
        std::shared_ptr<weight_index_t> final_weight_;

        std::unique_ptr<weight_index_t> local_weights_;
        std::unique_ptr<weight_index_t> global_weights_;
    };

    using distance_t = double;

    inline distance_t calc_distance(const weight_index_t &document1, const weight_index_t &document2) {
        std::vector<weight_index_t::value_type> common_weights; ///@todo idea: maybe constexpr func, by using std::array<y,document1.size()>
        std::set_union(document1.cbegin(), document1.cend(),
                       document2.cbegin(), document2.cend(),
                       std::back_inserter(common_weights));

        distance_t scalar_product = 0, norm_sum1 = 0, norm_sum2 = 0;

        for (const auto &word: common_weights) {
            const auto w1 = document1.count(word.first) ? document1.at(word.first) : 0.0;
            const auto w2 = document2.count(word.first) ? document2.at(word.first) : 0.0;
            scalar_product += w1 * w2;
            norm_sum1 += std::pow(w1, 2);
            norm_sum2 += std::pow(w2, 2);
        }
        return (norm_sum1 > 0 && norm_sum2 > 0) ? scalar_product / (std::sqrt(norm_sum1) * std::sqrt(norm_sum2)) : 0;
    }

}

#endif
