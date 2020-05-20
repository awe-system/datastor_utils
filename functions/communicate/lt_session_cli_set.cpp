#include <lt_function/thread_pool.hpp>
#include <awe_log.h>
#include "lt_session_cli_set.h"

lt_session_cli_set::lt_session_cli_set(boost::asio::io_service *_io_service, lt_session_callback *_cb) :
        io_service(_io_service), cb(_cb)
{

}

static data_channel::thread_pool delete_pool(1);

void delete_func(lt_session_cli_safe *sess)
{
    delete sess;
}

bool lt_session_cli_set::put_session_internal(lt_session_cli_safe *sess)
{
    bool to_destroy = sess->put();
    if ( to_destroy )
    {
//        std::cout << "-----" << __FUNCTION__ << " " << to_destroy << " " <<
//        std::this_thread::get_id() << std::endl;
        delete_pool.submit_task(boost::bind(&delete_func, sess));
    }
    return to_destroy;
}


void lt_session_cli_set::get_session_internal(lt_session_cli_safe *sess)
{
    sess->get();
}

lt_session_cli_safe *lt_session_cli_set::get_session(const std::string &key)
{
    AWE_MODULE_INFO("communicate",
                     "get_session key %s", key.c_str());
    lt_session_cli_safe *sess =  new lt_session_cli_safe(this, io_service, cb);
    
    AWE_MODULE_INFO("communicate",
                    "get_session key [%s] sess[%p]", key.c_str(),sess);
    sess->get();
    return sess;
}

void lt_session_cli_set::set_ioservice(boost::asio::io_service *_io_service)
{
    io_service = _io_service;
}

bool lt_session_cli_set::put_session(lt_session_cli_safe *sess)
{
    AWE_MODULE_INFO("communicate",
                    "put_session  sess[%p]",sess);
    return put_session_internal(sess);
}
