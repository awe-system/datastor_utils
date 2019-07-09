
#include <ctime>

#include "action_stub.h"

action_stub::action_stub() : snd_num(0), rcv_num(0)
{
//    AWE_MODULE_DEBUG("communicate", "action_stub::action_stub this %p", this);
    
    last_snd_timestamp.store(time(0));
    last_rcv_timestamp.store(time(0));
}

action_stub::action_stub(const action_stub &other)
{
//    AWE_MODULE_DEBUG("communicate", "action_stub::action_stub copy this %p", this);
    last_snd_timestamp.store(other.last_snd_timestamp);
    last_rcv_timestamp.store(other.last_rcv_timestamp);
    snd_num.store(other.snd_num);
    rcv_num.store(other.rcv_num);
}

void action_stub::mark_sent()
{
    snd_num++;
    last_snd_timestamp.store((unsigned long) time(0));
}

void action_stub::mark_received()
{
    rcv_num++;
    last_rcv_timestamp.store((unsigned long) time(0));
}

const unsigned long action_stub::get_time_frame_since_last_snd() const
{
    return (unsigned long) time(0) - last_snd_timestamp.load();
}

const unsigned long action_stub::get_time_frame_since_last_rcv() const
{
    return (unsigned long) time(0) - last_rcv_timestamp.load();
}

const unsigned long action_stub::get_snd_num() const
{
    return snd_num.load();
}

const unsigned long action_stub::get_rcv_num() const
{
    return rcv_num.load();
}

void action_stub::reset()
{
    last_snd_timestamp.store((unsigned long) time(0));
    last_rcv_timestamp.store((unsigned long) time(0));
    snd_num.store(0);
    rcv_num.store(0);
}

action_stub::~action_stub()
{
//    AWE_MODULE_DEBUG("communicate", "action_stub::~action_stub this %p", this);
}
