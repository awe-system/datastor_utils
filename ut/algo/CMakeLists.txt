set(algo_src )

add_compile_options("-fprofile-arcs")
add_compile_options("-ftest-coverage")

add_executable(test_algo test_algo.cpp ${algo_src})
include_directories(/usr/local/include)
include_directories(/usr/local/include/lt_data)
include_directories(/usr/local/include/lt_function)
target_link_libraries(test_algo datastor_utils gcov boost_system boost_date_time boost_thread pthread)