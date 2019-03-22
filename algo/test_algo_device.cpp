#include "algo.h"
#include "test_algo_device.h"
// using namespace ServerSan_Algo;

using namespace std;

#include <fcntl.h>

using namespace ServerSan_Algo;

void request_worker(test_algo_device *device)
{
    request_t *request = NULL;
    while ( 1 )
    {
        assert(device != NULL );
        if ( device->io_request_queue.empty())
        {
            usleep(100);
            continue;
        }

//        assert(device->io_callback);
        device->io_request_queue.pop(request);
        try
        {
            unsigned int real_len = 0;
            if ( device->fd < 0 )
            {
                throw ERROR_TYPE_NOTOPEN;
            }
            switch ( request->type )
            {
                case REQUEST_ASYNC_WRITE:
//                    printf("ASYNC Write req = %p, offset = %d, len = %d, buf = %p\n", request, request->offset, request->len,request->buf);
                    real_len = pwrite(device->fd, request->buf, request->len, request->offset * 512);
//                    cout << "after Write real_len:" << real_len << endl;
                    if ( real_len != request->len )
                        throw ERROR_TYPE_DEVICE;
                    break;
                case REQUEST_ASYNC_READ:
//                    printf("ASYNC Read req = %p, offset = %d, len = %d, buf = %p\n", request, request->offset, request->len,request->buf);
                    real_len = pread(device->fd, request->buf, request->len, request->offset * 512);
//                    cout << "after Read real_len:" << real_len << endl;
                    if ( real_len != request->len )
                    {
                        printf("throw 1, errno = %d\n", errno);
                        throw ERROR_TYPE_DEVICE;
                    }
                    break;
                default:
                {
                    printf("throw 2, errno = %d\n", errno);
                    throw ERROR_TYPE_WRONGTYPE;
                }
            }
        }
        catch (...)
        {
//            printf("test error req done catch\n");
            device->complete_request(request, ERROR_TYPE_DEVICE);
            continue;
        }
//        printf("test error req done ok \n");
        device->complete_request(request, ERROR_TYPE_OK);
    }
}

test_algo_device::test_algo_device(const string &serial_num,const string &path, unsigned long sector_num) :
        serial_num(serial_num),
        path(path),
        sector_num(sector_num),
        io_request_queue(1280000)
{
    io_worker_threads.create_thread(bind(request_worker, this));
}


test_algo_device::test_algo_device(const string &_ser, const string &_path, const string &host,
                                   unsigned long _size_secs, bool is_local):
        serial_num(_ser),
        path(_path),
        host_name(host),
        sector_num(_size_secs),
        _is_local(is_local),
        io_request_queue(1280000)
{
    io_worker_threads.create_thread(bind(request_worker, this));
}



test_algo_device::test_algo_device(const string& _ser, const string& _path, const string& host, unsigned long long sector_num,const string&ip) :
        serial_num(_ser),
        path(_path),
        host_name(host),
        sector_num(sector_num),
        //        _is_local(is_local),
        io_request_queue(1280000),
        ip(ip)
{
    io_worker_threads.create_thread(bind(request_worker, this));
}

unsigned long long test_algo_device::get_sector_num() const
{
    return sector_num;
}

string test_algo_device::get_device_id(void)const
{
    return serial_num;
}

string test_algo_device::get_host_name(void)const
{
    return host_name;
}


bool test_algo_device::is_local(void)const
{
    return _is_local;
}

int test_algo_device::open(void)
{
    fd = ::open(path.c_str(), O_RDWR);
//    printf("test_algo_device::open:fd = %d, errno = %d, path = %s\n", fd, errno, path.c_str());
    return 0;
}

void test_algo_device::close(void)
{
    if ( fd > 0 )
        ::close(fd);
    fd = -1;
    return;
}

void test_algo_device::do_request(request_t *request)
{
    bool is_sync = false;

//    assert(io_callback);
    try
    {
        unsigned int real_len = 0;
        if ( fd < 0 )
        {
            throw ERROR_TYPE_NOTOPEN;
        }
        switch ( request->type )
        {
            case REQUEST_SYNC_WRITE:
                is_sync = true;
              //  cout << "Write" << endl;
                //                cout<<"fd:"<<fd<<" len:"<<request->len<<" off:"<<request->offset<<endl;
                real_len = pwrite(fd, request->buf, request->len, request->offset * 512);
                //                cout<<"after Write real_len:"<<real_len<<endl;
                if ( real_len != request->len )
                    throw ERROR_TYPE_DEVICE;
                break;
            case REQUEST_ASYNC_WRITE:
                io_request_queue.push(request);      //异步则直接加入队列
                break;
            case REQUEST_SYNC_READ:
                is_sync = true;
                //cout << "Read" << endl;
                //                cout<<"fd:"<<fd<<" len:"<<request->len<<" off:"<<request->offset<<endl;
                real_len = pread(fd, request->buf, request->len, request->offset * 512);
                //                cout<<"after Read real_len:"<<real_len<< endl;
                if ( real_len != request->len )
                    throw ERROR_TYPE_DEVICE;
                break;
            case REQUEST_ASYNC_READ:
                io_request_queue.push(request);        //异步则直接加入队列
                break;
            default:
                throw ERROR_TYPE_WRONGTYPE;
        }
    }
    catch (ERROR_T &err)
    {
        if ( is_sync )
        {
            complete_request(request,-err);
        }

        return;
    }
    if ( is_sync )
    {
        complete_request(request,ERROR_TYPE_OK);
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
