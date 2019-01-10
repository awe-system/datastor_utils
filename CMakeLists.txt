cmake_minimum_required(VERSION 3.7)
project(datastor_utils)

set(CMAKE_CXX_STANDARD 11)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

set(CPACK_GENERATOR RPM)

set(BOOST_INCLUDEDIR "/usr/local/include")
set(BOOST_LIBRARYDIR "/usr/local/lib")
find_package(Boost 1.68.0 EXACT REQUIRED COMPONENTS)

endif ()
include_directories("./data")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
include_directories("./include")
include_directories(".")
include_directories("D://tool//boost_1_68_0//boost_1_68_0")
endif ()

set(SOURCE_FILES
        conf/env/env.cpp
        conf/env/env.h
        conf/awe_conf_error.h
        conf/awe_env.conf
        data/lt_data/lt_data_t.h
        data/json/json_obj.cpp
        data/json/json_obj.h
        data/json/test_json.cpp
        data/object/ref_obj.cpp
        data/object/ref_obj.h
        data/clean.sh
        data/CMakeLists.txt
        data/lt_data_error.h
        functions/base64/base64.cpp
        functions/base64/base64.h
        functions/communicate/action_stub.cpp
        functions/communicate/action_stub.h
        functions/communicate/lt_async_2safe_async.cpp
        functions/communicate/lt_async_2safe_async.h
        functions/communicate/lt_bidirectional_map.hpp
        functions/communicate/lt_client_service.cpp
        functions/communicate/lt_client_service.h
        functions/communicate/lt_condition.h
        functions/communicate/lt_data_translator.h
        functions/communicate/lt_reference.cpp
        functions/communicate/lt_reference.h
        functions/communicate/lt_safe_state.hpp
        functions/communicate/lt_server_service.cpp
        functions/communicate/lt_server_service.h
        functions/communicate/lt_session.cpp
        functions/communicate/lt_session.h
        functions/communicate/lt_session_cli.cpp
        functions/communicate/lt_session_cli.h
        functions/communicate/lt_session_cli_safe.cpp
        functions/communicate/lt_session_cli_safe.h
        functions/communicate/lt_session_cli_set.cpp
        functions/communicate/lt_session_cli_set.h
        functions/communicate/lt_session_dog.cpp
        functions/communicate/lt_session_dog.h
        functions/communicate/lt_session_serv.cpp
        functions/communicate/lt_session_serv.h
        functions/communicate/lt_thread_server.cpp
        functions/communicate/lt_thread_server.h
        functions/communicate/lt_watch_dog.cpp
        functions/communicate/lt_watch_dog.h
        functions/thread_pool/thread_pool.hpp
        functions/lt_function_error.h
        functions/utils/stronger_cond.h
        functions/utils/utils.cpp
        functions/utils/utils.h
        functions/CMakeLists.txt
#        functions/lt_function_error.h
#        log/include/log4cpp/threading/BoostThreads.hh
#        log/include/log4cpp/threading/DummyThreads.hh
#        log/include/log4cpp/threading/Makefile
#        log/include/log4cpp/threading/Makefile.am
#        log/include/log4cpp/threading/Makefile.in
#        log/include/log4cpp/threading/MSThreads.hh
#        log/include/log4cpp/threading/OmniThreads.hh
#        log/include/log4cpp/threading/PThreads.hh
#        log/include/log4cpp/threading/Threading.hh
#        log/include/log4cpp/AbortAppender.hh
#        log/include/log4cpp/Appender.hh
#        log/include/log4cpp/AppendersFactory.hh
#        log/include/log4cpp/AppenderSkeleton.hh
#        log/include/log4cpp/BasicConfigurator.hh
#        log/include/log4cpp/BasicLayout.hh
#        log/include/log4cpp/BufferingAppender.hh
#        log/include/log4cpp/Category.hh
#        log/include/log4cpp/CategoryStream.hh
#        log/include/log4cpp/config-MinGW32.h
#        log/include/log4cpp/config-openvms.h
#        log/include/log4cpp/config-win32-stlport-boost.h
#        log/include/log4cpp/config-win32.h
#        log/include/log4cpp/config.h
#        log/include/log4cpp/Configurator.hh
#        log/include/log4cpp/convenience.h
#        log/include/log4cpp/DailyRollingFileAppender.hh
#        log/include/log4cpp/Export.hh
#        log/include/log4cpp/FactoryParams.hh
#        log/include/log4cpp/FileAppender.hh
#        log/include/log4cpp/Filter.hh
#        log/include/log4cpp/FixedContextCategory.hh
#        log/include/log4cpp/HierarchyMaintainer.hh
#        log/include/log4cpp/IdsaAppender.hh
#        log/include/log4cpp/Layout.hh
#        log/include/log4cpp/LayoutAppender.hh
#        log/include/log4cpp/LayoutsFactory.hh
#        log/include/log4cpp/LevelEvaluator.hh
#        log/include/log4cpp/LoggingEvent.hh
#        log/include/log4cpp/Makefile
#        log/include/log4cpp/Makefile.am
#        log/include/log4cpp/Makefile.in
#        log/include/log4cpp/Manipulator.hh
#        log/include/log4cpp/NDC.hh
#        log/include/log4cpp/NTEventLogAppender.hh
#        log/include/log4cpp/OstreamAppender.hh
#        log/include/log4cpp/PassThroughLayout.hh
#        log/include/log4cpp/PatternLayout.hh
#        log/include/log4cpp/Portability.hh
#        log/include/log4cpp/Priority.hh
#        log/include/log4cpp/PropertyConfigurator.hh
#        log/include/log4cpp/RemoteSyslogAppender.hh
#        log/include/log4cpp/RollingFileAppender.hh
#        log/include/log4cpp/SimpleConfigurator.hh
#        log/include/log4cpp/SimpleLayout.hh
#        log/include/log4cpp/SmtpAppender.hh
#        log/include/log4cpp/ss_log4cpp.h
#        log/include/log4cpp/StringQueueAppender.hh
#        log/include/log4cpp/SyslogAppender.hh
#        log/include/log4cpp/TimeStamp.hh
#        log/include/log4cpp/TriggeringEventEvaluator.hh
#        log/include/log4cpp/TriggeringEventEvaluatorFactory.hh
#        log/include/log4cpp/Win32DebugAppender.hh
#        log/include/ss_log.h
#        log/src/log4cpp_src/AbortAppender.cpp
#        log/src/log4cpp_src/Appender.cpp
#        log/src/log4cpp_src/AppendersFactory.cpp
#        log/src/log4cpp_src/AppenderSkeleton.cpp
#        log/src/log4cpp_src/BasicConfigurator.cpp
#        log/src/log4cpp_src/BasicLayout.cpp
#        log/src/log4cpp_src/BufferingAppender.cpp
#        log/src/log4cpp_src/Category.cpp
#        log/src/log4cpp_src/CategoryStream.cpp
#        log/src/log4cpp_src/Configurator.cpp
#        log/src/log4cpp_src/DailyRollingFileAppender.cpp
#        log/src/log4cpp_src/DllMain.cpp
#        log/src/log4cpp_src/DummyThreads.cpp
#        log/src/log4cpp_src/FactoryParams.cpp
#        log/src/log4cpp_src/FileAppender.cpp
#        log/src/log4cpp_src/Filter.cpp
#        log/src/log4cpp_src/FixedContextCategory.cpp
#        log/src/log4cpp_src/HierarchyMaintainer.cpp
#        log/src/log4cpp_src/IdsaAppender.cpp
#        log/src/log4cpp_src/LayoutAppender.cpp
#        log/src/log4cpp_src/LayoutsFactory.cpp
#        log/src/log4cpp_src/LevelEvaluator.cpp
#        log/src/log4cpp_src/Localtime.cpp
#        log/src/log4cpp_src/Localtime.hh
#        log/src/log4cpp_src/LoggingEvent.cpp
#        log/src/log4cpp_src/MSThreads.cpp
#        log/src/log4cpp_src/NDC.cpp
#        log/src/log4cpp_src/NTEventLogAppender.cpp
#        log/src/log4cpp_src/OmniThreads.cpp
#        log/src/log4cpp_src/OstreamAppender.cpp
#        log/src/log4cpp_src/PassThroughLayout.cpp
#        log/src/log4cpp_src/PatternLayout.cpp
#        log/src/log4cpp_src/PortabilityImpl.cpp
#        log/src/log4cpp_src/PortabilityImpl.hh
#        log/src/log4cpp_src/Priority.cpp
#        log/src/log4cpp_src/Properties.cpp
#        log/src/log4cpp_src/Properties.hh
#        log/src/log4cpp_src/PropertyConfigurator.cpp
#        log/src/log4cpp_src/PropertyConfiguratorImpl.cpp
#        log/src/log4cpp_src/PropertyConfiguratorImpl.hh
#        log/src/log4cpp_src/PThreads.cpp
#        log/src/log4cpp_src/RemoteSyslogAppender.cpp
#        log/src/log4cpp_src/RollingFileAppender.cpp
#        log/src/log4cpp_src/SimpleConfigurator.cpp
#        log/src/log4cpp_src/SimpleLayout.cpp
#        log/src/log4cpp_src/SmtpAppender.cpp
#        log/src/log4cpp_src/snprintf.c
#        log/src/log4cpp_src/StringQueueAppender.cpp
#        log/src/log4cpp_src/StringUtil.cpp
#        log/src/log4cpp_src/StringUtil.hh
#        log/src/log4cpp_src/SyslogAppender.cpp
#        log/src/log4cpp_src/TimeStamp.cpp
#        log/src/log4cpp_src/TriggeringEventEvaluatorFactory.cpp
#        log/src/log4cpp_src/Win32DebugAppender.cpp
#        log/src/ss_log.cpp
#        log/src/ss_log4cpp.cpp
#        log/src/ss_logger.cpp
#        log/src/ss_logger.h
#        log/CMakeLists.txt
#        log/main.cpp
#        log/readne.txt
#        log/ss_log_error.h
        README.md)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(boostdir "D://tool//boost_1_68_0//boost_1_68_0//stage//lib//lib")#根据boost安装位置调整
    add_library(datastor_utils ${SOURCE_FILES})
    target_link_libraries(datastor_utils "${boostdir}boost_system-vc141-mt-gd-x64-1_68.lib")
    target_link_libraries(datastor_utils "${boostdir}boost_date_time-vc141-mt-gd-x64-1_68.lib")
    target_link_libraries(datastor_utils "${boostdir}boost_regex-vc141-mt-gd-x64-1_68.lib")
    target_link_libraries(datastor_utils "${boostdir}boost_thread-vc141-mt-gd-x64-1_68.lib")
    target_link_libraries(datastor_utils "${boostdir}boost_chrono-vc141-mt-gd-x64-1_68.lib")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_library(datastor_utils ${SOURCE_FILES})
    target_link_libraries(datastor_utils boost_system boost_thread)
