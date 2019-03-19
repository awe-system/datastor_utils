#ifndef TESTCLIENT_TESTCLIENT_H
#define TESTCLIENT_TESTCLIENT_H

#include "testclient_internal.h"

namespace testclient
{

class testclient
{

private:
    bool to_destroy;
    std::thread * th;
    lt_condition discon_cond;
    std::mutex disconn_m;
    bool is_now_connected;
    bool is_user_discon;
    testclient_client cli;

public:
    testclient();
    ~testclient();
    void run();
    int connect(const std::string &ip);
    void disconnect_async();
    void disconnect();

public:
    void disconnected_internal();
    virtual void disconnected();

public:
    int test(IN const unsigned int &input_int, OUT unsigned int &output_int);

};

}
#endif
