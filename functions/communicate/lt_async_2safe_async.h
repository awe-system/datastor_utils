#ifndef _COMMUNICATE_LT_ASYNC_2SAFE_ASYNC_H
#define _COMMUNICATE_LT_ASYNC_2SAFE_ASYNC_H

#include <iostream>
#include <boost/function.hpp>
#include <mutex>
#include <list>

/*
 * 将一个不安全的异步经过队列处理变成一个安全异步的类
 * 保证一个接口到他的回调中间的过程和再次调用这个接口没有冲突
 * 构造时使用一个func和异步回调
 *
 * */
class func_pair
{
public:
    boost::function<void()> f;
    boost::function<void()> callback_f;
public:
    func_pair(boost::function<void()> _f, boost::function<void()> _callback_f);

    func_pair(const func_pair &other);

};

class lt_async_2safe_async
{
private:
    std::mutex f_m;
    bool is_doing;
    std::list<func_pair> func_list;
public:
    lt_async_2safe_async();

public:
    bool is_empty();
    
    void begin_to(boost::function<void()> f, boost::function<void()> callback_f);

    void continue_to();

    void clear();
};


#endif //TEST_COMMUNICATE_LT_ASYNC_2SAFE_ASYNC_H
