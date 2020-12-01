#include "lt_session_dog.h"

lt_session_dog::lt_session_dog(boost::asio::io_service *_io_service) : lt_watch_dog(_io_service),action_stub()
{
}
