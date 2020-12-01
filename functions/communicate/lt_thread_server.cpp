#include <iostream>
#include "lt_thread_server.h"

lt_thread_single::lt_thread_single(boost::asio::io_service *io_service)
        : _io_service(io_service)
{
    th = new boost::thread(boost::bind(&lt_thread_single::run, this));
    cond.wait();
}

lt_thread_single::~lt_thread_single()
{
    th->join();
    delete th;
}

boost::asio::io_service *lt_thread_single::get_io_service()
{
    return _io_service;
}

void lt_thread_single::run()
{
    cond.notify();
    _io_service->run();
}

lt_thread_server::lt_thread_server(int thread_num) :
        _io_service(),_work(_io_service), curret_cnt(0)
{
    for ( int i = 0; i < thread_num; ++i )
    {
        lt_thread_single *t = new lt_thread_single(&_io_service);
        threads.push_back(t);
    }
}

boost::asio::io_service *lt_thread_server::get_io_service()
{
   return &_io_service;
}

lt_thread_server::~lt_thread_server()
{
    _io_service.stop();
    while ( !threads.empty() )
    {
        delete threads.back();
        threads.pop_back();
    }
}
