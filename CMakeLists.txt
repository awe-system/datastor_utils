cmake_minimum_required(VERSION 3.7)
project(datastor_utils)

set(CMAKE_CXX_STANDARD 11)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

set(CPACK_GENERATOR RPM)

set(BOOST_INCLUDEDIR "/usr/local/include")
set(BOOST_LIBRARYDIR "/usr/local/lib")
find_package(Boost 1.69.0 REQUIRED COMPONENTS)

endif ()

include_directories("./data")
include_directories("./functions")
include_directories("functions/thread_pool")
include_directories("conf")
include_directories("log/include")
include_directories("log")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
include_directories("./include")
include_directories(".")
include_directories("D://tool//boost_1_68_0//boost_1_68_0")
endif ()

set(SOURCE_FILES
        conf/awe_conf/env.cpp
        conf/awe_conf/env.h
        conf/awe_conf_error.h
        conf/awe_env.conf
        data/lt_data/lt_data_t.h
        data/lt_data/json_obj.cpp
        data/lt_data/json_obj.h
        data/lt_data/ref_obj.cpp
        data/lt_data/ref_obj.h
        data/clean.sh
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
        functions/lt_function/thread_pool.hpp
        functions/lt_function_error.h
        functions/lt_function/stronger_cond.h
        functions/lt_function/lt_safe_set.h
        functions/lt_function/utils.cpp
        functions/lt_function/utils.h
        algo/algo.cpp algo/algo.h
        algo/algo_block_io_jbod.cpp algo/algo_block_io_jbod.h
        algo/algo_block_io_jbod_nosuper.cpp algo/algo_block_io_jbod_nosuper.h
        algo/algo_block_io_super.cpp algo/algo_block_io_super.h
        algo/algo_block_multi_super.cpp algo/algo_block_multi_super.h
        algo/algo_block_open_ref.cpp algo/algo_block_open_ref.h
        algo/block_request.h algo/block_request.cpp
        algo/algo_nocopy_request_mapper.cpp algo/algo_nocopy_request_mapper.h
        algo/algo_range.cpp algo/algo_range.h
        algo/algo_section.cpp algo/algo_section.h
        algo/request_ref.h algo/request_ref.cpp
        algo/super_algo_device.cpp algo/super_algo_device.h
        algo/test_algo_device.h algo/test_algo_device.cpp
        algo/algo_interface.h algo/algo_interface.cpp
        algo/debug.h algo/debug.cpp
        README.md functions/lt_function/lt_safe_set.h algo/test_block_io_set.cpp algo/test_block_io_set.h algo/algo_sections.cpp algo/algo_sections.h algo/algo_bitmap.cpp algo/algo_bitmap.h algo/algo_bitmaps.cpp algo/algo_bitmaps.h
        data/lt_data/lt_stack.h
        data/lt_data/lt_mempool.h)

set(LOG_FILES
        log/src/ss_logger.h
        log/src/ss_logger.cpp
        log/src/awe_log.cpp
        log/src/ss_log4cpp.cpp
        log/src/log4cpp_src/Appender.cpp
        log/src/log4cpp_src/AppenderSkeleton.cpp
        log/src/log4cpp_src/AppendersFactory.cpp
        log/src/log4cpp_src/BufferingAppender.cpp
        log/src/log4cpp_src/FactoryParams.cpp
        log/src/log4cpp_src/LayoutsFactory.cpp
        log/src/log4cpp_src/LevelEvaluator.cpp
        log/src/log4cpp_src/Localtime.cpp
        log/src/log4cpp_src/PassThroughLayout.cpp
        log/src/log4cpp_src/TriggeringEventEvaluatorFactory.cpp
        log/src/log4cpp_src/LayoutAppender.cpp
        log/src/log4cpp_src/FileAppender.cpp
        log/src/log4cpp_src/DailyRollingFileAppender.cpp
        log/src/log4cpp_src/RollingFileAppender.cpp
        log/src/log4cpp_src/FixedContextCategory.cpp
        log/src/log4cpp_src/IdsaAppender.cpp
        log/src/log4cpp_src/OstreamAppender.cpp
        log/src/log4cpp_src/StringQueueAppender.cpp
        log/src/log4cpp_src/SyslogAppender.cpp
        log/src/log4cpp_src/RemoteSyslogAppender.cpp
        log/src/log4cpp_src/SimpleLayout.cpp
        log/src/log4cpp_src/BasicLayout.cpp
        log/src/log4cpp_src/PatternLayout.cpp
        log/src/log4cpp_src/Category.cpp
        log/src/log4cpp_src/CategoryStream.cpp
        log/src/log4cpp_src/HierarchyMaintainer.cpp
        log/src/log4cpp_src/Configurator.cpp
        log/src/log4cpp_src/BasicConfigurator.cpp
        log/src/log4cpp_src/SimpleConfigurator.cpp
        log/src/log4cpp_src/PropertyConfigurator.cpp
        log/src/log4cpp_src/PropertyConfiguratorImpl.cpp
        log/src/log4cpp_src/LoggingEvent.cpp
        log/src/log4cpp_src/Priority.cpp
        log/src/log4cpp_src/NDC.cpp
        log/src/log4cpp_src/Filter.cpp
        log/src/log4cpp_src/TimeStamp.cpp
        log/src/log4cpp_src/StringUtil.cpp
        log/src/log4cpp_src/Properties.cpp
        log/src/log4cpp_src/Win32DebugAppender.cpp
        log/src/log4cpp_src/NTEventLogAppender.cpp
        log/src/log4cpp_src/DllMain.cpp
        log/src/log4cpp_src/DummyThreads.cpp
        log/src/log4cpp_src/MSThreads.cpp
        log/src/log4cpp_src/OmniThreads.cpp
        log/src/log4cpp_src/PThreads.cpp
        log/src/log4cpp_src/PortabilityImpl.cpp
        log/src/log4cpp_src/AbortAppender.cpp
        data/lt_data/lt_mempool.h)


