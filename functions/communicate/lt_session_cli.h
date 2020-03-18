#ifndef _COMMUNICATE_LT_SESSION_CLI_H
#define _COMMUNICATE_LT_SESSION_CLI_H

#include "lt_session.h"

class lt_session_cli : public lt_session
{
public:
    virtual ~lt_session_cli();
    lt_session_cli(boost::asio::io_service *_io_service, lt_session_callback *cb);

    virtual void connect(const std::string &ip, unsigned short port);

    virtual void disconnect();

protected:
    void connected();
};


#endif
