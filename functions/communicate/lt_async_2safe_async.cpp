#include "lt_async_2safe_async.h"

func_pair::func_pair(boost::function<void()> _f, boost::function<void()> _callback_f) : f(_f), callback_f(_callback_f)
{
}

func_pair::func_pair(const func_pair &other)
{
    f = other.f;
    callback_f = other.callback_f;
}


void lt_async_2safe_async::begin_to(boost::function<void()> f, boost::function<void()> callback_f)
{
    std::unique_lock<std::mutex> lock(f_m);
    if ( is_doing )
    {
        func_pair pair(f, callback_f);
        func_list.push_back(pair);
        return;
    }
    else
    {
        is_doing = true;
        f();
        lock.unlock();
    }
}

void lt_async_2safe_async::continue_to()
{
    std::unique_lock<std::mutex> lock(f_m);
    is_doing = false;
    if ( func_list.empty())
    {
        return;
    }
    else
    {
        func_pair funcs = func_list.front();
        func_list.pop_front();
        is_doing = true;
        funcs.f();
        lock.unlock();
    }
}

void lt_async_2safe_async::clear()
{
    std::unique_lock<std::mutex> lock(f_m);
    std::list<func_pair> tmp_list;
    tmp_list.assign(func_list.begin(), func_list.end());
    func_list.clear();
    lock.unlock();
    for ( func_pair funcs : tmp_list )
    {
        funcs.callback_f();
    }
}


lt_async_2safe_async::lt_async_2safe_async() : is_doing(false)
{

}