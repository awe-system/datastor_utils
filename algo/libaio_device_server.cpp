//
// Created by root on 18-3-16.
//

#include "libaio_device_server.h"

libaio_device_service::libaio_device_service(int thread_num) : threads(thread_num)
{
    libaio_epoll_create();
    boost::thread epoll_thread(boost::bind(&libaio_device_service::get_epoll_event, this));

}

void libaio_device_service::get_epoll_event()
{
    int res = 0, i = 0;
    while (1) {
        res = epoll_wait(epollfd, event_buf, MAX_EPOLL_EVENT, -1);
        if (res == -1) {
            perror("epoll_wait");
            return ;
        }

        for(i = 0; i < res; i++)
        {
            libaio_device *device =(libaio_device *)event_buf[i].data.ptr;
            threads.submit_task(boost::bind(&libaio_device_service::handle_event, this, device));
        }
    }
}

void libaio_device_service::handle_event(libaio_device* device) {
    device->get_io();
}

void libaio_device_service::libaio_epoll_create()
{
    epollfd = epoll_create(MAX_EPOLL_EVENT);
    if ( epollfd == -1 )
    {
        perror("epoll_create-----------");
        return ;
    }
}


void libaio_device_service::insert_device(unsigned long fd, libaio_device *device)
{
    std::lock_guard<std::mutex> lock(devices_mutex);
    devices.insert(std::make_pair(fd, device));

    ev_ctl.events = EPOLLIN | EPOLLET;
    ev_ctl.data.ptr = (void *)device;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, device->get_event_fd(), &ev_ctl)) {
        perror("epoll_ctl");
        return ;
    }
}

void libaio_device_service::erase_device(unsigned long fd)
{
    std::lock_guard<std::mutex> lock(devices_mutex);
    devices_it_t it = devices.find(fd);
    if ( it != devices.end())
    {
        devices.erase(it);
    }
}

