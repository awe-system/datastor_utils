//
// Created by root on 19-3-18.
//
#include <communicate/lt_session.h>
#include "conf/testserver.h"
#include "../../algo/debug.h"
#include "conf/test_async_server.h"
#include "conf/test_sync_server.h"

class test_server : public testserver::testserver
{
public:
    test_server() : testserver::testserver(1)
    {
    }

    int test(const unsigned int &input_int,
             lt_session_description *server_context) override
    {
        unsigned int output_int = input_int + 1;
        //std::cout  << "output : " << output_int << std::endl;
        test_done(output_int, server_context, 0);
        return 0;
    }
    
    int test_void_p(const std::string &input_string,
            const char * &input_char_p,
            const unsigned long &input_ulong,
            lt_session_description *server_context) override {
        std::string out = input_string;
        char* output_char_p = const_cast<char *>(input_char_p);
        unsigned long output_ulong = input_ulong;
        test_void_p_done(out, output_char_p, output_ulong , server_context, 0);
        return 0;
    }
    
    int test_bool(const bool &input_bool,
            const unsigned long &input_data_len,
            unsigned char *input_data_buf,
            const void *&input_void_p,
            lt_session_description *server_context) override {
        bool output_bool = input_bool;
        unsigned long output_data_len = input_data_len;
        void * output_void_p = const_cast<void*>(input_void_p);
        test_bool_done(output_bool, output_data_len, input_data_buf, output_void_p,
                server_context, 0);
        return 0;
    }
    
    void disconnected(lt_session_description *sess) override
    {
        dbg << "test_server:disconnected" << end_dbgl;
    }
};

class test_async  : public testasyncserver::test_async_server {

public:
    int test( const unsigned int &input_int, lt_session_description *server_context) {
        unsigned int tmp = input_int;
        test_done(tmp, server_context, 0);
    }
    
    int test_string( const std::string &input_string, lt_session_description *server_context) {
        std::string tmp = input_string;
        test_string_done(tmp, server_context, 0);
    }
    
    int test_char_p( const char * &input_char_p, lt_session_description *server_context) {
        char *tmp = (char *)input_char_p;
        test_char_p_done(tmp, server_context, 0);
    }
    
    int test_ulong( const unsigned long &input_ulong, lt_session_description *server_context) {
        unsigned long tmp = input_ulong;
        test_ulong_done(tmp, server_context, 0);
    }
    
    int test_bool( const bool &input_bool, lt_session_description *server_context) {
        bool tmp = input_bool;
        test_bool_done(tmp, server_context, 0);
    }
    
     int test_data(IN const unsigned long &input_data_len, IN unsigned char *input_data_buf,
             lt_session_description *server_context) {
        unsigned  long tmp_len = input_data_len;
        test_data_done(tmp_len, input_data_buf, server_context, 0);
    }
    
    int test_void_p(IN const void *&input_void_p, lt_session_description *server_context) {
         void *tmp  =(void *)input_void_p;
        test_void_p_done(tmp, server_context, 0);
    }
    
    int test_no_arg(lt_session_description *server_context) {
        test_no_arg_done(server_context, 0);
    }
    
};

int main(int argc, const char *argv[])
{
    test_server server;
    sleep(3);
    while(1)
    {
        sleep(1);
    }
}
