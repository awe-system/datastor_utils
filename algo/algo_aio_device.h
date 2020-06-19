//
// Created by root on 6/18/20.
//

#ifndef DATASTOR_UTILS_ALGO_AIO_DEVICE_H
#define DATASTOR_UTILS_ALGO_AIO_DEVICE_H

#include <string>
#include <boost/atomic.hpp>
#include <mutex>
#include "algo_interface.h"
#include "libaio_device_server.h"
#include "libaio_device.h"

using namespace std;
namespace ServerSan_Algo
{

class algo_aio_device : public algo_device, public io_done_callback
{
public:
    algo_aio_device(const string ser, const string path,
            unsigned long size_secs, libaio_device_service *aio_server);

    unsigned long long int get_sector_num() const override ;

    string get_host_name(void) const override ;

    bool is_local() const override ;

    int open(void) override ;

    void close(void) override ;

    void do_request(request_t *request) override ;

    void write_done(void *pri, int error) override;

    void read_done(void *pri, int error) override;

private:
    libaio_device_service *aio_server_;
    string serial_num_;
    string path_;
    unsigned long long sector_num_;
    libaio_device aio_dev_;
};

}

#endif //DATASTOR_UTILS_ALGO_AIO_DEVICE_H
