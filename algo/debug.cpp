//
// Created by root on 17-12-5.
//

#include <iostream>
#include "debug.h"


#define RED 	"\033[31;4m"
#define RESET	"\033[0m"
#define GREEN	"\033[36;4m"
#define YELLOW	"\033[32;4m"
#define PURPLE	"\033[35;4m"
#define BLUE	"\033[34;4m"
#define BGREEN	"\033[1;36;47;4m"
#define BRED 	"\033[1;31;47;4m"
#define BYELLOW	"\033[1;32;47;4m"
#define BPURPLE	"\033[1;35;47;4m"
#define BBLUE	"\033[1;34;47;4m"
#define NORMAL	"\033[30;4m"


debug &  debug::operator<<(const std::string &out)
{
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
    end_str = RESET;
    switch (color)
    {
        case color_normal:
            start_str=NORMAL;
            end_str = string("");
            break;
        case color_blue:
            start_str = BLUE;
            break;
        case color_red:
            start_str = RED;
            break;
        case color_green:
            start_str = GREEN;
            break;
        case color_purple:
            start_str = PURPLE;
            break;
        case color_yellow:
            start_str = YELLOW;
            break;
        case color_bgreen:
            start_str = BGREEN;
            break;
        case color_bblue:
            start_str = BLUE;
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
    set_color(color);
}

debug &debug::operator<<(const dbg_end_type &end)
{
    set_color(color_normal);
    cout<<DEBUG_END;
}

debug &debug::operator<<(const debug &debug1)
{
    set_color(color_normal);
    cout<<DEBUG_END;
}
