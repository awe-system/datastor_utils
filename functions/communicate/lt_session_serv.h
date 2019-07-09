#ifndef _COMMUNICATE_LT_SESSION_SERV_H
#define _COMMUNICATE_LT_SESSION_SERV_H

#include <boost/function.hpp>
#include "lt_session.h"

class lt_session_serv : public lt_session
{
private:
    lt_session_callback *cb;
public:
    lt_session_serv(boost::asio::io_service *_io_service, lt_session_callback *_cb);
    virtual ~lt_session_serv();
};


#endif
