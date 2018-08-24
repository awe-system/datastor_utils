cmake_minimum_required(VERSION 3.7)

PROJECT(ss-log)
set(CMAKE_CXX_STANDARD 11)
INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(.)

set(SOURCE_FILES src/ss_logger.h
        src/ss_logger.cpp
        src/ss_log.cpp
        src/ss_log4cpp.cpp
        src/log4cpp_src/Appender.cpp
        src/log4cpp_src/AppenderSkeleton.cpp
        src/log4cpp_src/AppendersFactory.cpp
        src/log4cpp_src/BufferingAppender.cpp
        src/log4cpp_src/FactoryParams.cpp
        src/log4cpp_src/LayoutsFactory.cpp
        src/log4cpp_src/LevelEvaluator.cpp
        src/log4cpp_src/Localtime.cpp
        src/log4cpp_src/PassThroughLayout.cpp
        src/log4cpp_src/TriggeringEventEvaluatorFactory.cpp
        src/log4cpp_src/LayoutAppender.cpp
        src/log4cpp_src/FileAppender.cpp
        src/log4cpp_src/DailyRollingFileAppender.cpp
        src/log4cpp_src/RollingFileAppender.cpp
        src/log4cpp_src/FixedContextCategory.cpp
        src/log4cpp_src/IdsaAppender.cpp
        src/log4cpp_src/OstreamAppender.cpp
        src/log4cpp_src/StringQueueAppender.cpp
        src/log4cpp_src/SyslogAppender.cpp
        src/log4cpp_src/RemoteSyslogAppender.cpp
        src/log4cpp_src/SimpleLayout.cpp
        src/log4cpp_src/BasicLayout.cpp
        src/log4cpp_src/PatternLayout.cpp
        src/log4cpp_src/Category.cpp
        src/log4cpp_src/CategoryStream.cpp
        src/log4cpp_src/HierarchyMaintainer.cpp
        src/log4cpp_src/Configurator.cpp
        src/log4cpp_src/BasicConfigurator.cpp
        src/log4cpp_src/SimpleConfigurator.cpp
        src/log4cpp_src/PropertyConfigurator.cpp
        src/log4cpp_src/PropertyConfiguratorImpl.cpp
        src/log4cpp_src/LoggingEvent.cpp
        src/log4cpp_src/Priority.cpp
        src/log4cpp_src/NDC.cpp
        src/log4cpp_src/Filter.cpp
        src/log4cpp_src/TimeStamp.cpp
        src/log4cpp_src/StringUtil.cpp
        src/log4cpp_src/Properties.cpp
        src/log4cpp_src/Win32DebugAppender.cpp
        src/log4cpp_src/NTEventLogAppender.cpp
        src/log4cpp_src/DllMain.cpp
        src/log4cpp_src/DummyThreads.cpp
        src/log4cpp_src/MSThreads.cpp
        src/log4cpp_src/OmniThreads.cpp
        src/log4cpp_src/PThreads.cpp
        src/log4cpp_src/PortabilityImpl.cpp
        src/log4cpp_src/AbortAppender.cpp)

ADD_LIBRARY(ss_log SHARED ${SOURCE_FILES})

#add_executable(ss-log ${SOURCE_FILES} main.cpp)
#target_link_libraries(ss-log pthread)

execute_process(COMMAND date "+%Y%m%d" OUTPUT_VARIABLE date_time)
string(REPLACE "\n" "" date_time ${date_time} )
MESSAGE(STATUS "date_time : ${date_time}")

INSTALL (TARGETS ss_log LIBRARY DESTINATION /usr/local/lib)
INSTALL(FILES
        ./include/ss_log.h
        ss_log_error.h
        DESTINATION /usr/local/include/ss_log)

# 以下为RPM信息的设置，包名,概述，供应者，版本, 分组等等信息，通过其变量名称可以知道意思
set(CPACK_PACKAGE_NAME "ss-log_${date_time}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "serversan log")
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


# 设置默认生成器，RPM生成器会构建RPM安装包，其它还有TGZ/ZIP等
set(CPACK_GENERATOR "RPM")

# 安装前和安装后执行的shell脚本, 会打包到RPM中，安装时执行。这里可扩展性很强, 放在源码目录下即可
# set(CPACK_RPM_PRE_INSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/pre_script.sh)
# set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/post_script.sh)

# 引入CPack模块，必须的
include(CPack)