#include "lt_session_cli_set.h"

lt_session_cli_set::lt_session_cli_set(boost::asio::io_service *_io_service, lt_session_callback *_cb) :
        io_service(_io_service), cb(_cb)
{

}

void lt_session_cli_set::put_session(lt_session_cli_safe *sess)
{
    bool to_destroy = sess->put();
    if ( to_destroy )
    {
        std::unique_lock<std::mutex> lck(m);
        set.erase_val(sess);
        delete sess;
    }
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
