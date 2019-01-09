#include <iostream>
#include "lt_thread_server.h"

lt_thread_single::lt_thread_single()
        : _io_service(), _work(_io_service)
{
    th = new boost::thread(boost::bind(&lt_thread_single::run, this));
    cond.wait();
}

lt_thread_single::~lt_thread_single()
{
    _io_service.stop();
    th->join();
    delete th;
}

boost::asio::io_service *lt_thread_single::get_io_service()
{
    return &_io_service;
}

void lt_thread_single::run()
{
    cond.notify();
    _io_service.run();
}

lt_thread_server::lt_thread_server(int thread_num) : curret_cnt(0),
                                                     splck()
{
    for ( int i = 0; i < thread_num; ++i )
    {
        lt_thread_single *t = new lt_thread_single();
        threads.push_back(t);
    }
}

boost::asio::io_service *lt_thread_server::get_io_service()
{
    boost::lock_guard<boost::detail::spinlock> lck(splck);
    if(threads.empty()) return nullptr;
    if ( ++curret_cnt == threads.size() )
    { curret_cnt = 0; }
    return threads[curret_cnt]->get_io_service();
}

lt_thread_server::~lt_thread_server()
{
    boost::lock_guard<boost::detail::spinlock> lck(splck);
    while(!threads.empty())
    {
        delete threads.back();
        threads.pop_back();
    }
}
