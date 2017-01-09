/*! @file mpi_async_dispatcher.hpp
 *
 */

#ifndef DIST_VEC_RET_MPI_ASYNC_DISPATCHER_HPP
#define DIST_VEC_RET_MPI_ASYNC_DISPATCHER_HPP

#include "mpi_async.hpp"
#include <functional>
#include <memory>

namespace mpi {
    class mpi_async_dispatcher {
    public:
        mpi_async_dispatcher(bool auto_start = true);

        ~mpi_async_dispatcher();

        using request_t = std::unique_ptr<mpi_async_request>;
        using callback_t = std::function<void()>;

        void add_request(request_t &&request,
                         const callback_t &callback);

        void add_request(request_t &&request, callback_t &&callback = std::function<void()>{});

        void start();

        void stop() noexcept;

        bool running() const;

    private:
        struct impl;
        std::unique_ptr<impl> impl_;
    };
}

#endif //DIST_VEC_RET_MPI_ASYNC_DISPATCHER_HPP
