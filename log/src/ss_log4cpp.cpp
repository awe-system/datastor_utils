#include "awe_log.h"
#include "awe_conf/env.h"
#include "log4cpp/ss_log4cpp.h"

static env log_prefix("awe_log","log_prefix");
static env log_level("awe_log","log_level");
static string log_path_prefix = (log_prefix.get_string() == "")?("/var/log/msg"):log_prefix.get_string();
static string loglevel = (log_level.get_string() == "")?("info"):log_level.get_string();
ss_log4cpp &logger = ss_log4cpp::getInstance();

ss_log4cpp *ss_log4cpp::ss_log_ = NULL;

ss_log4cpp &ss_log4cpp::getInstance()
{
    if ( ss_log_ == NULL )
    {
        ss_log_ = new ss_log4cpp;
    }
    return *ss_log_;
}

void ss_log4cpp::destroy()
{
    if ( ss_log_ )
    {
        ss_log_->root_category.info("ss_log4cpp destroy");
        ss_log_->error_category.shutdown();
        ss_log_->root_category.shutdown();
        delete ss_log_;
    }
}

ss_log4cpp::ss_log4cpp() : root_category(log4cpp::Category::getRoot()),
                           error_category(log4cpp::Category::getInstance(std::string("error"))),
                           warn_category(log4cpp::Category::getInstance(std::string("warn"))),
                           info_category(log4cpp::Category::getInstance(std::string("info"))),
                           debug_category(log4cpp::Category::getInstance(std::string("debug")))
{
    log4cpp::PatternLayout *root_layout = new log4cpp::PatternLayout();
    root_layout->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S.%l} - %p] : %m%n");

    log4cpp::RollingFileAppender *roll_appender_all = new log4cpp::RollingFileAppender(
            "roll_appender", log_path_prefix+ ".log", 1024*1024*100, 3);
    roll_appender_all->setLayout(root_layout);


    log4cpp::PatternLayout *layout_error = new log4cpp::PatternLayout();
    layout_error->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S.%l} - %p] : %m%n");

    log4cpp::RollingFileAppender *roll_appender_error = new log4cpp::RollingFileAppender(
            "roll_appender_error", log_path_prefix + "_error.log", 1024*1024*100, 3);
    roll_appender_error->setLayout(layout_error);


    log4cpp::PatternLayout *layout_warn = new log4cpp::PatternLayout();
    layout_warn->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S.%l} - %p] : %m%n");

    log4cpp::RollingFileAppender *roll_appender_warn = new log4cpp::RollingFileAppender(
            "roll_appender_error", log_path_prefix + "_warn.log", 1024*1024*100, 3);
    roll_appender_warn->setLayout(layout_warn);


    log4cpp::PatternLayout *layout_info = new log4cpp::PatternLayout();
    layout_info->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S.%l} - %p] : %m%n");

    log4cpp::RollingFileAppender *roll_appender_info = new log4cpp::RollingFileAppender(
            "roll_appender_error", log_path_prefix + "_info.log", 1024*1024*100, 3);
    roll_appender_info->setLayout(layout_info);

    log4cpp::PatternLayout *layout_debug = new log4cpp::PatternLayout();
    layout_debug->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S.%l} - %p] : %m%n");

    log4cpp::RollingFileAppender *roll_appender_debug = new log4cpp::RollingFileAppender(
            "roll_appender_error", log_path_prefix + "_debug.log", 1024*1024*100, 3);
    roll_appender_debug->setLayout(layout_debug);
    
    if(loglevel == "error")
    {
        root_category.setPriority(log4cpp::Priority::ERROR);
    }
    else if(loglevel == "debug")
    {
        root_category.setPriority(log4cpp::Priority::DEBUG);
    }
    else if(loglevel == "warning")
    {
        root_category.setPriority(log4cpp::Priority::WARN);
    }
    else
    {
        root_category.setPriority(log4cpp::Priority::INFO);
    }
    root_category.addAppender(roll_appender_all);
}

ss_log4cpp::~ss_log4cpp()
{
    destroy();
}

void ss_log4cpp::error(std::string msg)
{
    root_category.error(msg);
}

void ss_log4cpp::warn(std::string msg)
{
    root_category.warn(msg);
}

void ss_log4cpp::info(std::string msg)
{
    root_category.info(msg);
}

void ss_log4cpp::debug(std::string msg)
{
    root_category.debug(msg);
}

void ss_log4cpp::set_priority(int pri)
{
    switch ( pri )
    {
        case (LOG_PRIORITY_ERROR):
            root_category.setPriority(log4cpp::Priority::ERROR);
            break;
        case (LOG_PRIORITY_WARN):
            root_category.setPriority(log4cpp::Priority::WARN);
            break;
        case (LOG_PRIORITY_INFO):
            root_category.setPriority(log4cpp::Priority::INFO);
            break;
        case (LOG_PRIORITY_DEBUG):
            root_category.setPriority(log4cpp::Priority::DEBUG);
            break;
        default:
            root_category.setPriority(log4cpp::Priority::DEBUG);
            break;
    }
}
