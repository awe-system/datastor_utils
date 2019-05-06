#ifndef SS_METADATA_LOG_HEAD_H
#define SS_METADATA_LOG_HEAD_H


#define postfix(msg)  std::string(msg).append(" ##[")\
    .append(__FILE__).append(":").append(__func__)\
    .append(":").append(std::to_string(__LINE__))\
    .append("]##").c_str()
    
enum priority
{
    LOG_PRIORITY_ERROR,
    LOG_PRIORITY_WARN,
    LOG_PRIORITY_INFO,
    LOG_PRIORITY_DEBUG
};

#define AWE_LOG_ERROR(msg, ...) awe_log_error(postfix(msg), ##__VA_ARGS__)
#define AWE_LOG_WARN(msg, ...)  awe_log_warn(postfix(msg), ##__VA_ARGS__)
#define AWE_LOG_INFO(msg, ...)  awe_log_info(postfix(msg), ##__VA_ARGS__)
#define AWE_LOG_DEBUG(msg, ...) awe_log_debug(postfix(msg), ##__VA_ARGS__)

#define module_connect(module,msg) std::string(" ***[").append(module)\
            .append("]*** ").append(msg).c_str()

#define AWE_MODULE_ERROR(module,msg, ...) AWE_LOG_ERROR(module_connect(module,msg), ##__VA_ARGS__)
#define AWE_MODULE_WARN(module,msg, ...)  AWE_LOG_WARN(module_connect(module,msg), ##__VA_ARGS__)
#define AWE_MODULE_INFO(module,msg, ...)  AWE_LOG_INFO(module_connect(module,msg), ##__VA_ARGS__)
#define AWE_MODULE_DEBUG(module,msg, ...) AWE_LOG_DEBUG(module_connect(module,msg), ##__VA_ARGS__)

void awe_log_error(const char *msg, ...);

void awe_log_warn(const char *msg, ...);

void awe_log_info(const char *msg, ...);

void awe_log_debug(const char *msg, ...);

void awe_log_set_priority(enum priority pri);

#endif //SS_METADATA_LOG_HEAD_H
