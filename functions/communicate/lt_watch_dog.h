#ifndef _COMMUNICATE_LT_WATCH_DOG_H
#define _COMMUNICATE_LT_WATCH_DOG_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <mutex>
#include <condition_variable>

class lt_watch_dog
{
private:
    std::mutex m;
    int seconds;
    bool is_monitoring;
    boost::asio::deadline_timer timer;

    std::atomic<bool> on_monitor;
    std::mutex pending_m;
    std::condition_variable pending_cond;
public:
    lt_watch_dog(boost::asio::io_service *_io_service);

    void set_seconds(int sec);

    ~lt_watch_dog();
public:
    void start_monitor();

    void stop_monitor();

public:
    //看门狗定期检查调用check_feed 发现返回指为false时发送消息并且关闭监听
    virtual bool is_to_feed() const = 0;

    virtual void handle_event() = 0;

private:
    void timer_handler(const boost::system::error_code &error);

    void start_timer();
};


#endif //TEST_COMMUNICATE_LT_WATCH_DOG_H
