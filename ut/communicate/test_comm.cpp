//
// Created by root on 19-3-18.
//
#include <communicate/lt_session.h>
#include <err.h>
#include "conf/testclient.h"
#include "conf/test_async_client.h"
#include "conf/test_sync_client.h"
#include "../../algo/debug.h"

class testcli : public testclient::testclient
{
public:
    std::mutex mtx;
    std::condition_variable cv;
    
    testcli():is_closing(false), count(0){};
    std::atomic_bool is_closing;
    std::atomic_uint count;
    void disconnected() override
    {
        std::cout << "------" << __FUNCTION__ << std::endl;
        //this->disconnect();
        is_closing = false;
    }
    
    int connect(const std::string &ip)
    {
        //if(is_closing) return -1;
       // cout<<"when not closing connect"<<endl;
        is_closing = true;
        count = 0;
        return testclient::connect(ip);
    }
    
    void test_callback(unsigned int &output_int, int error_internal) override
    {
        //count++;
        std::unique_lock<std::mutex> lock(mtx);
        int tmp = count.fetch_add(1);
        if(tmp == 99) {
            std::cout << "----------------------------------------" << std::endl;
            std::cout << __FUNCTION__ << "  err : " << error_internal <<
                " count :" << count <<  std::endl;
            cv.notify_one();
            lock.unlock();
        }
        if (error_internal) {
            std::cout << "------ error : " << error_internal << "---" << count <<std::endl;
        } else {
            std::cout << "-----call_back i :" << output_int << std::endl;
        }
    }
    
    void test_void_p_callback( std::string &output_string,
                        char * &output_char_p,
                        unsigned long &output_ulong,
                        int error_internal) {
        std::cout << " output_string : " << output_string
              << " output_char_p : " << output_char_p
              << " output_ulong : " << output_ulong
              << " error_internal : " << error_internal << std::endl;
    }
    
    void test_bool_callback(
            bool &output_bool,
            unsigned long &output_data_len,
            unsigned char *output_data_buf,
            void *&output_void_p,
            int error_internal) {
        std::cout << " output_bool : " << output_bool
                << " output_data_len : " << output_data_len
                << " output_data_buf : " << output_data_buf
                << " output_void_p : " << output_void_p
                << " error_internal : " << error_internal << std::endl;
    }
    
    void sleep() {
       std::unique_lock<std::mutex> lock(mtx);
       if(count < 100) {
           cv.wait(lock);
       }
    }
};

void test(){
    testcli cli;
    for(int i =0;i<10000;++i)
    {
        int err;
        do {
            //sleep(2);
            //usleep(0.1);
            err = cli.connect("127.0.0.1");
            std::cout << "connect : " << err << std::endl;
        }while (err);
        for(uint64_t j = 0 ; j < 100; j++) {
            err = cli.test(j);
            if(err)
                cout<<"test err wait 1 second"<<endl;
            //else
            
             // cout<<"j : "<<j<<endl;
        }
        std::cout << "client : disconnect start !" << std::endl;
        cli.sleep();
        cli.disconnect();
        std::cout << "client : disconnect stop !" << std::endl;
    }
}

class asyncclient : public  testasyncclient::test_async_client {
public:
    void disconnected() {}
    void test_callback( unsigned int &output_int, int error_internal) {
        std::cout << " output_int : " << output_int  <<
                    " error_internal : " << error_internal << std::endl;
    }
    
    void test_string_callback(OUT std::string &output_string, int error_internal){
        std::cout << " output_string : " << output_string <<
                 " error_internal : " << error_internal << std::endl;
    }
    
    
    void test_char_p_callback(OUT char * &output_char_p, int error_internal) {
        std::cout << " output_char_p : " << output_char_p <<
                    " error_internal : " << error_internal << std::endl;
    }
    
    void test_ulong_callback(OUT unsigned long &output_ulong, int error_internal){
        std::cout << " output_ulong : " << output_ulong <<
                " error_internal : " << error_internal << std::endl;
    }
    
    void test_bool_callback(OUT bool &output_bool, int error_internal) {
        std::cout << " output_bool : " << output_bool <<
            " error_internal : " << error_internal << std::endl;
    }
    
    void test_data_callback( unsigned long &output_data_len,
            unsigned char *output_data_buf, int error_internal) {
       std::cout << " output_data_len : " << output_data_len <<
               " error_internal : " << error_internal << std::endl;
    }
    
    void test_void_p_callback(OUT void *&output_void_p, int error_internal) {
        std::cout << " output_void_p : " << output_void_p <<
                " error_internal : " << error_internal << std::endl;
    }
    
    void test_no_arg_callback(int error_internal) {
        std::cout << " error_internal : " << error_internal << std::endl;
    }
    
private:
};

int main(int argc, const char *argv[])
{
    /*
    testcli cli;
    char *in = "abcd";
    const void *p = (void *)0x10;
    
    cli.connect("127.0.0.1");
    
    const std::string a("in");
    const char* b = "abcd";
    cli.test_void_p(a, b, 5);
    cli.test_bool(true, 5, (unsigned char *)in, p);
    
    sleep(5);
    
    asyncclient ac;
    ac.test(10);
    ac.test_string("123");
    const char* tt = "123";
    ac.test_char_p(tt);
    ac.test_ulong(100);
    ac.test_bool(false);
    unsigned char tt1[10] = {"abc"};
    ac.test_data(10, tt1);
    const void* tt2 = tt1;
    ac.test_void_p(tt2);
    ac.test_no_arg();
    
    sleep(5);
     */
    
    std::thread threads[2];
    for(auto &i : threads)
    {
        i = std::thread(test);
    }
    for(auto &i : threads) {
        i.join();
    }
    return 0;
}
