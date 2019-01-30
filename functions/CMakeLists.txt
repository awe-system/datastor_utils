cmake_minimum_required(VERSION 3.7)
project(lt-function)
set(CMAKE_CXX_STANDARD 11)
set(CPACK_GENERATOR "RPM")

include_directories(./)
include_directories(lt_function)
include_directories(/usr/local/include/lt_data)
include_directories(/usr/local/include/ss_log)

set(THREAD_POOL_FILES
        thread_pool/thread_pool.hpp)

set(COMMUNICATE_FILES
        communicate/action_stub.cpp
        communicate/lt_async_2safe_async.cpp
        communicate/lt_bidirectional_map.hpp
        communicate/lt_client_service.cpp
        communicate/lt_reference.cpp
        communicate/lt_safe_state.hpp
        communicate/lt_server_service.cpp
        communicate/lt_session.cpp
        communicate/lt_session_cli.cpp
        communicate/lt_session_cli_safe.cpp
        communicate/lt_session_cli_set.cpp
        communicate/lt_session_dog.cpp
        communicate/lt_session_serv.cpp
        communicate/lt_thread_server.cpp
        communicate/lt_watch_dog.cpp)

set(UTILS_FILES
        lt_function/utils.cpp)

set(BASE64_FILES
        base64/base64.cpp)

set(BOOST_INCLUDEDIR "/usr/local/include")
set(BOOST_LIBRARYDIR "/usr/local/lib")
find_package(Boost 1.62.0 EXACT REQUIRED COMPONENTS)

add_library(lt_function SHARED ${THREAD_POOL_FILES} ${COMMUNICATE_FILES} ${UTILS_FILES} ${BASE64_FILES})
set_target_properties(lt_function PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(lt_function ss_log lt_data boost_system boost_thread)

INSTALL (TARGETS lt_function LIBRARY DESTINATION /usr/local/lib)

INSTALL(FILES thread_pool/thread_pool.hpp
        communicate/action_stub.h
        communicate/lt_async_2safe_async.h
        communicate/lt_bidirectional_map.hpp
        communicate/lt_client_service.h
        communicate/lt_condition.h
        communicate/lt_data_translator.h
        communicate/lt_reference.h
        communicate/lt_safe_state.hpp
        communicate/lt_server_service.h
        communicate/lt_session.h
        communicate/lt_session_cli.h
        communicate/lt_session_cli_safe.h
        communicate/lt_session_cli_set.h
        communicate/lt_session_dog.h
        communicate/lt_session_serv.h
        communicate/lt_thread_server.h
        communicate/lt_watch_dog.h
        lt_function/stronger_cond.h
        lt_function/utils.h
        base64/base64.h
        lt_function_error.h
        DESTINATION /usr/local/include/lt_function)

##################################################################################
###conf###########################################################################

set(ENV_SOURCE_FILES
        env/env.cpp
        env/env.h)
add_library(ss_env SHARED ${ENV_SOURCE_FILES})

execute_process(COMMAND python -m py_compile lib/env.py)

INSTALL (TARGETS ss_env
        LIBRARY DESTINATION /usr/local/lib)
INSTALL(FILES env/env.h DESTINATION /usr/local/include/ss_conf)
INSTALL(FILES lib/env.py DESTINATION /usr/lib64/python2.7)
INSTALL(FILES lib/env.pyc DESTINATION /usr/lib64/python2.7)
INSTALL(FILES ss_conf_error.h DESTINATION /usr/local/include/ss_conf)

execute_process(COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/conf OUTPUT_VARIABLE conf_dest)
execute_process(COMMAND date "+%Y%m%d" OUTPUT_VARIABLE date_time)
string(REPLACE "\n" "" date_time ${date_time} )
MESSAGE(STATUS "date_time : ${date_time}")

string(REPLACE "\n" "" conf_dest ${conf_dest} )
MESSAGE(STATUS "conf_dest : ${conf_dest}")

execute_process(COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/conf/generate_profile.sh)

install(DIRECTORY awe_conf/conf DESTINATION "${conf_dest}/awe/")
install(DIRECTORY awe_conf/xml DESTINATION "${conf_dest}/yunke_serversan/")
install(FILES awe_env.conf DESTINATION "/etc")
install(PROGRAMS awe_conf/bin/awe_env_base DESTINATION "${conf_dest}/yunke_serversan/bin/")
install(PROGRAMS awe_conf/bin/asw_env DESTINATION "${conf_dest}/yunke_serversan/bin/")
install(PROGRAMS awe_conf/bin/version DESTINATION "${conf_dest}/yunke_serversan/bin/")
install(PROGRAMS awe_conf/bin/awe_env_base DESTINATION "/usr/bin/")
install(PROGRAMS awe_conf/bin/asw_env DESTINATION "/usr/bin/")
install(PROGRAMS awe_conf/bin/version DESTINATION "/usr/bin/")
install(PROGRAMS ss_conf_profile.sh DESTINATION "/etc/profile.d/")

###end conf
##################################################################################









execute_process(COMMAND date "+%Y%m%d" OUTPUT_VARIABLE date_time)
string(REPLACE "\n" "" date_time ${date_time} )
MESSAGE(STATUS "date_time : ${date_time}")


# 以下为RPM信息的设置，包名,概述，供应者，版本, 分组等等信息，通过其变量名称可以知道意思
set(CPACK_PACKAGE_NAME "lt-function_${date_time}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "serversan lt-function")
set(CPACK_PACKAGE_VENDOR "Yunke Serversan")
set(CPACK_PACKAGE_VERSION "1.1.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_RPM_PACKAGE_GROUP "Serversan")
set(CPACK_RPM_PACKAGE_URL "http://10.1.102.180/zentao")
set(CPACK_RPM_PACKAGE_DESCRIPTION "ServerSan Dependencies")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_RPM_PACKAGE_LICENSE "ServerSan Licence")

include (CPack)