include_directories(/usr/local/include)
include_directories(/usr/local/include/lt_data)
include_directories(/usr/local/include/lt_function)

add_executable(test_aio aio_test.cpp)
target_link_libraries(test_aio datastor_utils boost_system boost_thread pthread)