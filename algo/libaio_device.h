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

#include "thread_pool.hpp"

#define ALIGN_SIZE  512

class io_done_callback
{
public:
    virtual void write_done(void *pri, int error) = 0;

    virtual void read_done(void *pri, int error) = 0;
};
class libaio_device_service;
class event_ctx
{
public:
    void *pri;
    unsigned char *buf_ptr;    //保存request buf
    bool is_read;
    struct iocb *cb;
public:
    event_ctx(void *pri, bool is_read, struct iocb *cb, unsigned char *buf) : pri(pri), is_read(is_read), cb(cb), buf_ptr(buf)
    {
    }
};


class libaio_device
{
public:
    std::mutex mt;
private:
    unsigned long dev_fd;
    std::string dev_path;
    int fd_ref;
    int event_fd;
    io_done_callback *io_cb_;
    io_context_t *libaio_context;
    std::atomic_int pending_size;
    libaio_device_service *device_service;
    thread_pool threads;

public:
    libaio_device(std::string dev_path, int max_event_num,
            libaio_device_service *device_service, io_done_callback *io_cb);

    int open();

    void close();

    unsigned long obtain_fd();

    unsigned long get_fd();

    int put_fd();

    int get_event_fd();

    void put_pending_size();

    int obtain_pending_size();

    int sync_read(unsigned long offset, unsigned int len, unsigned char *buf);

    int sync_write(unsigned long offset, unsigned int len, unsigned char *buf);

    void async_read(unsigned long offset, unsigned int len, unsigned char *buf, void *pri);

    void async_write(unsigned long offset, unsigned int len, unsigned char *buf, void *pri);

    std::string &obtain_dev_path();

    void get_io();

private:
    void get_pending_size();
};

typedef std::map<unsigned long, libaio_device *> devices_t;
typedef std::map<unsigned long, libaio_device *>::iterator devices_it_t;


#endif //SS_NBD_TEST_DEVICE_H
