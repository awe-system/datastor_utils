#include <src/log4cpp_src/StringUtil.hh>
#include <boost/algorithm/string.hpp>
#include "awe_log.h"
#include "awe_conf/env.h"
#include "log4cpp/ss_log4cpp.h"

const int MAX_LOG_FILE_SIZE = 1024 * 1024 * 300;

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
                           error_category(log4cpp::Category::getInstance(
                                   std::string("error"))),
                           warn_category(log4cpp::Category::getInstance(
                                   std::string("warn"))),
                           info_category(log4cpp::Category::getInstance(
                                   std::string("info"))),
                           debug_category(log4cpp::Category::getInstance(
                                   std::string("debug")))
{
    static env    log_prefix("awe_log", "log_prefix");
    static env    log_level("awe_log", "log_level");
    static string log_path_prefix =
                          (log4cpp::StringUtil::trim(log_prefix.get_string()) ==
                           "") ? ("/var/log/msg") : log4cpp::StringUtil::trim(
                                  log_prefix.get_string());
    static string loglevel        =
                          (log4cpp::StringUtil::trim(log_level.get_string()) ==
                           "") ? ("info") : log4cpp::StringUtil::trim(
                                  log_level.get_string());
    
    log4cpp::PatternLayout *root_layout = new log4cpp::PatternLayout();
    root_layout->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S.%l} - %p] : %m%n");
    
    log4cpp::RollingFileAppender
            *roll_appender_all = new log4cpp::RollingFileAppender(
            "roll_appender", log_path_prefix + ".log", 1024 * 1024 * 100, 3);
    roll_appender_all->setLayout(root_layout);
    
    
    log4cpp::PatternLayout *layout_error = new log4cpp::PatternLayout();
    layout_error
            ->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S.%l} - %p] : %m%n");
    
    log4cpp::RollingFileAppender
            *roll_appender_error = new log4cpp::RollingFileAppender(
            "roll_appender_error", log_path_prefix + "_error.log",
			MAX_LOG_FILE_SIZE, 3);
    roll_appender_error->setLayout(layout_error);
    
    
    log4cpp::PatternLayout *layout_warn = new log4cpp::PatternLayout();
    layout_warn->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S.%l} - %p] : %m%n");
    
    log4cpp::RollingFileAppender
            *roll_appender_warn = new log4cpp::RollingFileAppender(
            "roll_appender_error", log_path_prefix + "_warn.log",
			MAX_LOG_FILE_SIZE, 3);
    roll_appender_warn->setLayout(layout_warn);
    
    
    log4cpp::PatternLayout *layout_info = new log4cpp::PatternLayout();
    layout_info->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S.%l} - %p] : %m%n");
    
    log4cpp::RollingFileAppender
            *roll_appender_info = new log4cpp::RollingFileAppender(
            "roll_appender_error", log_path_prefix + "_info.log",
			MAX_LOG_FILE_SIZE, 3);
    roll_appender_info->setLayout(layout_info);
    
    log4cpp::PatternLayout *layout_debug = new log4cpp::PatternLayout();
    layout_debug
            ->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S.%l} - %p] : %m%n");
    
    log4cpp::RollingFileAppender
            *roll_appender_debug = new log4cpp::RollingFileAppender(
            "roll_appender_error", log_path_prefix + "_debug.log",
			MAX_LOG_FILE_SIZE, 3);
    roll_appender_debug->setLayout(layout_debug);
    
    if ( loglevel == "error" )
    {
        root_category.setPriority(log4cpp::Priority::ERROR);
        cur_pri = LOG_PRIORITY_ERROR;
    }
    else if ( loglevel == "debug" )
    {
        root_category.setPriority(log4cpp::Priority::DEBUG);
        cur_pri = LOG_PRIORITY_DEBUG;
    }
    else if ( loglevel == "warning" )
    {
        root_category.setPriority(log4cpp::Priority::WARN);
        cur_pri = LOG_PRIORITY_WARN;
    }
    else
    {
        root_category.setPriority(log4cpp::Priority::INFO);
        cur_pri = LOG_PRIORITY_INFO;
    }
    root_category.addAppender(roll_appender_all);
}

