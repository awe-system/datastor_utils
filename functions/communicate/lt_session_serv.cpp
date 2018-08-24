#include "lt_session_serv.h"

lt_session_serv::lt_session_serv(boost::asio::io_service *_io_service, lt_session_callback *_cb)
        : lt_session(_io_service, _cb), cb(_cb)
{
}
