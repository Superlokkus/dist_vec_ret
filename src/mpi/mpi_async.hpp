/*! @file mpi_async_send.hpp
 *
 */

#ifndef DIST_VEC_RET_MPI_ASYNC_SEND_HPP
#define DIST_VEC_RET_MPI_ASYNC_SEND_HPP

#include <mpi.h>
#include <vector>
#include "mpi_exception.hpp"

namespace mpi {

    template<typename T>
    struct mpi_data_type {
        const static MPI_Datatype value = MPI_BYTE;
    };
    template<>
    struct mpi_data_type<char> {
        const static MPI_Datatype value = MPI_CHAR;
    };
    template<>
    struct mpi_data_type<unsigned long long int> {
        const static MPI_Datatype value = MPI_UNSIGNED_LONG_LONG;
    };
    template<>
    struct mpi_data_type<double> {
        const static MPI_Datatype value = MPI_DOUBLE;
    };
    template<>
    struct mpi_data_type<long double> {
        const static MPI_Datatype value = MPI_LONG_DOUBLE;
    };


    template<typename T, typename Container = std::vector<T>>
    class mpi_async_send {
    public:
        mpi_async_send(const Container &to_send) : data_(to_send) {
        }

        mpi_async_send(Container &&to_send) : data_(std::move(to_send)) {
        }

        mpi_async_send(const mpi_async_send &) = delete;

        mpi_async_send &operator=(const mpi_async_send &) = delete;

        mpi_async_send &operator=(mpi_async_send &&r) noexcept {
            wait();
            data_ = std::move(r.data_);
            request_ = std::move(r.request_);
            r.request_ = MPI_REQUEST_NULL;
            return *this;
        }

        mpi_async_send(mpi_async_send &&r) : data_(std::move(r.data_)), request_(std::move(r.request_)) {
            r.request_ = MPI_REQUEST_NULL;
        }


        void send(int dest, int tag, MPI_Comm com = MPI_COMM_WORLD) {
            send_(dest, tag, com);
        }

        bool test() noexcept {
            int flag = 0;
            MPI_Status status = {};
            MPI_CALL_AND_CHECK(MPI_Test(&request_, &flag, &status));
            return flag;
        }

        void wait() noexcept {
            MPI_Status status = {};
            MPI_CALL_AND_CHECK(MPI_Wait(&request_, &status));
        }

        ~mpi_async_send() {
            wait();
        }


    private:
        Container data_;
        MPI_Request request_ = MPI_REQUEST_NULL;

        void send_(int dest, int tag, MPI_Comm com) {
            MPI_CALL_AND_CHECK(
                    MPI_Isend(data_.data(), data_.size(), mpi_data_type<T>::value, dest, tag, com, &request_));
        }

    };


    template<typename T>
    class mpi_async_recv {
    public:
        mpi_async_recv(int source, int tag, MPI_Comm com = MPI_COMM_WORLD) {
            int flag = 0;
            MPI_Status status = {};
            MPI_CALL_AND_CHECK(MPI_Iprobe(source, tag, com, &flag, &status));
            if (flag) {
                int count = 0;
                MPI_Get_count(&status, mpi_data_type<T>::value, &count);
                data_.resize(count);
                MPI_CALL_AND_CHECK(
                        MPI_Irecv(data_.data(), count, mpi_data_type<T>::value, source, tag, com, &request_));
            }
        }

        mpi_async_recv(const mpi_async_recv &) = delete;

        mpi_async_recv &operator=(const mpi_async_recv &) = delete;

        mpi_async_recv &operator=(mpi_async_recv &&r) noexcept {
            wait();
            data_ = std::move(r.data_);
            request_ = std::move(r.request_);
            r.request_ = MPI_REQUEST_NULL;
            return *this;
        }

        mpi_async_recv(mpi_async_recv &&r) : data_(std::move(r.data_)), request_(std::move(r.request_)) {
            r.request_ = MPI_REQUEST_NULL;
        }

        bool test() noexcept {
            int flag = 0;
            MPI_Status status = {};
            MPI_CALL_AND_CHECK(MPI_Test(&request_, &flag, &status));
            return flag;
        }

        void wait() noexcept {
            MPI_Status status = {};
            MPI_CALL_AND_CHECK(MPI_Wait(&request_, &status));
        }

        /*! @attention Only call this once
         *
         * @return Moved data
         */
        std::vector<T> get() {
            wait();
            return std::move(data_);
        }

        ~mpi_async_recv() {
            wait();
        }


    private:
        std::vector<T> data_;
        MPI_Request request_ = MPI_REQUEST_NULL;

    };

}


#endif //DIST_VEC_RET_MPI_ASYNC_SEND_HPP
