#include <log4cpp/PatternLayout.hh>
#include <log4cpp/RollingFileAppender.hh>
#include "ss_logger.h"

ss_logger::ss_logger(priority pri, const std::string log_path, int log_file_size, int log_file_num) :
        root_category(log4cpp::Category::getRoot()),
        error_category(log4cpp::Category::getInstance(std::string("error")))
{
    log4cpp::PatternLayout *layout = new log4cpp::PatternLayout();
    layout->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S.%l} - %p] (%c) : %m%n");

    log4cpp::RollingFileAppender *roll_appender_all = new log4cpp::RollingFileAppender(
            "roll_appender", log_path + ("/ss_log.log"), 1024 * 1024 * 100, 3);
    roll_appender_all->setLayout(layout);

    log4cpp::RollingFileAppender *roll_appender_error = new log4cpp::RollingFileAppender(
            "roll_appender_error", log_path + "/ss_error_log.log", 1024 * 1024 * 100, 3);
    roll_appender_error->setLayout(layout);

    set_priority(pri);
    root_category.addAppender(roll_appender_all);

    error_category.setPriority(log4cpp::Priority::ERROR);
    error_category.setAppender(roll_appender_error);
}

void ss_logger::set_priority(priority pri)
{
    switch ( pri )
    {
        case (ERROR):
            root_category.setPriority(log4cpp::Priority::ERROR);
            break;
        case (WARN):
            root_category.setPriority(log4cpp::Priority::WARN);
            break;
        case (INFO):
            root_category.setPriority(log4cpp::Priority::INFO);
            break;
        case (DEBUG):
            root_category.setPriority(log4cpp::Priority::DEBUG);
            break;
        default:
            root_category.setPriority(log4cpp::Priority::DEBUG);
            break;
    }
}
