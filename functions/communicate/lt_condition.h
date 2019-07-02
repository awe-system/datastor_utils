#ifndef LT_CONDITION_H
#define LT_CONDITION_H

#include <condition_variable>
#include "lt_data/lt_data_t.h"

class lt_condition
{
    bool is_to_wait;
    std::mutex lock;
    std::condition_variable cond;
    int error;
    lt_data_t _data;
public:
    lt_condition() : is_to_wait(true), error(0)
    {
    }

    int wait()
    {
        std::unique_lock<std::mutex> lck(lock);
        while ( is_to_wait )
            cond.wait(lck);
        is_to_wait = true;
        return error;
    }

    void notify(const lt_data_t &data, int err)
    {
        error = err;
        if ( !error )
        {
            _data = data;
        }
        std::unique_lock<std::mutex> lck(lock);
        if(is_to_wait)
            cond.notify_one();
        is_to_wait = false;
    }
    
    void notify()
    {
        std::unique_lock<std::mutex> lck(lock);
        if(is_to_wait)
            cond.notify_one();
        is_to_wait = false;
        
    }

    const lt_data_t &get_data() const
    {
        return _data;
    }
};


#endif //SERVER_LT_CONDITION_H