endif()

set_target_properties(datastor_utils PROPERTIES LINKER_LANGUAGE CXX)
add_executable(test_function functions/test_functions.cpp)
target_link_libraries(test_function datastor_utils)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
target_link_libraries(test_function boost_system boost_thread pthread)

INSTALL (TARGETS datastor_utils LIBRARY DESTINATION /usr/local/lib)
INSTALL(FILES functions/thread_pool/thread_pool.hpp
        functions/communicate/action_stub.h
        functions/communicate/lt_async_2safe_async.h
        functions/communicate/lt_bidirectional_map.hpp
        functions/communicate/lt_client_service.h
        functions/communicate/lt_condition.h
        functions/communicate/lt_data_translator.h
        functions/communicate/lt_reference.h
        functions/communicate/lt_safe_state.hpp
        functions/communicate/lt_server_service.h
        functions/communicate/lt_session.h
        functions/communicate/lt_session_cli.h
        functions/communicate/lt_session_cli_safe.h
        functions/communicate/lt_session_cli_set.h
        functions/communicate/lt_session_dog.h
        functions/communicate/lt_session_serv.h
        functions/communicate/lt_thread_server.h
        functions/communicate/lt_watch_dog.h
        functions/utils/stronger_cond.h
        functions/utils/utils.h
        functions/base64/base64.h
        functions/lt_function_error.h
        DESTINATION /usr/local/include/lt_function)
