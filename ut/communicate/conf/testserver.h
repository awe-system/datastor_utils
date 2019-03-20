#ifndef TESTSERVER_TESTSERVER_H
#define TESTSERVER_TESTSERVER_H

#ifndef _FUNCTION_TYPE_7777_H
#define _FUNCTION_TYPE_7777_H

enum client_function_callback_type_7777
{
    client_function_callback_type_7777_test,
    client_function_callback_type_7777_ping_internal,
};
enum server_function_callback_type_7777
{
    server_function_callback_type_7777_test,
    server_function_callback_type_7777_ping_internal,
};

#endif
#include <lt_server_service.h>
#include <lt_data_translator.h>

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

namespace testserver
{

class server;

class server_handler
{
public:
    virtual int test(IN const unsigned int &input_int, lt_session_description *server_context) = 0;

    virtual int ping_internal(){return 0;};

};

class server_handler_done
{
public:
    virtual void test_done(OUT unsigned int &output_int, lt_session_description *server_context, int error_internal) = 0;

};

class server : public lt_server_callback, public server_handler_done
{
private:
    lt_server_service service;

    server_handler *handler;

public:
    server(int thread_num, server_handler *_handler);

    void do_func(lt_data_t *data, lt_session_serv *sess) override;

private:
    int test_done_gendata(unsigned int output_int, lt_session_description *server_context, int error_internal, lt_data_t *data);

    int ping_internal_gendata(int error_internal, void *internal_sync_cond_p, void *internal_pri, lt_data_t *data);

public:
    void test_done(unsigned int &output_int, lt_session_description *server_context, int error_internal);

};
class testserver:  public server, public server_handler
{
public:
    testserver(int threadnum):
        server(threadnum, (server_handler *)this)
    {}
};

}
#endif
