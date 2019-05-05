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

#define LOG_ERROR(msg, ...) ss_log_error(postfix(msg), ##__VA_ARGS__)
#define LOG_WARN(msg, ...)  ss_log_warn(postfix(msg), ##__VA_ARGS__)
#define LOG_INFO(msg, ...)  ss_log_info(postfix(msg), ##__VA_ARGS__)
#define LOG_DEBUG(msg, ...) ss_log_debug(postfix(msg), ##__VA_ARGS__)

void ss_log_error(const char *msg, ...);

void ss_log_warn(const char *msg, ...);

void ss_log_info(const char *msg, ...);

void ss_log_debug(const char *msg, ...);

void ss_log_set_priority(enum priority pri);

#endif //SS_METADATA_LOG_HEAD_H
