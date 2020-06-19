//
// Created by root on 18-3-16.
//

#ifndef SS_NBD_LIBAIO_DEVICE_SERVER_H
#define SS_NBD_LIBAIO_DEVICE_SERVER_H

#include "libaio_device.h"

#define MAX_EPOLL_EVENT 1024           //FIXME 设置epoll事件的最大值

class libaio_device_service
{
private:
    int epollfd;

    thread_pool threads;
    devices_t devices;

    std::mutex devices_mutex;
    struct epoll_event ev_ctl, event_buf[MAX_EPOLL_EVENT];

public:
    libaio_device_service(int thread_num);

    void insert_device(unsigned long fd, libaio_device *device);

    void erase_device(unsigned long fd);

    void get_epoll_event();

    void handle_event(libaio_device* device);

private:
    void libaio_epoll_create();
};


#endif //SS_NBD_LIBAIO_DEVICE_SERVER_H
