cmake_minimum_required (VERSION 3.0)
project (dist_vec_ret)

set (PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

include_directories("${PROJECT_BINARY_DIR}")

set (dist_vec_ret_lib_headers
${PROJECT_SOURCE_DIR}/dist_vec_ret.hpp
)

add_library(dist_vec_ret_lib ${PROJECT_SOURCE_DIR}/dist_vec_ret.cpp)
target_include_directories(dist_vec_ret_lib INTERFACE ${PROJECT_SOURCE_DIR})

set_property(TARGET dist_vec_ret_lib PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET dist_vec_ret_lib PROPERTY CXX_STANDARD 14)

add_subdirectory(${PROJECT_SOURCE_DIR}/mpi)


add_subdirectory(test)