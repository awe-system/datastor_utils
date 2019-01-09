#include <iostream>
#include "lt_thread_server.h"

lt_thread_single::lt_thread_single()
        : _io_service(), _work(_io_service), is_start(false)
{
    th = new boost::thread(boost::bind(&lt_thread_single::run, this));
    while(!is_start) usleep(1);
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
    is_start = true;
    std::cout<<"run"<<std::endl;
    _io_service.run();
}

lt_thread_server::lt_thread_server(int thread_num) : max(thread_num), curret_cnt(0)
{
    pthread_spin_init(&splck, PTHREAD_PROCESS_PRIVATE);
    for ( int i = 0; i < thread_num; ++i )
    {
        lt_thread_single *t = new lt_thread_single();
        threads.push_back(t);
    }
}

boost::asio::io_service *lt_thread_server::get_io_service()
{
    int i = 0;
    do
    {
        pthread_spin_lock(&splck);
        if ( ++curret_cnt == max )
        { curret_cnt = 0; }
        i = curret_cnt;
        pthread_spin_unlock(&splck);
    } while ( 0 );
    
    return threads[i]->get_io_service();
}

lt_thread_server::~lt_thread_server()
{
    for ( lt_thread_single *t : threads )
    {
        delete t;
    }
    pthread_spin_destroy(&splck);
}
