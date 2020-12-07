#ifndef SS_NBD_TEST_DEVICE_H
#define SS_NBD_TEST_DEVICE_H

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <libaio.h>
#include <string>
#include <condition_variable>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>

#include <lt_function/thread_pool.hpp>

#define ALIGN_SIZE  4096

typedef std::function<void(void* ,int)> io_done_callback;

class libaio_device_service;
class event_ctx
{
public:
    void *pri;
    unsigned char *buf_ptr;    //保存request buf
    bool is_read;
    struct iocb *cb;
public:
    event_ctx(void *pri, bool is_read, struct iocb *cb, unsigned char *buf) : pri(pri), buf_ptr(buf), is_read(is_read), cb(cb)
    {
    }
};

class libaio_device
{
private:
    unsigned long dev_fd;
    std::string dev_path;
    int event_fd;
    io_done_callback io_cb_;
    io_context_t *libaio_context;
    std::atomic_int pending_size;
    libaio_device_service *device_service;
    
    std::mutex mtx_;
public:
    libaio_device(std::string dev_path, int max_event_num,
            libaio_device_service *device_service, io_done_callback io_cb);

    void set_open_buff();

    int open();

    void close();

    int get_event_fd();

    void put_pending_size();

    int obtain_pending_size();

    void async_read(unsigned long offset, unsigned int len, unsigned char *buf, void *pri);

    void async_write(unsigned long offset, unsigned int len, unsigned char *buf, void *pri);

    std::string &obtain_dev_path();

    void get_io();
    ~libaio_device();

private:
    bool is_buff_io = false;
    void get_pending_size();
};

#endif //SS_NBD_TEST_DEVICE_H
