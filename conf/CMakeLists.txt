cmake_minimum_required(VERSION 3.7)
project(ss_conf)

set(CMAKE_CXX_STANDARD 11)

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

#
INSTALL(FILES awe_conf_error.h DESTINATION /usr/local/include/ss_conf)

set(CPACK_GENERATOR "RPM")

execute_process(COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/conf OUTPUT_VARIABLE conf_dest)
execute_process(COMMAND date "+%Y%m%d" OUTPUT_VARIABLE date_time)
string(REPLACE "\n" "" date_time ${date_time} )
MESSAGE(STATUS "date_time : ${date_time}")

string(REPLACE "\n" "" conf_dest ${conf_dest} )
MESSAGE(STATUS "conf_dest : ${conf_dest}")

#execute_process(COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/generate_profile.sh)


install(DIRECTORY awe_conf/conf DESTINATION "${conf_dest}/yunke_serversan/")
install(DIRECTORY awe_conf/xml DESTINATION "${conf_dest}/yunke_serversan/")
install(FILES awe_env.conf DESTINATION "/etc")
install(PROGRAMS awe_conf/bin/awe_env_base DESTINATION "${conf_dest}/awe_conf/bin/")
install(PROGRAMS awe_conf/bin/awe_env DESTINATION "${conf_dest}/yunke_serversan/bin/")
install(PROGRAMS awe_conf/bin/version DESTINATION "${conf_dest}/yunke_serversan/bin/")
install(PROGRAMS awe_conf/bin/awe_env_base DESTINATION "/usr/bin/")
install(PROGRAMS awe_conf/bin/awe_env DESTINATION "/usr/bin/")
install(PROGRAMS awe_conf/bin/version DESTINATION "/usr/bin/")
install(PROGRAMS ss_conf_profile.sh DESTINATION "/etc/profile.d/")



#set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "cp ${CMAKE_CURRENT_SOURCE_DIR}/lnn.sh /tmp")
#execute_process(COMMAND bash /tmp/lnn.sh)

#execute_process(COMMAND "bash /usr/bin/ln -s /${conf_dest}/awe_conf/bin/awe_env_base /usr/bin/awe_env_base")
#execute_process(COMMAND "bash /usr/bin/ln -s /${conf_dest}/awe_conf/bin/awe_env /usr/bin/awe_env")
#execute_process(COMMAND "bash /usr/bin/ln -s /${conf_dest}/awe_conf/bin/bin/version /usr/bin/bin/version")
message("安装后 请执行source /etc/profile 让结果生效")

# 以下为RPM信息的设置，包名,概述，供应者，版本, 分组等等信息，通过其变量名称可以知道意思
set(CPACK_PACKAGE_NAME "ss-conf_${date_time}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "serversan conf")
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
