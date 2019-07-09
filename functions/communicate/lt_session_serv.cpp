#include "lt_session_serv.h"
#include "../log/include/awe_log.h"

lt_session_serv::lt_session_serv(boost::asio::io_service *_io_service, lt_session_callback *_cb)
        : lt_session(_io_service, _cb), cb(_cb)
{
    AWE_MODULE_DEBUG("communicate", "lt_session_serv::lt_session_serv %p", this);
    
}

lt_session_serv::~lt_session_serv()
{
    AWE_MODULE_DEBUG("communicate", "lt_session_serv::~lt_session_serv %p", this);
}
