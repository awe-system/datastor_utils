//
// Created by root on 17-12-5.
//

#ifndef SS_METADATA_DEBUG_H
#define SS_METADATA_DEBUG_H

#include <mutex>
#include <string>
#include "lt_data/json_obj.h"
#include "lt_data/lt_data_t.h"

#define  DEBUG_END "\n"
#define START_DEBUG

typedef enum
{
    color_red,
    color_green,
    color_blue,
    color_yellow,
    color_purple,
    color_bred,
    color_bgreen,
    color_bblue,
    color_byellow,
    color_bpurple,
    color_normal,
}color_type;

class dbg_end_type
{
};

extern dbg_end_type end_dbg;

std::string dbg_now_time();

#define end_dbgl (dbg<<color_bblue<<__FILE__<<":"<<__LINE__<<color_red<<":"\
                    <<color_yellow<<dbg_now_time()<<color_normal<<" ")

class debug
{
    std::mutex m;
    string start_str;
    string end_str;
    bool is_start;
public:
    void start();
    void stop();
    debug();
    debug(color_type color);
    void set_color(color_type color);
    debug & operator <<(const debug & debug1);
    debug & operator <<(const std::string & out);
    debug &  operator <<(const lt_data_t& out);
    debug &  operator <<(const color_type& color);
    debug &  operator <<(const dbg_end_type& end);
    debug &  operator <<(const char out);
    debug &  operator <<(const void* out);
    debug &  operator <<(const int out);
    debug &  operator <<(const long out);
    debug &  operator <<(const unsigned long out);
    debug &  operator <<(const unsigned int out);
    debug &  operator <<(const char *out);
    debug &  operator <<(const json_obj & obj);
    debug &  operator <<(const std::vector<void *> & stack_out);
};

extern debug dbg;

#endif //SS_METADATA_DEBUG_H