ss_log4cpp::~ss_log4cpp()
{
    destroy();
}

void ss_log4cpp::error(std::string msg)
{
    std::string module;
    if ( is_msg_in_modulelist(msg, module) && !is_module_match(module) )
    {
        return;
    }
    root_category.error(msg);
    
}

void ss_log4cpp::warn(std::string msg)
{
    std::string module;
    if ( is_msg_in_modulelist(msg, module) && !is_module_match(module) )
    {
        return;
    }
    root_category.warn(msg);
}

void ss_log4cpp::info(std::string msg)
{
    std::string module;
    if ( is_msg_in_modulelist(msg, module) && !is_module_match(module) )
    {
        return;
    }
    root_category.info(msg);
}

void ss_log4cpp::debug(std::string msg)
{
    std::string module;
    if ( is_msg_in_modulelist(msg, module) && !is_module_match(module) )
    {
//        root_category.debug("ignore");
        return;
    }
//    root_category.debug("debug ----->msg:");
    root_category.debug(msg);
}

void ss_log4cpp::set_priority(int pri)
{
    switch ( pri )
    {
        case (LOG_PRIORITY_ERROR):
            root_category.setPriority(log4cpp::Priority::ERROR);
            cur_pri = LOG_PRIORITY_ERROR;
            break;
        case (LOG_PRIORITY_WARN):
            root_category.setPriority(log4cpp::Priority::WARN);
            cur_pri = LOG_PRIORITY_WARN;
            break;
        case (LOG_PRIORITY_INFO):
            root_category.setPriority(log4cpp::Priority::INFO);
            cur_pri = LOG_PRIORITY_INFO;
            break;
        case (LOG_PRIORITY_DEBUG):
            root_category.setPriority(log4cpp::Priority::DEBUG);
            cur_pri = LOG_PRIORITY_DEBUG;
            break;
        default:
            root_category.setPriority(log4cpp::Priority::DEBUG);
            cur_pri = LOG_PRIORITY_DEBUG;
            break;
    }
}

void ss_log4cpp::set_module_priority(const string &module, int pri)
{
    if ( LOG_PRIORITY_DEFAULT == pri )
    {
        try_remove_from_module_map(module);
        return;
    }
    try_add_to_module_map(module, pri);
}

void ss_log4cpp::try_remove_from_module_map(const string &module)
{
    auto it = module_map.find(module);
    if ( it != module_map.end() )
    {
        module_map.erase(it);
    }
}

void ss_log4cpp::try_add_to_module_map(const string &module, int pri)
{
    const auto &it = module_map.find(module);
    if ( it == module_map.end() )
    {
        module_map.insert(make_pair(module, pri));
    }
    else
    {
        it->second = pri;
    }
}

bool
ss_log4cpp::is_msg_in_modulelist(const std::string &msg, std::string &module)
{
    
//    root_category.debug(string("msg:[") + msg + string("]"));
    for ( auto it : module_map )
    {
//        root_category.debug(string("in for match module:[") + it.first + string("]"));
//        取出randcolor
        if ( boost::algorithm::starts_with(&msg[RANDCLOLORSIZE], it.first) )
        {
            module = it.first;
//            root_category.debug(string("in for match module:[") + module + string("]"));
            return true;
        }
        
    }
//    root_category.debug(string("not match msg:[") + msg + string("]"));
    return false;
}

bool ss_log4cpp::is_module_match(const std::string &module)
{
    int module_pri = module_map[module];
//    root_category.debug(string("module_pri:[") + to_string(module_pri) +
//                        string("]"));
//    root_category.debug(string("cur_pri:[") + to_string(cur_pri) + string("]"));
//    root_category.debug(string("module:[") + module + string("]"));
    
    if ( compare_priority(module_pri, cur_pri) >= 0 )
    { return false; }
    return true;
}

//A的优先级高于B时>0 相同时=0 A的优先级低于B时<0
int ss_log4cpp::compare_priority(int pri_a, int pri_b)
{
    assert(pri_a < LOG_PRIORITY_DEFAULT && pri_b < LOG_PRIORITY_DEFAULT);
    return pri_b - pri_a;
}

