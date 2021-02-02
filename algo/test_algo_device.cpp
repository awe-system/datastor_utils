#include "algo.h"
#include "test_algo_device.h"
// using namespace ServerSan_Algo;

using namespace std;

#include <fcntl.h>
#include "../log/include/awe_log.h"

#define MAX_QUE_DEPTH          10000
using namespace ServerSan_Algo;

static void dump_request(request_t *req)
{
    cout << "offset:" << req->offset << endl;
    cout << "len   :" << req->len << endl;
    if(req->is_read())
    {
        cout << "type  :READ" << endl;
    }
    else
    {
        cout << "type  :WRITE" << endl;
    }
}

#define ALIGNED_BASE 4096
#define uintptr_t unsigned long long int

bool ServerSan_Algo::is_buf_4k_aligned(void *buf)
{
    uintptr_t buf_long = (uintptr_t)buf;
    return buf_long % ALIGNED_BASE == 0;
}

void request_worker(test_algo_device *device)
{
    request_t *request = NULL;
    while(!device->is_stop)
    {
        assert(device != NULL);
        request = device->pop_request();
        if(!request)
        {
            usleep(100);
            continue;
        }
        try
        {
            unsigned int real_len = 0;
            if(device->fd < 0)
            {
                throw ERROR_TYPE_NOTOPEN;
            }

            if(!is_buf_4k_aligned(request->buf))
            {
                AWE_MODULE_ERROR("algo",
                                 "fault IO not 4K align request_worker path[%s] count[%lu] seek[%lu] buf[%p]  <=0 %p request %p : %s",
                                 device->path.c_str(), size_to_secs(request->len), request->offset, request->buf,
                                 device, request, request->to_json_obj().dumps().c_str());
            }

            switch(request->type)
            {
                case REQUEST_ASYNC_WRITE:
                    while(real_len < request->len)
                    {
                        int this_len = pwrite(device->fd, request->buf,
                                              request->len,
                                              request->offset * 512);
                        if(this_len <= 0)
                        {
                            AWE_MODULE_ERROR("algo",
                                             "fault IO request_worker path[%s] count[%lu] seek[%lu] buf[%p] this_len[%d] <=0 %p request %p : %s",
                                             device->path.c_str(), size_to_secs(request->len), request->offset, request->buf, this_len,
                                             device, request, request->to_json_obj().dumps().c_str());
                            dump_request(request);
                            throw ERROR_TYPE_DEVICE;
                        }

                        real_len += this_len;
                    }
                    break;
                case REQUEST_ASYNC_READ:
                    while(real_len < request->len)
                    {
                        int this_len = pread(device->fd, request->buf,
                                             request->len,
                                             request->offset * 512);
                        if(this_len <= 0)
                        {
                            AWE_MODULE_ERROR("algo",
                                             "fault IO request_worker path[%s] count[%u] seek[%lu] this_len[%d] <=0 %p request %p : %s",
                                             device->path.c_str(), size_to_secs(request->len), request->offset, this_len,
                                             device, request, request->to_json_obj().dumps().c_str());
                            dump_request(request);
                            throw ERROR_TYPE_DEVICE;
                        }
                        real_len += this_len;
                    }
                    break;
                default:
                {
                    printf("throw 2, errno = %d\n", errno);
                    throw ERROR_TYPE_WRONGTYPE;
                }
            }
        }
        catch(...)
        {
            AWE_MODULE_ERROR("algo",
                             "request_worker before complete ERROR_TYPE_DEVICE device %p request %p : %s",
                             device, request, request->to_json_obj().dumps().c_str());
            //            printf("test error req done catch\n");
            device->complete_request(request, ERROR_TYPE_DEVICE);

            AWE_MODULE_ERROR("algo",
                             "request_worker after complete ERROR_TYPE_DEVICE device %p request %p",
                             device, request);
            continue;
        }
        //        printf("test error req done ok \n");
        AWE_MODULE_DEBUG("algo",
                         "request_worker before complete OK device %p request %p : %s",
                         device, request, request->to_json_obj().dumps().c_str());
        device->complete_request(request, ERROR_TYPE_OK);
        AWE_MODULE_DEBUG("algo",
                         "request_worker after complete OK device %p request %p",
                         device, request);
    }
}

test_algo_device::test_algo_device(const string &serial_num, const std::string &path,
                                   unsigned long sector_num) :
        serial_num(serial_num),
        path(path),
        sector_num(sector_num),
        io_request_queue(MAX_QUE_DEPTH)
{
    th = io_worker_threads.create_thread(bind(request_worker, this));
}


