#ifndef _COMMUNICATE_LT_SESSION_CLI_SET_H
#define _COMMUNICATE_LT_SESSION_CLI_SET_H

#include "lt_bidirectional_map.hpp"
#include "lt_session.h"
#include "lt_session_cli.h"
#include "lt_session_cli_safe.h"

class lt_session_cli_set
{
private:
    std::mutex              m;
    boost::asio::io_service *io_service;
    lt_session_callback     *cb;
public:
    lt_session_cli_set(boost::asio::io_service *_io_service,
                       lt_session_callback *_cb);
    
    void set_ioservice(boost::asio::io_service *_io_service);

private:
    lt_bidirectional_map<std::string, lt_session_cli_safe *> set;
public:
    void get_session(lt_session_cli_safe *sess);
    
    lt_session_cli_safe *get_session(const std::string &key);
    
    void put_session(lt_session_cli_safe *);
};

#endif //TEST_COMMUNICATE_LT_SESSION_CLI_SET_H
