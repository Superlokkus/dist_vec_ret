/*! @file mpi_async_send.hpp
 *
 */

#ifndef DIST_VEC_RET_MPI_ASYNC_SEND_HPP
#define DIST_VEC_RET_MPI_ASYNC_SEND_HPP

#include <mpi.h>
#include <vector>

namespace mpi {

    enum struct modes {
        standard,
        buffered,
        synchronous,
        ready,
    };

    template<typename T, typename Container = std::vector<T>, modes Mode = modes::standard>
    class mpi_async_send {
    public:
        mpi_async_send(Container &&to_send) : data_(to_send) {}

        mpi_async_send(const mpi_async_send &) = delete;

        mpi_async_send &operator=(const mpi_async_send &) = delete;


    private:
        Container data_;
    };

    class mpi_async_recv {

    };

}


#endif //DIST_VEC_RET_MPI_ASYNC_SEND_HPP
