cmake_minimum_required (VERSION 3.0)
project (dist_vec_ret)

set (PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

include_directories("${PROJECT_BINARY_DIR}")

set (CORE_SOURCE_FILES
${PROJECT_SOURCE_DIR}/dist_vec_ret.hpp
${PROJECT_SOURCE_DIR}/dist_vec_ret.cpp
)

add_library(dist_vec_ret_lib ${CORE_SOURCE_FILES})

set_property(TARGET dist_vec_ret_lib PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET dist_vec_ret_lib PROPERTY CXX_STANDARD 11)

add_subdirectory(${PROJECT_SOURCE_DIR}/mpi)

enable_testing()
add_subdirectory(test)