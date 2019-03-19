#ifndef TESTCLIENT_TESTCLIENT_H
#define TESTCLIENT_TESTCLIENT_H

#include "testclient_internal.h"

namespace testclient
{

class testclient
{
    bool to_destroy;
    
    std::thread * th;
    std::mutex conn_m;
    lt_condition discon_cond;
    std::mutex disconn_m;
    bool is_now_connected;
    bool is_user_discon;
private:
    testclient_client cli;

public:
    testclient();
    virtual ~testclient();
    void run();
    
    virtual int connect(const std::string &ip);
    void disconnect();
    void disconnect_async();

public:
    void disconnected_internal();
    virtual void disconnected();
public:
    int test(IN const unsigned int &input_int, OUT unsigned int &output_int);

};

}
#endif
