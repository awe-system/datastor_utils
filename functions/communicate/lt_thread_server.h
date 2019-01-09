#ifndef LT_THREAD_SERVER_H
#define LT_THREAD_SERVER_H

#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>


class lt_thread_single
{
    boost::asio::io_service _io_service;
    boost::asio::io_service::work _work;
    boost::thread* th;
    bool is_start;
public:
    void run();
public:
    lt_thread_single();
    boost::asio::io_service *get_io_service();
    ~lt_thread_single();
};

class lt_thread_server
{
private:
    boost::detail::spinlock splck;
    int curret_cnt;
    int max;
    std::vector<lt_thread_single *> threads;
public:
    lt_thread_server(int thread_num);
    boost::asio::io_service *get_io_service();
    ~lt_thread_server();
};


#endif //SERVER_LT_THREAD_SERVER_H
