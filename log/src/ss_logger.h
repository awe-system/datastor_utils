#ifndef SS_LOG_SS_LOGGER_H
#define SS_LOG_SS_LOGGER_H

#include <ss_log.h>
#include "log4cpp/Category.hh"

class ss_logger
{
private:
    log4cpp::Category &root_category;
    log4cpp::Category &error_category;

public:
    ss_logger(priority pri, const std::string log_path, int log_file_size, int log_file_num);
    void set_priority(priority pri);
};


#endif //SS_LOG_SS_LOGGER_H
