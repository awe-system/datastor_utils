#include <lt_data_translator.h>
#include "testclient.h"
#include "awe_conf/env.h"

namespace testclient
{
env testclient_threadnum("test_comm", "testclient_threadnum");
class testclient_thread_handler : public testclient_callback_handler
{
private:
    std::mutex              connected_list_m;
    std::list<testclient *> connected_list;
public:

    void test_callback(OUT unsigned int &output_int, INOUT void *&internal_pri, OUT int error_internal) override
    {
        lt_data_t res_data(sizeof(output_int));
        unsigned char *res_buf = res_data.get_buf();
        lt_data_translator::by_uint(output_int, res_buf);

        lt_condition *_internal_sync_cond = (lt_condition *) internal_pri;
        _internal_sync_cond->notify(res_data, error_internal);
    }
    void addcli_to_event(testclient *cli)
    {
        std::unique_lock<std::mutex> lck(connected_list_m);
        connected_list.push_back(cli);
    }

    void disconnected(lt_session *sess) override
    {
        std::unique_lock<std::mutex> lck(connected_list_m);
        std::list<testclient *>      org_connected_list = connected_list;
        connected_list.clear();
        while ( !org_connected_list.empty() )
        {
            testclient *cli = org_connected_list.front();
            org_connected_list.pop_front();
            cli->disconnected_internal();
        }
    }

};

static testclient_thread_handler handler;
static testclient_client_callback cb(max(testclient_threadnum.get_int(),1), &handler);
void *test_ping_func(testclient *cli);
testclient::testclient() : cli(&cb),is_user_discon(false),
        is_now_connected(false),to_destroy(false)
{
    th = new thread(test_ping_func, this);
}

void *test_ping_func(testclient *cli)
{
    cli->run();
    return nullptr;
}

int testclient::connect(const std::string &ip)
{
    is_user_discon = false;
    int err = cli.connect(ip);
    if ( !err )
    {
        handler.addcli_to_event(this);
        is_now_connected = true;
    }
    return err;
}

void testclient::disconnect_async()
{
    cli.disconnect();
}

void testclient::disconnect()
{
    std::unique_lock<std::mutex> lck(disconn_m);
    if ( is_now_connected )
    {
        is_user_discon = true;
        lck.unlock();
        cli.disconnect();
        discon_cond.wait();
    }
    else
    {
        lck.unlock();
        cli.disconnect();
    }
}

void testclient::disconnected_internal()
{
    std::unique_lock<std::mutex> lck(disconn_m);
    is_now_connected = false;
    if ( is_user_discon )
    {
        lck.unlock();
        discon_cond.notify();
    }
    else
    {
        lck.unlock();
        disconnected();
    }
}

void testclient::disconnected()
{
}

testclient::~testclient ()
{
    to_destroy = true;
    th->join();
    delete th;
}

void testclient::run()
{
    while(!to_destroy)
    {
        if(is_now_connected)
        {
            ping_internal();
        }
        sleep(min(DEFAULT_WAIT_SECONDS/3,3));
    }
}

int testclient::test(IN const unsigned int &input_int, OUT unsigned int &output_int)
{
    lt_condition _internal_sync_cond;
    void *internal_pri = (void *) &_internal_sync_cond;

    int err_internal =  cli.test(input_int, internal_pri);
    if(err_internal) return err_internal;

    int error_internal = _internal_sync_cond.wait();
    if(error_internal) return error_internal;

    const lt_data_t &res_data = _internal_sync_cond.get_data();
    unsigned char *buf = res_data.get_buf();

    output_int = lt_data_translator::to_uint(buf);

    return error_internal;

}

int testclient::ping_internal()
{
    void *internal_pri = (void *) this;
    return cli.ping_internal(internal_pri);
}

};

