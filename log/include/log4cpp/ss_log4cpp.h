#ifndef SS_METADATA_SS_LOG_H
#define SS_METADATA_SS_LOG_H

#include "log4cpp/Category.hh"
#include "log4cpp/PatternLayout.hh"
#include "log4cpp/RollingFileAppender.hh"

class ss_log4cpp
{
public:
    static ss_log4cpp &getInstance();

    static void destroy();

    void error(std::string msg);

    void warn(std::string msg);

    void debug(std::string msg);

    void info(std::string msg);

private:
    ss_log4cpp();
    ~ss_log4cpp();
private:
    static ss_log4cpp *ss_log_;
    log4cpp::Category &root_category;
    log4cpp::Category &error_category;
    log4cpp::Category &warn_category;
    log4cpp::Category &info_category;
    log4cpp::Category &debug_category;
};

#endif //SS_METADATA_SS_LOG_H
