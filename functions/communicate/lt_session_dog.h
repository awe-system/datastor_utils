#ifndef _COMMUNICATE_LT_SESSION_DOG_H
#define _COMMUNICATE_LT_SESSION_DOG_H

#include "action_stub.h"
#include "lt_watch_dog.h"


class lt_session_dog : public lt_watch_dog, public action_stub
{
public:
    lt_session_dog(boost::asio::io_service *_io_service);
    virtual ~lt_session_dog() = default;
};


#endif
