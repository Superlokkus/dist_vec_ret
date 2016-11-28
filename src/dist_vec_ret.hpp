/*! @file dist_vec_ret.hpp
 *
 */

#ifndef INFORMATION_RETRIEVAL_DIST_VEC_RET_HPP
#define INFORMATION_RETRIEVAL_DIST_VEC_RET_HPP

#include <string>
#include <memory>


namespace information_retrieval{

class indexer {
public:
    indexer() = default;
    indexer(std::string query);
    indexer(std::unique_ptr<std::istream> &&input_stream) : input_stream_(std::move(input_stream)){

    }

    bool isindexed() const noexcept {
        return this->isindexed_;
    }

    void index();

private:
    bool isindexed_ = false;

    std::unique_ptr<std::istream> input_stream_;
};

}

#endif
