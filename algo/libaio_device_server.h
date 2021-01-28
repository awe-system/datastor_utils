//
// Created by root on 18-3-16.
//

#ifndef SS_NBD_LIBAIO_DEVICE_SERVER_H
#define SS_NBD_LIBAIO_DEVICE_SERVER_H

#include "libaio_device.h"

#define MAX_EPOLL_EVENT 65535           //FIXME 设置epoll事件的最大值

class libaio_device_service
{
private:
    int epollfd;

    data_channel::thread_pool threads;

    struct epoll_event event_buf[MAX_EPOLL_EVENT];

public:
    libaio_device_service(int thread_num);

    void insert_device(libaio_device *device);

    void get_epoll_event();

    void handle_event(libaio_device* device);

private:
    void libaio_epoll_create();
};


#endif //SS_NBD_LIBAIO_DEVICE_SERVER_H
