//
// Created by root on 17-12-5.
//

#include <iostream>
#include <chrono>
#include<iomanip>
#include<ctime>
#include "debug.h"

#include <boost/date_time/time_clock.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


#define AWE_RED 	"\033[31;0m"
#define AWE_RESET	"\033[0m"
#define AWE_GREEN	"\033[36;0m"
#define AWE_YELLOW	"\033[32;0m"
#define AWE_PURPLE	"\033[35;0m"
#define AWE_BLUE	"\033[34;0m"
#define BGREEN	"\033[1;36;47;0m"
#define BRED 	"\033[1;31;47;0m"
#define BYELLOW	"\033[1;32;47;0m"
#define BPURPLE	"\033[1;35;47;0m"
#define BBLUE	"\033[1;34;47;0m"
#define AWE_NORMAL	"\033[30;0m"


debug &  debug::operator<<(const std::string &out)
{
    std::unique_lock<std::mutex> lck(m);
#ifndef START_DEBUG
    return *this;
#endif
    if(!is_start)return *this;
    cout<<start_str;
    cout<<out;
    cout<<end_str;
    return *this;
}

debug &  debug::operator<<(const int out)
{
    std::unique_lock<std::mutex> lck(m);
#ifndef START_DEBUG
    return *this;
#endif
    if(!is_start)return *this;
    cout<<start_str;
    cout<<out;
    cout<<end_str;
    return *this;
}

debug &  debug::operator<<(const char *out)
{
    std::unique_lock<std::mutex> lck(m);
#ifndef START_DEBUG
    return *this;
#endif
    if(!is_start)return *this;
    cout<<start_str;
    cout<<out;
    cout<<end_str;
    return *this;
}

debug &  debug::operator<<(const json_obj &out)
{
    std::unique_lock<std::mutex> lck(m);
#ifndef START_DEBUG
    return *this;
#endif
    if(!is_start)return *this;
    cout<<start_str;
    cout<<out.dumps();
    cout<<end_str;
    return *this;
}

debug &  debug::operator<<(const long out)
{
    std::unique_lock<std::mutex> lck(m);
#ifndef START_DEBUG
    return *this;
#endif
    if(!is_start)return *this;
    cout<<start_str;
    cout<<out;
    cout<<end_str;
    return *this;
}

debug &debug::operator<<(const unsigned long out)
{
    std::unique_lock<std::mutex> lck(m);
#ifndef START_DEBUG
    return *this;
#endif
    if(!is_start)return *this;
    cout<<start_str;
    cout<<out;
    cout<<end_str;
    return *this;
}

debug &debug::operator<<(const unsigned int out)
{
    std::unique_lock<std::mutex> lck(m);
#ifndef START_DEBUG
    return *this;
#endif
    if(!is_start)return *this;
    cout<<start_str;
    cout<<out;
    cout<<end_str;
    return *this;
}

debug &debug::operator<<(const void *out)
{
    std::unique_lock<std::mutex> lck(m);
#ifndef START_DEBUG
    return *this;
#endif
    if(!is_start)return *this;
    cout<<start_str;
    printf("%p",out);
    cout<<end_str;
    return *this;
}

debug &debug::operator<<(const char out)
{
    std::unique_lock<std::mutex> lck(m);
#ifndef START_DEBUG
    return *this;
#endif
    if(!is_start)return *this;
    cout<<start_str;
    cout<<out;
    cout<<end_str;
    return *this;
}

debug::debug(color_type color)
{
    stop();
    set_color(color);
}

void debug::set_color(color_type color)
{
    end_str = AWE_RESET;
    switch (color)
    {
        case color_normal:
            start_str=AWE_NORMAL;
            end_str = string("");
            break;
        case color_blue:
            start_str = AWE_BLUE;
            break;
        case color_red:
            start_str = AWE_RED;
            break;
        case color_green:
            start_str = AWE_GREEN;
            break;
        case color_purple:
            start_str = AWE_PURPLE;
            break;
        case color_yellow:
            start_str = AWE_YELLOW;
            break;
        case color_bgreen:
            start_str = BGREEN;
            break;
        case color_bblue:
            start_str = AWE_BLUE;
            break;
        case color_bred:
            start_str = BRED;
            break;
        case color_bpurple:
            start_str = BPURPLE;
            break;
        case color_byellow:
            start_str = BYELLOW;
            break;
    }

}

debug::debug()
{
    stop();
    set_color(color_normal);
}



debug &debug::operator<<(const lt_data_t &out)
{
    std::unique_lock<std::mutex> lck(m);
#ifndef START_DEBUG
    return *this;
#endif
    if(!is_start)return *this;
    cout<<start_str;
    cout<<"len:"<<out._length<<endl;
    cout<<"buf:"<<endl;
    unsigned  char * buf = out.get_buf();
    long i = 0;
    if(out._length <= 1024)
    {
        for(i = 0; i<out._length; ++i)
        {
            printf("%02x ",buf[i]);
            if(i%32 == 31) cout<<endl;
        }
    }
    else
    {
        for(i = 0; i<512; ++i)
        {
            printf("%02x ",buf[i]);
            if(i%32 == 31) cout<<endl;
        }

        for(long i = 0; i<32; ++i)
        {
            printf("-- ");
            if(i%32 == 31) cout<<endl;
        }
        for(i = out._length-512; i<out._length; ++i)
        {
            printf("%02x ",buf[i]);
            if(i%32 == 31) cout<<endl;
        }
    }

    cout<<endl;
    cout<<end_str;
    return *this;
}

void debug::start()
{
    is_start = true;
}

void debug::stop()
{
    is_start = false;
}

debug &debug::operator<<(const std::vector<void *> &stack_out)
{
    std::unique_lock<std::mutex> lck(m);
#ifndef START_DEBUG
    return *this;
#endif
    if(!is_start)return *this;
    cout<<start_str;
    cout<<"stack addr:"<<&stack_out<<endl;
    int i=0;
    for(std::vector<void *>::const_iterator it = stack_out.begin();
            it!= stack_out.end();
            ++it)
    {
        printf("level %d\t:%p\n",i++,*it);
    }

    cout<<endl;
    cout<<end_str;
    return *this;
}

debug &debug::operator<<(const color_type &color)
{
    std::unique_lock<std::mutex> lck(m);
    set_color(color);
    return *this;
}

debug &debug::operator<<(const dbg_end_type &end)
{
    std::unique_lock<std::mutex> lck(m);
    set_color(color_normal);
    
    cout<<dbg_now_time()<<DEBUG_END;
    return *this;
}

debug &debug::operator<<(const debug &debug1)
{
    std::unique_lock<std::mutex> lck(m);
    set_color(color_normal);
    cout<<DEBUG_END;
    return *this;
}

std::string dbg_now_time()
{
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
    string now_str = to_iso_extended_string(now.date()) + " " + to_simple_string(now.time_of_day());
    return string("[") + now_str + string("]");
}
