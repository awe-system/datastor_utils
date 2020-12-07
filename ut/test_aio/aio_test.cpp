//
// Created by root on 2020/11/11.
//

#include <iostream>
#include "../algo/algo_aio_device.h"

class test_aio : public ServerSan_Algo::block_io_callback
{

public:
    test_aio() = default;
    void test_write()
    {
        memset(a, 'j', sizeof(a));
        libaio_device_service ser(4);
        ServerSan_Algo::algo_aio_device device("aaa", "/dev/sdc", 20000, &ser);
        ServerSan_Algo::request_t req;
        req.buf = a;
        req.len = sizeof(a);
        req.offset = 0;
        req.type = ServerSan_Algo::REQUEST_ASYNC_WRITE;
        //req.type=ServerSan_Algo::REQUEST_ASYNC_READ;
        device.open();
        device.do_request_withcb(&req, this);
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock);
    }

    void test_read()
    {
        //memset(a, 'a', 4096);
        libaio_device_service ser(4);
        ServerSan_Algo::algo_aio_device device("aaa", "/dev/sdc", 20000, &ser);
        ServerSan_Algo::request_t req;
        req.buf = static_cast<unsigned char *>(malloc(4096));
        req.len = sizeof(a);
        req.offset = 0;
        //req.type=ServerSan_Algo::REQUEST_ASYNC_WRITE;
        req.type = ServerSan_Algo::REQUEST_ASYNC_READ;
        device.open();
        device.do_request_withcb(&req, this);
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock);
    }

    void request_done(ServerSan_Algo::request_t *request, int err) override
    {
        if(err)
        {
            std::cout << "------------err=%d---" << err << std::endl;
            assert(0);
        }
        //std::cout << request->buf << std::endl;
        if(request->is_read())
        {
            if(memcmp(request->buf, a, request->len) != 0)
            {
                std::cout << "error " << std::endl;
            }
            else
            {
                std::cout << "test ok" << std::endl;
                cv.notify_one();
            }
        }
        cv.notify_one();
    }

private:
    unsigned char a[500] = {0};
    std::mutex mtx;
    std::condition_variable cv;
};

int main()
{
    //test_aio testaio;
    //testaio.test_write();
    //testaio.test_read();
}

