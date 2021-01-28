//
// Created by root on 2021/1/27.
//

#include <lt_function/thread_pool.hpp>
#include <awe_log.h>
#include "algo.h"
#include "psync_device.h"
#include "psync_service.h"

#define ALIGN_SIZE  4096

void psync_device::from_json_obj(const json_obj &obj)
{
}

json_obj psync_device::to_json_obj() const
{
    return json_obj();
}

unsigned long long int psync_device::get_sector_num() const
{
    return sector_num;
}

int psync_device::open(void)
{
    fd = ::open(path.c_str(), O_RDWR | O_DIRECT);
    if(fd == -1)
    {
        AWE_MODULE_ERROR("algo", "open file failed : %d, path : %s", fd, path.c_str());
        return -1;
    }
    return 0;
}

void psync_device::close(void)
{
    if(fd > 0)
    {
        ::close(fd);
    }
    fd = -1;
    AWE_MODULE_DEBUG("algo", "close file : %d, path : %s", fd, path.c_str());
    return;
}

void psync_device::do_request(ServerSan_Algo::request_t *request)
{
    assert_legal();
    AWE_MODULE_DEBUG("algo", "fd [%d] path : %s do request [%p] [%s]",
                     fd, path.c_str(), request, request->to_json_obj().dumps().c_str());
    switch(request->type)
    {
        case ServerSan_Algo::REQUEST_SYNC_WRITE:
        {
            AWE_MODULE_DEBUG("algo", "path : %s do request [%p] [%s]",
                             path.c_str(), request, request->to_json_obj().dumps().c_str());
            do_request_write(request);
            break;
        }
        case ServerSan_Algo::REQUEST_SYNC_READ:
        {
            AWE_MODULE_DEBUG("algo", "path : %s do request [%p] [%s]",
                             path.c_str(), request, request->to_json_obj().dumps().c_str());
            do_request_read(request);
            break;
        }
        case ServerSan_Algo::REQUEST_ASYNC_WRITE:
        case ServerSan_Algo::REQUEST_ASYNC_READ:
        {
            ServerSan_Algo::psync_service *service = ServerSan_Algo::get_psync_service();
            assert(service);
            AWE_MODULE_DEBUG("algo", "path : %s  service [%p] do request [%p] [%s]",
                             path.c_str(), service, request, request->to_json_obj().dumps().c_str());
            service->do_request(this, request);
            break;
        }
        default:
            abort();
    }
}

psync_device::~psync_device()
{
    AWE_MODULE_DEBUG("algo", "~psync_device file : %d, path : %s", fd, path.c_str());
}

psync_device::psync_device(const string &_path, unsigned long _size_secs) :
        path(_path), sector_num(_size_secs), fd(-1)
{
    AWE_MODULE_DEBUG("algo", "psync_device file : %d, path : %s", fd, path.c_str());
}

void psync_device::request_mem_align_read(ServerSan_Algo::request_t *request)
{
    AWE_MODULE_DEBUG("algo", "path : %s do request [%p] [%s]",
                     path.c_str(), request, request->to_json_obj().dumps().c_str());
    void *buf_4K_aligned = NULL;
    if(request->len % ALIGN_SIZE != 0)
    {
        AWE_MODULE_ERROR("algo", "path : %s do request [%p] [%s]",
                         path.c_str(), request, request->to_json_obj().dumps().c_str());
    }
    assert(request->len % ALIGN_SIZE == 0);
    assert(!::posix_memalign(&buf_4K_aligned, ALIGN_SIZE, request->len));
    request->push_private(request->buf);
    request->buf = static_cast<unsigned char *>(buf_4K_aligned);
    memset(request->buf, 0xaa, request->len);
    AWE_MODULE_DEBUG("algo", "path : %s  request->buf [%p]do request [%p] [%s]",
                     path.c_str(),request->buf, request, request->to_json_obj().dumps().c_str());
}

void psync_device::request_mem_recovery_read(ServerSan_Algo::request_t *request)
{
    AWE_MODULE_DEBUG("algo", "path : %s do request [%p] [%s]",
                     path.c_str(), request, request->to_json_obj().dumps().c_str());
    void *buf_4K_aligned = request->buf;
    request->buf = static_cast<unsigned char *>(request->pop_private());
    memcpy(request->buf, buf_4K_aligned, request->len);
    free(buf_4K_aligned);
    AWE_MODULE_DEBUG("algo", "path : %s do request [%p] [%s]",
                     path.c_str(), request, request->to_json_obj().dumps().c_str());
}

