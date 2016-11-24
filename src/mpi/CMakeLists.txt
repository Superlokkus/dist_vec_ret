find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

add_executable(mpi_main mpi_main.cpp)

set_property(TARGET dist_vec_ret_lib PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET dist_vec_ret_lib PROPERTY CXX_STANDARD 14)
target_link_libraries(mpi_main ${MPI_LIBRARIES})

if(MPI_COMPILE_FLAGS)
  set_target_properties(mpi_main PROPERTIES
    COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
  set_target_properties(mpi_main PROPERTIES
    LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()