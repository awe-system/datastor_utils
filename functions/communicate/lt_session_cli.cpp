#include "lt_session_cli.h"
#include "../lt_function_error.h"
#include "../log/include/awe_log.h"

lt_session_cli::lt_session_cli(boost::asio::io_service *_io_service, lt_session_callback *cb) :
        lt_session(_io_service, cb)
{
    AWE_MODULE_INFO("comunicate","struct lt_session_cli %p", this);
}

void lt_session_cli::connect(const std::string &ip, unsigned short port)
{
    boost::asio::ip::tcp::endpoint endpoint =
            boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port);
    boost::system::error_code error;
    _socket.connect(endpoint, error);
    int err = boost_err_translate(error);
    if ( err )
    {
        throw err;
    }
    let_it_up();
}


void lt_session_cli::connected()
{
    lt_session::connected();
}

void lt_session_cli::disconnect()
{
    AWE_MODULE_DEBUG("communicate", "--enter lt_session_cli::disconnect sess %p", this);
    let_it_down();
    AWE_MODULE_DEBUG("communicate", "--leave lt_session_cli::disconnect sess %p", this);
}

lt_session_cli::~lt_session_cli()
{
    AWE_MODULE_INFO("comunicate","~lt_session_cli %p", this);
}

