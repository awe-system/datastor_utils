#ifndef LT_THREAD_SERVER_H
#define LT_THREAD_SERVER_H

#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>

class lt_thread_server
{
private:
    boost::asio::io_service _io_service;
    boost::asio::io_service::work _work;
    std::vector<boost::shared_ptr<boost::thread> > threads;

public:
    lt_thread_server(int thread_num);
    boost::asio::io_service *get_io_service();
    ~lt_thread_server();
};


#endif //SERVER_LT_THREAD_SERVER_H
