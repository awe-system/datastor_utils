cmake_minimum_required(VERSION 3.7)
project(lt-data)
set(CMAKE_CXX_STANDARD 11)
set(CPACK_GENERATOR "RPM")
include_directories(include)

set(DATA_FILES
        lt_data/lt_data_t.h)

set(JSON_FILES
        json/json_obj.h
        json/json_obj.cpp
        lt_data_error.h)

set(OBJECT_FILES
        object/ref_obj.h
        object/ref_obj.cpp)


add_library(lt_data SHARED ${DATA_FILES}  ${JSON_FILES} ${OBJECT_FILES})
target_link_libraries(lt_data boost_system boost_thread)
set_target_properties(lt_data PROPERTIES LINKER_LANGUAGE CXX)

add_executable(test_json json/test_json.cpp ${DATA_FILES}  ${JSON_FILES} ${OBJECT_FILES})
target_link_libraries(test_json lt_data boost_system boost_thread)


INSTALL (TARGETS lt_data LIBRARY DESTINATION /usr/local/lib)
INSTALL(FILES lt_data/lt_data_t.h
        json/json_obj.h
        object/ref_obj.h
        lt_data_error.h
        DESTINATION /usr/local/include/lt_data)

execute_process(COMMAND date "+%Y%m%d" OUTPUT_VARIABLE date_time)
string(REPLACE "\n" "" date_time ${date_time} )
MESSAGE(STATUS "date_time : ${date_time}")

# 以下为RPM信息的设置，包名,概述，供应者，版本, 分组等等信息，通过其变量名称可以知道意思
set(CPACK_PACKAGE_NAME "lt-data_${date_time}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "serversan lt-data")
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