#include "testserver.h"

namespace testserver
{

#define SERVER_PORT    7777

class test_context : public lt_session_description
{
public:
    void set_session_private(void *pri) override
    {
        sess->set_session_private(pri);
    }

    void *get_session_private() const override
    {
        return sess->get_session_private();
    }
    
    bool is_connected() const override
    {
        return sess->is_connected();
    }
    
    void *cli_pri;
    lt_data_t *request_data;
    lt_session_serv *sess;
};

server::server(int thread_num, server_handler *_handler) :
        service(thread_num, SERVER_PORT, this), handler(_handler)
{
    service.start_accept();
}

void server::do_func(lt_data_t *received_data, lt_session_serv *sess)
{
    lt_data_t *request_data = new lt_data_t();
    *request_data = *received_data;
    unsigned char *buf = request_data->get_buf();
    unsigned int func_type = lt_data_translator::to_uint(buf);
    switch ( func_type )
    {
        case server_function_callback_type_7777_test:
        {
            unsigned int input_int = lt_data_translator::to_uint(buf);
            test_context *context = new test_context();
            context->sess = sess;
            context->request_data = request_data;
            context->cli_pri = lt_data_translator::to_void_p(buf);
            int error_internal = handler->test(input_int, (lt_session_description  *) context);
        }
            break;
        case server_function_callback_type_7777_ping_internal:
        {
            void *internal_sync_cond_p = lt_data_translator::to_void_p(buf);
            void * internal_pri = lt_data_translator::to_void_p(buf);
            int error_internal = handler->ping_internal();
            service.snd(sess, boost::bind(&server::ping_internal_gendata, this, error_internal, internal_sync_cond_p, internal_pri, _1));
            delete request_data;
        }
            break;
        default:
            abort();
    }
}
int server::test_done_gendata(unsigned int output_int, lt_session_description *server_context, int error_internal, lt_data_t *data)
{
    test_context *context = (test_context *)server_context;
    unsigned int func_type = client_function_callback_type_7777_test;
    data->_length = sizeof(func_type) + sizeof(unsigned int) + sizeof(void *) + sizeof(void *) + sizeof(error_internal);
    data->realloc_buf();
    unsigned char *res_buf = data->get_buf();
    lt_data_translator::by_uint(func_type, res_buf);
    lt_data_translator::by_uint(output_int, res_buf);
    lt_data_translator::by_uint(error_internal, res_buf);
    lt_data_translator::by_void_p(context->cli_pri, res_buf);
    return 0;
}

int server::ping_internal_gendata(int error_internal, void *internal_sync_cond_p, void *internal_pri,lt_data_t *data)
{
    unsigned int func_type = client_function_callback_type_7777_ping_internal;
    data->_length = sizeof(func_type) + sizeof(int) + sizeof(void *) + sizeof(void *);
    data->realloc_buf();
    unsigned char *res_buf = data->get_buf();
    lt_data_translator::by_uint(func_type, res_buf);
    lt_data_translator::by_uint(error_internal, res_buf);
    lt_data_translator::by_void_p(internal_sync_cond_p, res_buf);
    lt_data_translator::by_void_p(internal_pri, res_buf);
    return 0;
}

void server::test_done(unsigned int &output_int, lt_session_description *server_context, int error_internal)
{
    test_context *context = (test_context *) server_context;
    lt_session_serv *sess = context->sess;
    int err_internal = service.snd(sess, boost::bind(&server::test_done_gendata, this, output_int, server_context, error_internal, _1));
    lt_data_t *request_data = context->request_data;
    delete request_data;
    delete context;
}


}
