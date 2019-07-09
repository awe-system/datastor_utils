#include "lt_session_dog.h"

#include "../log/include/awe_log.h"

lt_session_dog::lt_session_dog(boost::asio::io_service *_io_service) : lt_watch_dog(_io_service)
{
    AWE_MODULE_DEBUG("communicate", "lt_session_dog::lt_session_dog this %p", this);
}

lt_session_dog::~lt_session_dog()
{
    AWE_MODULE_DEBUG("communicate", "lt_session_dog::~lt_session_dog this %p", this);
}
