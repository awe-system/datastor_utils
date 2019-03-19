//
// Created by root on 19-3-18.
//
#include <communicate/lt_session.h>
#include "conf/testclient.h"
#include "conf/testserver.h"
#include "../../algo/debug.h"

class test_server : public testserver::testserver
{
public:
    test_server() : testserver::testserver(1)
    {
    }

    int test(const unsigned int &input_int,
             lt_session_description *server_context) override
    {
        dbg << "test_server:input" << input_int << "server_context:" << server_context
                << end_dbgl;
        sleep(5);
        unsigned int output_int = input_int + 1;
        dbg << "test_server:output_int" << output_int << "server_context:" << server_context
                << end_dbgl;
        test_done(output_int,server_context, 0);
        return 0;
    }

    void disconnected(lt_session_description *sess) override
    {
        dbg << "test_server:disconnected" << end_dbgl;
        //hello
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
