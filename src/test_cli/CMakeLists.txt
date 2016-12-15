
find_package(Boost COMPONENTS system filesystem REQUIRED)

add_executable(test_cli test_cli.cpp)
set_property(TARGET test_cli PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET test_cli PROPERTY CXX_STANDARD 14)
target_link_libraries(test_cli dist_vec_ret_lib ${Boost_LIBRARIES})