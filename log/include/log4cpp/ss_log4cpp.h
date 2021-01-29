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
    
    void set_priority(int pri);

//约定在打log级别里将某个模块的日志界别降低
    void set_module_priority(const std::string &module, int pri);

    int init_awe_log(const std::string &project);
private:
    ss_log4cpp();
    
    ~ss_log4cpp();

private:
    //A的优先级高于B时>0 相同时=0 A的优先级低于B时<0
    int compare_priority(int pri_a, int pri_b);
    
    bool is_module_match(const std::string &module);
    
    //日志信息受到规则约束
    bool is_msg_in_modulelist(const std::string &msg, std::string &module);
    
    void try_remove_from_module_map(const std::string &module);
    
    void try_add_to_module_map(const std::string &module, int pri);

private:
    /** 约定约定在打log级别里将某个模块的日志界别降低《黑名单》 **/
    int cur_pri;
    std::map<std::string, int> module_map;
    static ss_log4cpp          *ss_log_;
    log4cpp::Category          &root_category;
    log4cpp::Category          &error_category;
    log4cpp::Category          &warn_category;
    log4cpp::Category          &info_category;
    log4cpp::Category          &debug_category;
};

#endif //SS_METADATA_SS_LOG_H
