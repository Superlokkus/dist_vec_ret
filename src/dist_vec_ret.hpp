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

    class indexer {
    public:
        indexer() = default;

        indexer(std::string query);

        indexer(std::unique_ptr<std::istream> &&input_stream) : input_stream_(std::move(input_stream)) {

        }

        bool isindexed() const noexcept {
            return this->isindexed_;
        }

        void update_index();
        index_t get_index() const;

    private:
        bool isindexed_ = false;

        std::unique_ptr<std::istream> input_stream_;
        index_t last_generated_index_;
    };

}

#endif
