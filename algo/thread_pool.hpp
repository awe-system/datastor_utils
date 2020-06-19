#ifndef NBD_THREAD_POOL_H
#define NBD_THREAD_POOL_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>

class thread_pool
{
private:
    boost::thread_group group;
    boost::asio::io_service io_service_;
    boost::asio::io_service::work work_;

public:
    thread_pool(int max_thread_num) : io_service_(), work_(io_service_)
    {
        for ( int i = 0; i < max_thread_num; ++i )
        {
            group.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
        }
    }

    template<typename FUNC>
    void submit_task(FUNC func_)
    {
        io_service_.post(func_);
    }

    ~thread_pool()
    {
        io_service_.stop();
        group.join_all();
    }


};


#endif
