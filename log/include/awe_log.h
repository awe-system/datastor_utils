#ifndef SS_METADATA_LOG_HEAD_H
#define SS_METADATA_LOG_HEAD_H

#include <unistd.h>
#include <sys/syscall.h>
#include <string>
#include <string.h>

#define AWE_RED    " \033[31m "
#define AWE_GREEN    " \033[32m "
#define AWE_YELLOW    " \033[33m "
#define AWE_PURPLE    " \033[35m "
#define AWE_BLUE    " \033[34m "
#define AWE_CYAN    " \033[36m "
#define AWE_NORMAL    " \033[30m "
#define AWE_LINE    "\33[4m"
#define AWE_RESET    " \033[0m "
#define RANDCLOLORSIZE strlen(AWE_RED)

std::string rand_color(const char *msg);

#define postfix(msg)  std::string(msg).append(AWE_BLUE).append(AWE_LINE)\
    .append(" [").append(__FILE__).append(":").append(__func__)\
    .append(":").append(std::to_string(__LINE__)).append("]").append(AWE_RESET)\
    .append(AWE_NORMAL).append("[")\
    .append(std::to_string( syscall(__NR_gettid))).append("]").c_str()

enum priority
{
    LOG_PRIORITY_ERROR,
    LOG_PRIORITY_WARN,
    LOG_PRIORITY_INFO,
    LOG_PRIORITY_DEBUG,
    LOG_PRIORITY_DEFAULT
};

#define AWE_LOG_ERROR(msg, ...) awe_log_error(rand_color(postfix(msg)).c_str(), ##__VA_ARGS__)
#define AWE_LOG_WARN(msg, ...)  awe_log_warn(rand_color(postfix(msg)).c_str(), ##__VA_ARGS__)
#define AWE_LOG_INFO(msg, ...)  awe_log_info(rand_color(postfix(msg)).c_str(), ##__VA_ARGS__)
#define AWE_LOG_DEBUG(msg, ...) awe_log_debug(rand_color(postfix(msg)).c_str(), ##__VA_ARGS__)

#define module_string(module) std::string("***[").append(module).append("]*** ")

#define module_connect(module, msg) module_string(module).append(msg).c_str()

#define AWE_MODULE_ERROR(module, msg, ...) AWE_LOG_ERROR(module_connect(module,msg), ##__VA_ARGS__)
#define AWE_MODULE_WARN(module, msg, ...)  AWE_LOG_WARN(module_connect(module,msg), ##__VA_ARGS__)
#define AWE_MODULE_INFO(module, msg, ...)  AWE_LOG_INFO(module_connect(module,msg), ##__VA_ARGS__)
#define AWE_MODULE_DEBUG(module, msg, ...) AWE_LOG_DEBUG(module_connect(module,msg), ##__VA_ARGS__)

void awe_log_error(const char *msg, ...);

void awe_log_warn(const char *msg, ...);

void awe_log_info(const char *msg, ...);

void awe_log_debug(const char *msg, ...);

void awe_log_set_priority(enum priority pri);

void awe_log_set_module_priority(const char *module, enum priority pri);

int init_awe_log(const std::string &project);
#endif //SS_METADATA_LOG_HEAD_H