if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(boostdir "D://tool//boost_1_68_0//boost_1_68_0//stage//lib//lib")#根据boost安装位置调整
    add_library(datastor_utils ${SOURCE_FILES} ${LOG_FILES})
    target_link_libraries(datastor_utils "${boostdir}boost_system-vc141-mt-gd-x64-1_68.lib")
    target_link_libraries(datastor_utils "${boostdir}boost_date_time-vc141-mt-gd-x64-1_68.lib")
    target_link_libraries(datastor_utils "${boostdir}boost_regex-vc141-mt-gd-x64-1_68.lib")
    target_link_libraries(datastor_utils "${boostdir}boost_thread-vc141-mt-gd-x64-1_68.lib")
    target_link_libraries(datastor_utils "${boostdir}boost_chrono-vc141-mt-gd-x64-1_68.lib")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_library(datastor_utils SHARED ${SOURCE_FILES} ${LOG_FILES} )
    target_link_libraries(datastor_utils boost_system boost_date_time boost_thread)
endif()

set_target_properties(datastor_utils PROPERTIES LINKER_LANGUAGE CXX)
add_executable(test_function functions/test_functions.cpp algo/test_block_io_set.cpp algo/test_block_io_set.h)
target_link_libraries(test_function datastor_utils)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(include_dir "include")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(include_dir "/usr/local/include")
endif ()

INSTALL(FILES functions/lt_function/thread_pool.hpp
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
        functions/lt_function/stronger_cond.h
        functions/lt_function/lt_safe_set.h
        functions/lt_function/utils.h
        functions/base64/base64.h
        functions/lt_function_error.h
        DESTINATION ${include_dir}/lt_function)
INSTALL(FILES data/lt_data/lt_data_t.h
        data/lt_data/json_obj.h
        data/lt_data/ref_obj.h
        data/lt_data/lt_stack.h
        data/lt_data/lt_mempool.h
        data/lt_data_error.h
        DESTINATION ${include_dir}/lt_data)


INSTALL(FILES log/include/awe_log.h
        log/awe_log_error.h
        DESTINATION ${include_dir})

INSTALL(FILES algo/algo.h
        algo/algo_error.h
        algo/algo_interface.h
        algo/algo_block_io_jbod.h
        algo/algo_block_io_jbod_nosuper.h
        algo/algo_block_io_super.h
        algo/algo_block_multi_super.h
        algo/algo_block_open_ref.h
        algo/algo_nocopy_request_mapper.h
        algo/algo_range.h
        algo/algo_section.h
        algo/algo_sections.h
        algo/block_request.h
        algo/request_ref.h
        algo/super_algo_device.h
        algo/test_algo_device.h
        algo/test_block_io_set.h
        algo/debug.h
        DESTINATION ${include_dir}/algo)

INSTALL(FILES conf/awe_conf/env.h DESTINATION  ${include_dir}/awe_conf)
INSTALL(FILES conf/awe_conf_error.h DESTINATION  ${include_dir}/awe_conf)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
target_link_libraries(test_function boost_system boost_date_time boost_thread pthread)

INSTALL (TARGETS datastor_utils LIBRARY DESTINATION /usr/local/lib)

INSTALL (FILES raid/common.py raid/err.py raid/raid_manager.py raid/raid.py
        raid/storage.py raid/vg.py
        DESTINATION /opt/raid)


##################################################################################
###conf###########################################################################
execute_process(COMMAND python -m py_compile ../conf/lib/env.py)


INSTALL(FILES conf/lib/env.py DESTINATION /usr/lib64/python2.7)
#INSTALL(FILES conf/lib/env.pyc DESTINATION /usr/lib64/python2.7)

execute_process(COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/conf/conf OUTPUT_VARIABLE conf_dest)
execute_process(COMMAND date "+%Y%m%d" OUTPUT_VARIABLE date_time)
string(REPLACE "\n" "" date_time ${date_time} )
MESSAGE(STATUS "date_time : ${date_time}")

string(REPLACE "\n" "" conf_dest ${conf_dest} )
MESSAGE(STATUS "conf_dest : ${conf_dest}")

#execute_process(COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/conf/generate_profile.sh)
#install(DIRECTORY conf/conf DESTINATION "${conf_dest}/")
MESSAGE(STATUS "${conf_dest}/")
install(DIRECTORY conf/awe-conf/conf DESTINATION "${conf_dest}/awe_conf/")
install(FILES conf/awe_env.conf DESTINATION "/etc")
install(PROGRAMS conf/awe-conf/bin/check_start DESTINATION "${conf_dest}/awe_conf/bin/")
install(PROGRAMS conf/awe-conf/bin/awe_env_base DESTINATION "${conf_dest}/awe_conf/bin/")
install(PROGRAMS conf/awe-conf/bin/awe_env DESTINATION "${conf_dest}/awe_conf/bin/")
install(PROGRAMS conf/awe-conf/bin/version DESTINATION "${conf_dest}/awe_conf/bin/")
install(PROGRAMS conf/awe-conf/bin/awe_env_base DESTINATION "/usr/bin/")
install(PROGRAMS conf/awe-conf/bin/awe_env DESTINATION "/usr/bin/")
install(PROGRAMS conf/awe-conf/bin/version DESTINATION "/usr/bin/")
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


add_subdirectory(ut)