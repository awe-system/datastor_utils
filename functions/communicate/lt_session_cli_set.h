#ifndef _COMMUNICATE_LT_SESSION_CLI_SET_H
#define _COMMUNICATE_LT_SESSION_CLI_SET_H

#include "lt_bidirectional_map.hpp"
#include "lt_session.h"
#include "lt_session_cli.h"
#include "lt_session_cli_safe.h"

class lt_session_cli_set
{
private:
    boost::asio::io_service *io_service;
    lt_session_callback     *cb;
public:
    lt_session_cli_set(boost::asio::io_service *_io_service,
                       lt_session_callback *_cb);
    
    void set_ioservice(boost::asio::io_service *_io_service);
    
public:
    void get_session_internal(lt_session_cli_safe *sess);
    
    bool put_session_internal(lt_session_cli_safe *sess);
    
public:
    lt_session_cli_safe *get_session(const std::string &key);
    bool put_session(lt_session_cli_safe *sess);
};

#endif //TEST_COMMUNICATE_LT_SESSION_CLI_SET_H
