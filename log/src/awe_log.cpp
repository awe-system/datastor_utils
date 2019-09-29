#include <src/log4cpp_src/StringUtil.hh>
#include <awe_log.h>
#include <thread>

#include "log4cpp/ss_log4cpp.h"

extern ss_log4cpp &logger;

#define USE_LOG

void awe_log_error(const char * msg, ...)
{
#ifdef USE_LOG
    va_list ap;
    va_start(ap, msg);
    logger.error( log4cpp::StringUtil::vform(msg, ap));
    va_end(ap);
#endif
}

void awe_log_warn(const char * msg, ...)
{
#ifdef USE_LOG
    va_list ap;
    va_start(ap, msg);
    logger.warn( log4cpp::StringUtil::vform(msg, ap) );
    va_end(ap);
#endif
}

void awe_log_info(const char * msg, ...)
{
#ifdef USE_LOG
    va_list ap;
    va_start(ap, msg);
    logger.info( log4cpp::StringUtil::vform(msg, ap) );
    va_end(ap);
#endif
}

void awe_log_debug(const char *msg, ...)
{
#ifdef USE_LOG
    va_list ap;
    va_start(ap, msg);
    logger.debug(log4cpp::StringUtil::vform(msg, ap));
    va_end(ap);
#endif
}

void awe_log_set_priority(enum priority pri)
{
#ifdef USE_LOG
    logger.set_priority(pri);
#endif
}

void awe_log_set_module_priority(const char *module, enum priority pri)
{
#ifdef USE_LOG
    logger.set_module_priority(module_string(module), pri);
#endif
}

#include <unistd.h>
#include <stdio.h>

#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)


std::string tid_str()
{
    auto id = gettid();
    return std::to_string(id);
}
