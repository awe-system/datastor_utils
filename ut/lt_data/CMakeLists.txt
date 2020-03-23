
include_directories(../../data/lt_data)

add_executable(test_data test_data.cpp)
target_link_libraries(test_data datastor_utils boost_system boost_thread pthread)