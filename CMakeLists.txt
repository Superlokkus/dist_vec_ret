cmake_minimum_required (VERSION 3.0)
project (dist_vec_ret)

set (PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

include_directories("${PROJECT_BINARY_DIR}")

set (dist_vec_ret_lib_headers
        ${PROJECT_SOURCE_DIR}/dist_vec_ret.hpp
        ${PROJECT_SOURCE_DIR}/dist_vec_ret_manager.hpp
        ${PROJECT_SOURCE_DIR}/time_utility.hpp
)

set(dist_vec_ret_lib_implementation_files
        ${PROJECT_SOURCE_DIR}/dist_vec_ret.cpp
        ${PROJECT_SOURCE_DIR}/dist_vec_ret_manager.cpp
        ${PROJECT_SOURCE_DIR}/time_utility.cpp
        )

add_library(dist_vec_ret_lib ${dist_vec_ret_lib_implementation_files})
target_include_directories(dist_vec_ret_lib INTERFACE ${PROJECT_SOURCE_DIR})

set_property(TARGET dist_vec_ret_lib PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET dist_vec_ret_lib PROPERTY CXX_STANDARD 14)

add_subdirectory(${PROJECT_SOURCE_DIR}/mpi)
add_subdirectory(${PROJECT_SOURCE_DIR}/test_cli)

enable_testing()
add_subdirectory(test)