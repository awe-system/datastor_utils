#ifndef TESTCLIENT_TESTCLIENT_INTERNAL_H
#define TESTCLIENT_TESTCLIENT_INTERNAL_H

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
#include <lt_session.h>
#include <lt_session_cli_safe.h>
#include <lt_client_service.h>
#include <lt_session_cli_set.h>
#include <lt_data_translator.h>
#include <lt_condition.h>

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

namespace testclient
{

class testclient_callback_handler
{
public:
    virtual void disconnected(lt_session *sess) = 0;
    virtual void test_callback(OUT unsigned int &output_int, INOUT void *&internal_pri, int error_internal) = 0;

};

class testclient_client_callback : public lt_client_service, public lt_session_cli_set
{
private:
    testclient_callback_handler *cb_handler;
    lt_thread_server server;

public:
    testclient_client_callback(int thread_num, testclient_callback_handler *cb_handler);

private:
    void handler_by_output(lt_data_t *received_data) override;

    void handler_by_input(lt_data_t *sent_data, int error_internal) override;

protected:
    virtual void disconnected(lt_session *sess) override;
};

class testclient_client
{
private:
    testclient_client_callback *cb;
    std::mutex m;
    lt_session_cli_safe *sess;

public:
    testclient_client(testclient_client_callback *_cb);

    int connect(const std::string &ip);

    void disconnect();

    int test(IN const unsigned int &input_int, INOUT void *&internal_pri);

    int ping_internal(INOUT void *&internal_pri);

private:
    int test_gendata(IN const unsigned int &input_int, INOUT void *&internal_pri, lt_data_t *data);

    int ping_internal_gendata(lt_condition *_internal_sync_cond, INOUT void *&internal_pri, lt_data_t *data);

};
}
#endif
