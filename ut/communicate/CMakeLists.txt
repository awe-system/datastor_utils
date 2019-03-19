add_executable(test_comm test_comm.cpp
        conf/testclient.h conf/testclient.cpp
        conf/testclient_internal.cpp conf/testclient_internal.h
        )
include_directories(/usr/local/include)
include_directories(/usr/local/include/lt_data)
include_directories(/usr/local/include/lt_function)
target_link_libraries(test_comm datastor_utils boost_system boost_thread pthread)

add_executable(test_commserver test_commserver.cpp
        conf/testserver.h conf/testserver.cpp)
target_link_libraries(test_commserver datastor_utils boost_system boost_thread pthread)