void psync_device::request_mem_align_write(ServerSan_Algo::request_t *request)
{
    AWE_MODULE_DEBUG("algo", "path : %s do request [%p] [%s]",
                     path.c_str(), request, request->to_json_obj().dumps().c_str());
    void *buf_4K_aligned = NULL;
    assert(request->len % ALIGN_SIZE == 0);
    assert(!::posix_memalign(reinterpret_cast<void **>(&buf_4K_aligned), ALIGN_SIZE, request->len));
    memcpy(buf_4K_aligned, request->buf, request->len);
    request->push_private(request->buf);
    request->buf = static_cast<unsigned char *>(buf_4K_aligned);
    AWE_MODULE_DEBUG("algo", "path : %s do request [%p] [%s]",
                     path.c_str(), request, request->to_json_obj().dumps().c_str());
}

void psync_device::request_mem_recovery_write(ServerSan_Algo::request_t *request)
{
    AWE_MODULE_DEBUG("algo", "path : %s do request [%p] [%s]",
                     path.c_str(), request, request->to_json_obj().dumps().c_str());
    void *buf_4K_aligned = request->buf;
    request->buf = static_cast<unsigned char *>(request->pop_private());
    free(buf_4K_aligned);
    AWE_MODULE_DEBUG("algo", "path : %s do request [%p] [%s]",
                     path.c_str(), request, request->to_json_obj().dumps().c_str());
}

void psync_device::do_request_write(ServerSan_Algo::request_t *request)
{
    int err = ServerSan_Algo::ERROR_TYPE_OK;

    request_mem_align_write(request);

    ssize_t this_len = pwrite(fd, request->buf,
                              request->len,
                              request->offset * 512);

    request_mem_recovery_write(request);

    if(this_len <= 0)
    {
        AWE_MODULE_ERROR("algo", "path : %s do request [%p] [%s]",
                         path.c_str(), request, request->to_json_obj().dumps().c_str());
        err = -ServerSan_Algo::ERROR_TYPE_DEVICE;
    }
    AWE_MODULE_DEBUG("algo", "complete write path : %s do request [%p] [%s]",
                     path.c_str(), request, request->to_json_obj().dumps().c_str());
    complete_request(request, err);
}

void psync_device::do_request_read(ServerSan_Algo::request_t *request)
{
    int err = ServerSan_Algo::ERROR_TYPE_OK;

    request_mem_align_read(request);

    ssize_t this_len = pread(fd, request->buf,
                             request->len,
                             request->offset * 512);
    if(this_len <= 0)
    {
        AWE_MODULE_ERROR("algo", "fd [%d]  path : %s do request [%p] [%s] this_len [%d]",
                         fd, path.c_str(), request, request->to_json_obj().dumps().c_str(), this_len);
        err = -ServerSan_Algo::ERROR_TYPE_DEVICE;
    }

    request_mem_recovery_read(request);


    AWE_MODULE_DEBUG("algo", "complete read path : %s do request [%p] [%s]",
                     path.c_str(), request, request->to_json_obj().dumps().c_str());
    complete_request(request, err);
}

void psync_device::do_request_sync(ServerSan_Algo::request_t *request)
{
    assert_legal();
    AWE_MODULE_DEBUG("algo", "path : %s do request [%p] [%s]",
                     path.c_str(), request, request->to_json_obj().dumps().c_str());
    switch(request->type)
    {
        case ServerSan_Algo::REQUEST_ASYNC_WRITE:
        {
            AWE_MODULE_DEBUG("algo", "path : %s do request [%p] [%s]",
                             path.c_str(), request, request->to_json_obj().dumps().c_str());
            do_request_write(request);
            break;
        }
        case ServerSan_Algo::REQUEST_ASYNC_READ:
        {
            AWE_MODULE_DEBUG("algo", "path : %s do request [%p] [%s]",
                             path.c_str(), request, request->to_json_obj().dumps().c_str());
            do_request_read(request);
            break;
        }
        default:
            abort();
    }
}