INSTALL(FILES data/lt_data/lt_data_t.h
        data/json/json_obj.h
        data/object/ref_obj.h
        data/lt_data_error.h
        DESTINATION /usr/local/include/lt_data)



##################################################################################
###conf###########################################################################
execute_process(COMMAND python -m py_compile ../conf/lib/env.py)

INSTALL(FILES conf/env/env.h DESTINATION /usr/local/include/awe_conf)
INSTALL(FILES conf/lib/env.py DESTINATION /usr/lib64/python2.7)
#INSTALL(FILES conf/lib/env.pyc DESTINATION /usr/lib64/python2.7)
INSTALL(FILES conf/awe_conf_error.h DESTINATION /usr/local/include/awe_conf)

execute_process(COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/conf/conf OUTPUT_VARIABLE conf_dest)
execute_process(COMMAND date "+%Y%m%d" OUTPUT_VARIABLE date_time)
string(REPLACE "\n" "" date_time ${date_time} )
MESSAGE(STATUS "date_time : ${date_time}")

string(REPLACE "\n" "" conf_dest ${conf_dest} )
MESSAGE(STATUS "conf_dest : ${conf_dest}")

#execute_process(COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/conf/generate_profile.sh)
#install(DIRECTORY conf/awe_conf DESTINATION "${conf_dest}/")
MESSAGE(STATUS "${conf_dest}/")
install(DIRECTORY conf/awe_conf/conf DESTINATION "${conf_dest}/awe_conf/")
install(FILES conf/awe_env.conf DESTINATION "/etc")
install(PROGRAMS conf/awe_conf/bin/awe_env_base DESTINATION "${conf_dest}/awe_conf/bin/")
install(PROGRAMS conf/awe_conf/bin/awe_env DESTINATION "${conf_dest}/awe_conf/bin/")
install(PROGRAMS conf/awe_conf/bin/version DESTINATION "${conf_dest}/awe_conf/bin/")
install(PROGRAMS conf/awe_conf/bin/awe_env_base DESTINATION "/usr/bin/")
install(PROGRAMS conf/awe_conf/bin/awe_env DESTINATION "/usr/bin/")
install(PROGRAMS conf/awe_conf/bin/version DESTINATION "/usr/bin/")
#install(PROGRAMS conf/awe_conf_profile.sh DESTINATION "/etc/profile.d/")

###end conf
##################################################################################


execute_process(COMMAND date "+%Y%m%d" OUTPUT_VARIABLE date_time)
string(REPLACE "\n" "" date_time ${date_time} )
MESSAGE(STATUS "date_time : ${date_time}")

# 以下为RPM信息的设置，包名,概述，供应者，版本, 分组等等信息，通过其变量名称可以知道意思
set(CPACK_PACKAGE_NAME "datastor_utils_${date_time}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "awe datastor_utils")
set(CPACK_PACKAGE_VENDOR "awe datastor_utils")
set(CPACK_PACKAGE_VERSION "1.1.1")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_RPM_PACKAGE_GROUP "AWE")
set(CPACK_RPM_PACKAGE_URL "https://github.com/awe-systerm/datastor_utils/tree/master")
set(CPACK_RPM_PACKAGE_DESCRIPTION "awe datastor Dependencies")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_RPM_PACKAGE_LICENSE "AWE Licence")
include (CPack)

endif ()
