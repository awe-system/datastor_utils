#ifndef _COMMUNICATE_LT_SAFE_STATE_H
#define _COMMUNICATE_LT_SAFE_STATE_H

#include <boost/function.hpp>
#include <list>
#include <atomic>
#include <mutex>

template<typename T>
class lt_safe_state
{
private:
    std::list<boost::function<void(const T &)>> callback_list;
    std::mutex m;
    std::atomic<T> cur_state;
public:
    lt_safe_state(const T &init_stat)
    {
        cur_state.store(init_stat);
    }

    lt_safe_state(const T &init_stat, boost::function<void(const T &)> state_changed)
    {
        cur_state.store(init_stat);
        callback_list.push_back(state_changed);
    }

    void register_monitor(boost::function<void(const T &)> state_changed)
    {
        std::lock_guard<std::mutex> lck(m);
        callback_list.push_back(state_changed);
    }

    void set(const T &stat)
    {
        std::unique_lock<std::mutex> lock(m);
        if ( stat == cur_state.load())
            return;
        cur_state.store(stat);
        bool tmp_state = cur_state.load();

        for ( boost::function<void(const T &)> st_changed : callback_list )
        {
            lock.unlock();
            st_changed(tmp_state);
            lock.lock();
        }
    }

    T get() const
    {
        return cur_state.load();
    }
};


#endif
