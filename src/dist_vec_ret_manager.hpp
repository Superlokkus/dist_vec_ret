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
#include <map>
#include <boost/uuid/uuid.hpp>

#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <boost/filesystem.hpp>
#include <boost/serialization/string.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>

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

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & id;
                ar & common_name;
            }

        protected:
            static boost::uuids::uuid get_namespace_uuid();
        };


        using result_t = std::vector<std::pair<distance_t, document_meta_t>>;
        using simple_result = std::vector<std::pair<distance_t, std::string>>;

        void add_document(document_meta_t meta_data, boost::filesystem::path path_to_file);

        bool remove_document(const decltype(document_meta_t::id) &id) noexcept;

        /*!
         * @param query
         * @return The documents with distance
         */
        result_t find_match_for(const std::string &query);

        simple_result find_match_for_simple(const std::string &query);

    private:
        std::shared_ptr<global_weight_state_t> global_state_;
        std::map<boost::uuids::uuid, std::shared_ptr<weight_index_t>> weights_;
        std::map<boost::uuids::uuid, document_meta_t> meta_data_;
    };

}


#endif //DIST_VEC_RET_DIST_VEC_RET_MANAGER_HPP
