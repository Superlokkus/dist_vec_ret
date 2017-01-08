/*! @file mpi_exception.hpp
 *
 */

#ifndef DIST_VEC_RET_MPI_EXCEPTION_HPP
#define DIST_VEC_RET_MPI_EXCEPTION_HPP

#include <mpi.h>
#include <stdexcept>
#include <string>

struct mpi_exception : std::runtime_error {
    explicit mpi_exception(const std::string &what, int mpi_error)
            : std::runtime_error(what), mpi_error_code_(mpi_error) {}

    explicit mpi_exception(const char *what, int mpi_error)
            : std::runtime_error(what), mpi_error_code_(mpi_error) {}

    static mpi_exception init_with_error_code(int mpi_error) {
        char buffer[MPI_MAX_ERROR_STRING] = {};
        int string_len = 0;
        MPI_Error_string(mpi_error, buffer, &string_len);
        return mpi_exception{buffer, mpi_error};
    }

    int mpi_error_code() const {
        return mpi_error_code_;
    }

private:
    int mpi_error_code_;
};

#define MPI_CALL_AND_CHECK(MPI_FUNC) do{\
int call_ret = MPI_FUNC;\
if (call_ret != MPI_SUCCESS)\
    {\
    throw mpi_exception(std::string( #MPI_FUNC ) + " failed with " + std::to_string(call_ret), call_ret); \
    }} while(0);


#endif //DIST_VEC_RET_MPI_EXCEPTION_HPP