int ss_log4cpp::init_awe_log(const std::string &project)
{
    root_category.removeAllAppenders();

    static env    log_prefix_dest("awe_log", project.c_str());
    static env    log_level("awe_log", "log_level");
    if(log_prefix_dest.get_string().length() <= 0)
    {
        printf("conf %s empty!", project.c_str());
        return -1;
    }

    static string log_path_prefix =
            (log4cpp::StringUtil::trim(log_prefix_dest.get_string()) ==
             "") ? ("/var/log/msg") : log4cpp::StringUtil::trim(
                    log_prefix_dest.get_string());

    std::string size_conf = project + "_size";
    static env log_size("awe_log", size_conf.c_str());
    static int max_log_file_size = (log_size.get_int() == 0) ? (MAX_LOG_FILE_SIZE) : log_size.get_int();
    max_log_file_size = max_log_file_size * 1024 * 1024;

    std::string log_num_conf = project + "_num";
    static env log_num("awe_log", log_num_conf.c_str());
    static int max_log_num = (log_num.get_int() == 0) ? 3 : log_num.get_int();

    static string loglevel = (log4cpp::StringUtil::trim(log_level.get_string()) == "") ?
            ("info") : log4cpp::StringUtil::trim(log_level.get_string());

    log4cpp::PatternLayout *root_layout = new log4cpp::PatternLayout();
    root_layout->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S.%l} - %p] : %m%n");

    log4cpp::RollingFileAppender
            *roll_appender_all = new log4cpp::RollingFileAppender(
            "roll_appender", log_path_prefix + ".log", max_log_file_size, max_log_num);
    roll_appender_all->setLayout(root_layout);


    log4cpp::PatternLayout *layout_error = new log4cpp::PatternLayout();
    layout_error
            ->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S.%l} - %p] : %m%n");

    log4cpp::RollingFileAppender
            *roll_appender_error = new log4cpp::RollingFileAppender(
            "roll_appender_error", log_path_prefix + "_error.log",
            MAX_LOG_FILE_SIZE, 3);
    roll_appender_error->setLayout(layout_error);


    log4cpp::PatternLayout *layout_warn = new log4cpp::PatternLayout();
    layout_warn->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S.%l} - %p] : %m%n");

    log4cpp::RollingFileAppender
            *roll_appender_warn = new log4cpp::RollingFileAppender(
            "roll_appender_error", log_path_prefix + "_warn.log",
            MAX_LOG_FILE_SIZE, 3);
    roll_appender_warn->setLayout(layout_warn);


    log4cpp::PatternLayout *layout_info = new log4cpp::PatternLayout();
    layout_info->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S.%l} - %p] : %m%n");

    log4cpp::RollingFileAppender
            *roll_appender_info = new log4cpp::RollingFileAppender(
            "roll_appender_error", log_path_prefix + "_info.log",
            MAX_LOG_FILE_SIZE, 3);
    roll_appender_info->setLayout(layout_info);

    log4cpp::PatternLayout *layout_debug = new log4cpp::PatternLayout();
    layout_debug
            ->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S.%l} - %p] : %m%n");

    log4cpp::RollingFileAppender
            *roll_appender_debug = new log4cpp::RollingFileAppender(
            "roll_appender_error", log_path_prefix + "_debug.log",
            MAX_LOG_FILE_SIZE, 3);
    roll_appender_debug->setLayout(layout_debug);

    if ( loglevel == "error" )
    {
        root_category.setPriority(log4cpp::Priority::ERROR);
        cur_pri = LOG_PRIORITY_ERROR;
    }
    else if ( loglevel == "debug" )
    {
        root_category.setPriority(log4cpp::Priority::DEBUG);
        cur_pri = LOG_PRIORITY_DEBUG;
    }
    else if ( loglevel == "warning" )
    {
        root_category.setPriority(log4cpp::Priority::WARN);
        cur_pri = LOG_PRIORITY_WARN;
    }
    else
    {
        root_category.setPriority(log4cpp::Priority::INFO);
        cur_pri = LOG_PRIORITY_INFO;
    }
    root_category.addAppender(roll_appender_all);
    return 0;
}