test_algo_device::test_algo_device(const string &_ser, const string &_path,
                                   const string &host,
                                   unsigned long _size_secs, bool is_local) :
        serial_num(_ser),
        path(_path),
        host_name(host),
        sector_num(_size_secs),
        _is_local(is_local),
        io_request_queue(MAX_QUE_DEPTH)
{
    th = io_worker_threads.create_thread(bind(request_worker, this));
}


test_algo_device::test_algo_device(const string &_ser, const string &_path,
                                   const string &host,
                                   unsigned long long sector_num,
                                   const string &ip) :
        serial_num(_ser),
        path(_path),
        host_name(host),
        sector_num(sector_num),
        //        _is_local(is_local),
        io_request_queue(MAX_QUE_DEPTH),
        ip(ip)
{
    th = io_worker_threads.create_thread(bind(request_worker, this));
}

unsigned long long test_algo_device::get_sector_num() const
{
    return sector_num;
}

string test_algo_device::get_device_id(void) const
{
    return serial_num;
}

string test_algo_device::get_host_name(void) const
{
    return host_name;
}


bool test_algo_device::is_local(void) const
{
    return _is_local;
}

int test_algo_device::open(void)
{
    fd = ::open(path.c_str(), O_RDWR | O_DIRECT);
    //    printf("test_algo_device::open:fd = %d, errno = %d, path = %s\n", fd, errno, path.c_str());
    if(fd == -1)
    {
        AWE_MODULE_DEBUG("algo", "open file failed : %d, path : %s", fd, path.c_str());
        return -1;
    }
    return 0;
}

void test_algo_device::close(void)
{
    if(fd > 0)
    {
        ::close(fd);
    }
    fd = -1;
    AWE_MODULE_DEBUG("algo", "close file : %d, path : %s", fd, path.c_str());
    return;
}

void test_algo_device::do_request(request_t *request)
{
    bool is_sync = false;

    //    assert(io_callback);
    AWE_MODULE_DEBUG("algo",
                     "do_request %p request %p : %s request",
                     this, request, request->to_json_obj().dumps().c_str());
    try
    {
        unsigned int real_len = 0;
        if(fd == -1)
        {
            AWE_MODULE_DEBUG("algo", "open file failed : %d, path : %s", fd, path.c_str());
            throw ERROR_TYPE_NOTOPEN;
        }
        switch(request->type)
        {
            case REQUEST_SYNC_WRITE:
                is_sync = true;
                //  cout << "Write" << endl;
                //                cout<<"fd:"<<fd<<" len:"<<request->len<<" off:"<<request->offset<<endl;
                while(real_len < request->len)
                {
                    int this_len = pwrite(fd, request->buf, request->len,
                                          request->offset * 512);

                    //                cout<<"after Write real_len:"<<real_len<<endl;
                    if(this_len <= 0)
                    {
                        throw ERROR_TYPE_DEVICE;
                    }
                    real_len += this_len;
                }
                break;
            case REQUEST_ASYNC_WRITE:
            {
                std::unique_lock<std::mutex> lck(m);
                io_request_queue.push_back(request);
            }
                break;
            case REQUEST_SYNC_READ:
                is_sync = true;
                while(real_len < request->len)
                {
                    int this_len = pread(fd, request->buf, request->len,
                                         request->offset * 512);
                    if(this_len <= 0)
                    {
                        throw ERROR_TYPE_DEVICE;
                    }
                    real_len += this_len;
                }
                break;
            case REQUEST_ASYNC_READ:
            {
                std::unique_lock<std::mutex> lck(m);
                io_request_queue.push_back(request);
            }
                break;
            default:
                throw ERROR_TYPE_WRONGTYPE;
        }
    }
    catch(ERROR_T &err)
    {
        if(is_sync)
        {
            complete_request(request, -err);
        }
        else
        {
            std::unique_lock<std::mutex> lck(m);
            io_request_queue.push_back(request);
        }

        return;
    }
    if(is_sync)
    {
        complete_request(request, ERROR_TYPE_OK);
    }

}

string test_algo_device::get_ip()
{
    return ip;
}

void test_algo_device::set_ip(string &ip)
{
    this->ip = ip;
}

test_algo_device::~test_algo_device()
{
    AWE_MODULE_DEBUG("algo", "test_algo_device::~test_algo_device this %p",
                     this);
    is_stop = true;

    AWE_MODULE_DEBUG("algo", "~test_algo_device before join this %p", this);
    th->join();

    AWE_MODULE_DEBUG("algo", "~test_algo_device after join this %p", this);
}

request_t *test_algo_device::pop_request()
{
    std::unique_lock<std::mutex> lck(m);
    if(io_request_queue.empty())
    {
        return nullptr;
    }
    request_t *req = io_request_queue.front();
    io_request_queue.pop_front();
    return req;
}
