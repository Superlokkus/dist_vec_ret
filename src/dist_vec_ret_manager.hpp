/*! @file dist_vec_ret_manager.hpp
 *
 */

#ifndef DIST_VEC_RET_DIST_VEC_RET_MANAGER_HPP
#define DIST_VEC_RET_DIST_VEC_RET_MANAGER_HPP

#include <memory>
#include <istream>
#include <string>
#include <vector>
#include <tuple>
#include <boost/uuid/uuid.hpp>

#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <boost/filesystem.hpp>

#include "dist_vec_ret.hpp"

namespace information_retrieval {
    /*!
     *
     */
    class dist_vec_ret_manager {
    public:
        dist_vec_ret_manager() : dist_vec_ret_manager(std::make_shared<global_weight_state_t>()) {}

        dist_vec_ret_manager(std::shared_ptr<global_weight_state_t> global_state);

        std::shared_ptr<global_weight_state_t> get_global_state() {
            return this->global_state_;
        }

        struct document_meta_t {
            std::string common_name;
            boost::uuids::uuid id;

            document_meta_t();

            document_meta_t(boost::filesystem::path path_to_file);

        protected:
            static boost::uuids::uuid get_namespace_uuid();
        };

        void add_document(document_meta_t meta_data, boost::filesystem::path path_to_file);

        /*!
         * @param query
         * @return The documents with distance, sorted
         */
        std::vector<std::tuple<distance_t, document_meta_t>> find_match_for(std::string query);


    private:
        std::shared_ptr<global_weight_state_t> global_state_;
    };

}


#endif //DIST_VEC_RET_DIST_VEC_RET_MANAGER_HPP
