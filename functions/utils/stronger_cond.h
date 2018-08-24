#ifndef _STRONGER_COND_H
#define _STRONGER_COND_H

#include <atomic>
#include <mutex>
#include <condition_variable>

class stronger_cond
{
private:
    bool is_to_wait;
    std::mutex lock;
    std::condition_variable cond;

    std::atomic<bool> is_to_wait_ext;

public:
    stronger_cond() : is_to_wait(true)
    {
        is_to_wait_ext.store(true);
    }

    void wait()
    {
        std::unique_lock<std::mutex> lck(lock);
        while ( is_to_wait )
        {
            cond.wait(lck);
        }

        is_to_wait = true;
    }

    void notify()
    {
        std::unique_lock<std::mutex> lck(lock);
        is_to_wait = false;
        cond.notify_one();
    }

};


#endif