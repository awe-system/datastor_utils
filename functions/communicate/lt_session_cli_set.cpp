#include <lt_function/thread_pool.hpp>
#include "lt_session_cli_set.h"

lt_session_cli_set::lt_session_cli_set(boost::asio::io_service *_io_service, lt_session_callback *_cb) :
        io_service(_io_service), cb(_cb)
{

}

static data_channel::thread_pool delete_pool(1);

void delete_func(lt_session_cli_safe *sess)
{
    std::cout << "delete session : " << sess << std::endl;
    delete sess;
}

bool lt_session_cli_set::put_session(lt_session_cli_safe *sess)
{
    std::unique_lock<std::mutex> lck(m);
    bool to_destroy = sess->put();
    if ( to_destroy )
    {
        std::cout << "-----" << __FUNCTION__ << " " << to_destroy << " " <<
        std::this_thread::get_id() << std::endl;
        set.erase_val(sess);
        lck.unlock();
        delete_pool.submit_task(boost::bind(&delete_func, sess));
    }
    return to_destroy;
}


void lt_session_cli_set::get_session(lt_session_cli_safe *sess)
{
    sess->get();
}

lt_session_cli_safe *lt_session_cli_set::get_session(const std::string &key)
{
    lt_session_cli_safe *res = NULL;
    std::unique_lock<std::mutex> lck(m);
    std::map<std::string, lt_session_cli_safe *>::iterator it = set.k_map.find(key);
    if ( it != set.k_map.end())
    {
        res = it->second;
        res->get();
        return res;
    }
    try
    {
        res = new lt_session_cli_safe(this, io_service, cb);
        res->get();
        set.insert(key, res);
    }
    catch (...)
    {
        return NULL;
    }
    return res;
}

void lt_session_cli_set::set_ioservice(boost::asio::io_service *_io_service)
{
    io_service = _io_service;
}
