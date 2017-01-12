find_package(MPI REQUIRED)
find_package(Boost COMPONENTS system filesystem serialization mpi REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

set(mpi_main_sources
        mpi_main.cpp
        mpi_utility.cpp
        mpi_utility.hpp
        mpi_exception.hpp
        mpi_global_weight_state.cpp
        mpi_global_weight_state.hpp
        mpi_async.hpp)

add_executable(mpi_main ${mpi_main_sources})

set_property(TARGET mpi_main PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET mpi_main PROPERTY CXX_STANDARD 14)

target_link_libraries(mpi_main ${MPI_LIBRARIES} ${Boost_LIBRARIES} dist_vec_ret_lib)

if(MPI_COMPILE_FLAGS)
  set_target_properties(mpi_main PROPERTIES
    COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
  set_target_properties(mpi_main PROPERTIES
    LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()