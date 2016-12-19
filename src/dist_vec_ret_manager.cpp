/*! @file dist_vec_ret_manager.cpp
 *
 */


#include "dist_vec_ret_manager.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/random_generator.hpp>

information_retrieval::dist_vec_ret_manager::dist_vec_ret_manager(
        std::shared_ptr<information_retrieval::global_weight_state_t> global_state) :
        global_state_(global_state) {

}

void information_retrieval::dist_vec_ret_manager::add_document(
        information_retrieval::dist_vec_ret_manager::document_meta_t meta_data, boost::filesystem::path path_to_file) {
    word_counter count{std::make_shared<boost::filesystem::ifstream>(path_to_file)};
    //TODO profiling via std::chrono?!
    count.update_index();
    this->global_state_->update_document(meta_data.id, *count.get_index());

}

std::vector<std::tuple<information_retrieval::distance_t, information_retrieval::dist_vec_ret_manager::document_meta_t>>
information_retrieval::dist_vec_ret_manager::find_match_for(std::string query) {
    return std::vector<std::tuple<information_retrieval::distance_t, information_retrieval::dist_vec_ret_manager::document_meta_t>>();
}

information_retrieval::dist_vec_ret_manager::document_meta_t::document_meta_t() {
    this->id = boost::uuids::nil_uuid();
}

information_retrieval::dist_vec_ret_manager::document_meta_t::document_meta_t(boost::filesystem::path path_to_file) :
        common_name{path_to_file.filename().string()} {
    this->id = boost::uuids::name_generator(get_namespace_uuid())(path_to_file.string());
}

boost::uuids::uuid information_retrieval::dist_vec_ret_manager::document_meta_t::get_namespace_uuid() {
    static boost::uuids::uuid id = boost::uuids::random_generator()();
    return id;
}

