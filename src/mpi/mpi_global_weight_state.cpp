/*! @file mpi_global_weight_state.cpp
 *
 */

#include "mpi_global_weight_state.hpp"

information_retrieval::global_weight_state_t::count_t
information_retrieval::mpi_global_weight_state_t::remote_get_total_document_count() const {
    return global_weight_state_t::remote_get_total_document_count();
}

information_retrieval::global_weight_state_t::count_t
information_retrieval::mpi_global_weight_state_t::remote_get_document_count_with(
        const information_retrieval::string_t &word) const {
    return global_weight_state_t::remote_get_document_count_with(word);
}

information_retrieval::global_weight_state_t::count_t
information_retrieval::mpi_global_weight_state_t::remote_get_document_count_with(
        const std::set<information_retrieval::string_t> &word) const {
    return global_weight_state_t::remote_get_document_count_with(word);
}

void information_retrieval::mpi_global_weight_state_t::remote_update_document(
        const information_retrieval::global_weight_state_t::document_id_t &document_id,
        const information_retrieval::count_index_t &count_index) {
    global_weight_state_t::remote_update_document(document_id, count_index);
}

void information_retrieval::mpi_global_weight_state_t::remote_remove_document(
        const information_retrieval::global_weight_state_t::document_id_t &document_id) {
    global_weight_state_t::remote_remove_document(document_id);
}
