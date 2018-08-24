#include "lt_thread_server.h"

lt_thread_server::lt_thread_server(int thread_num) : _io_service(), _work(_io_service)
{
    for ( int i = 0; i < thread_num; ++i )
    {
        boost::shared_ptr<boost::thread> thread = boost::shared_ptr<boost::thread>(
                new boost::thread(boost::bind(&boost::asio::io_service::run, &_io_service)));
        threads.push_back(thread);
    }
}

boost::asio::io_service *lt_thread_server::get_io_service()
{
    return &_io_service;
}

lt_thread_server::~lt_thread_server()
{
    _io_service.stop();

    for ( boost::shared_ptr<boost::thread> t : threads )
    {
        t->join();
    }

}
