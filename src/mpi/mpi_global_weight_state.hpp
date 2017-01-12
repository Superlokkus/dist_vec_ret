/*! @file mpi_global_weight_state.hpp
 *
 */

#ifndef DIST_VEC_RET_MPI_GLOBAL_WEIGHT_STATE_HPP
#define DIST_VEC_RET_MPI_GLOBAL_WEIGHT_STATE_HPP

#include <dist_vec_ret.hpp>
#include <thread>
#include <atomic>

#include <boost/mpi/communicator.hpp>

namespace information_retrieval {
    class mpi_global_weight_state_t : public global_weight_state_t {
    public:
        explicit mpi_global_weight_state_t(boost::mpi::communicator com);
        ~mpi_global_weight_state_t();
    private:
        std::thread dispatcher_;
        std::atomic<bool> dispatch_run_;
        boost::mpi::communicator nodes_com_;
        void internal_concurrent_dispatcher();

        static const int mpi_tag = 42 + 1337;

        virtual count_t remote_get_total_document_count() const override;

        virtual count_t remote_get_document_count_with(const string_t &word) const override;

        virtual count_t remote_get_document_count_with(const std::set<string_t> &word) const override;

        virtual void
        remote_update_document(const document_id_t &document_id, const count_index_t &count_index) override;

        virtual void remote_remove_document(const document_id_t &document_id) override;
    };

}


#endif //DIST_VEC_RET_MPI_GLOBAL_WEIGHT_STATE_HPP
