#ifndef TEST_ALGO_DEVICE_H
#define TEST_ALGO_DEVICE_H

#include <string>
#include "algo_interface.h"
#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>
#include <boost/thread.hpp>

using namespace std;
namespace ServerSan_Algo
{
/*
FIXME:下面这个类是一个示例具体的实现要重新写包括构造函数的形式也需要更改
*/
class test_algo_device : public algo_device
{
public:
    unsigned long long int get_sector_num() const;

    string get_host_name(void) const;

    bool is_local(void) const;

    int open(void);

    void close(void);

    void do_request(request_t *request);

    string get_ip();
    void set_ip(string &ip);
public:/*下面函数会根据不同设备进行不同实现*/
    test_algo_device(const string &_ser, const string &_path, unsigned long _size_secs);
    test_algo_device(const string &_ser, const string &_path, const string& host,unsigned long _size_secs, bool _is_local);
    test_algo_device(const string &_ser,const string& _path, const string& host, unsigned long long _size_secs,const  string &ip);

    string get_device_id(void) const;

    boost::lockfree::queue<request_t *, boost::lockfree::fixed_sized<false>> io_request_queue; //搞个队列专门存储request
    boost::thread_group io_worker_threads;
    int fd;

private:
    string serial_num;
    string path;
    unsigned long long sector_num;
    string host_name;
    bool _is_local;
    string ip;
};

}
#endif