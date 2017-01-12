/*! @file mpi_global_weight_state.cpp
 *
 */

#include "mpi_global_weight_state.hpp"

#include <iostream>

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

information_retrieval::mpi_global_weight_state_t::mpi_global_weight_state_t(boost::mpi::communicator com) :
        global_weight_state_t{},
        dispatch_run_{true},
        nodes_com_{std::move(com)} {
    std::cout << "Com size is " << nodes_com_.size() << std::endl;
    this->dispatcher_ = std::thread{&mpi_global_weight_state_t::internal_concurrent_dispatcher, this};
}

void information_retrieval::mpi_global_weight_state_t::internal_concurrent_dispatcher() {
    while (this->dispatch_run_) {

    }
}

information_retrieval::mpi_global_weight_state_t::~mpi_global_weight_state_t() {
    this->dispatch_run_ = false;
    if (this->dispatcher_.joinable()) {
        dispatcher_.join();
    }
}
