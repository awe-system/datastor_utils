//
// Created by root on 6/18/20.
//

#include "algo_aio_device.h"

namespace ServerSan_Algo {

algo_aio_device::algo_aio_device(const string ser, const string path, unsigned long size_secs,
                                 libaio_device_service *aio_server) :
    serial_num_(ser), path_(path),sector_num_(size_secs), aio_server_(aio_server),
    aio_dev_(path, 256, aio_server, this){}


unsigned long long int ServerSan_Algo::algo_aio_device::get_sector_num() const {
    return sector_num_;
}

string ServerSan_Algo::algo_aio_device::get_host_name(void) const {
    return serial_num_;
}

bool ServerSan_Algo::algo_aio_device::is_local() const {
    return false;
}

int ServerSan_Algo::algo_aio_device::open(void) {
    return aio_dev_.open();
}

void ServerSan_Algo::algo_aio_device::close(void) {
    aio_dev_.close();
}

void ServerSan_Algo::algo_aio_device::do_request(ServerSan_Algo::request_t *request) {
    if( request->is_read()) {
        aio_dev_.async_read(request->offset, request->len, request->buf, request);
    } else {
        aio_dev_.async_write(request->offset, request->len, request->buf, request);
    }
}

void algo_aio_device::write_done(void *pri, int error) {
    complete_request(static_cast<ServerSan_Algo::request_t*>(pri), error);
}

void algo_aio_device::read_done(void *pri, int error) {
    complete_request(static_cast<ServerSan_Algo::request_t*>(pri), error);
}

}
