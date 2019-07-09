#include "lt_session_cli_safe.h"
#include "lt_session_cli_set.h"
#include "../log/include/awe_log.h"

void lt_session_cli_safe::connect(const std::string &ip, unsigned short port)
{
    try
    {
        std::unique_lock<std::mutex> lck(conn_m);
        if ( ++out_connect_ref == 1 )
        {
            connect_first(ip, port);
        }
        else if ( !is_down_connected )
        {
            reconnect(ip, port);
        }
    }
    catch (int &err)
    {
        --out_connect_ref;
        throw err;
    }
}

void lt_session_cli_safe::disconnect()
{
    if ( --out_connect_ref == 0 )
    {
        disconnect_last();
    }
}

lt_session_cli_safe::lt_session_cli_safe(lt_session_cli_set *_set,
                                         boost::asio::io_service *_io_service,
                                         lt_session_callback *_cb) :
        lt_session_cli(_io_service, (lt_session_callback *) this),
        is_down_connected(false), cb(_cb), set(_set)
{
    AWE_MODULE_DEBUG("communicate", "lt_session_cli_safe::lt_session_cli_safe %p", this);
    out_connect_ref.store(0);
}

void lt_session_cli_safe::disconnected()
{
    std::unique_lock<std::mutex> lck(conn_m);
    is_down_connected = false;
    lck.unlock();
    lt_session::disconnected();
}

void lt_session_cli_safe::connect_first(const std::string &ip, unsigned short port)
{
    lt_session_cli::connect(ip, port);
    is_down_connected = true;
}

void lt_session_cli_safe::reconnect(const std::string &ip, unsigned short port)
{
    lt_session_cli::connect(ip, port);
    is_down_connected = true;
}

void lt_session_cli_safe::disconnect_last()
{
    lt_session_cli::disconnect();
}

void lt_session_cli_safe::rcv_done(lt_session *sess, lt_data_t *received_data, int error)
{
    cb->rcv_done(sess, received_data, error);
    set->put_session(this);
}

void lt_session_cli_safe::snd_done(lt_session *sess, lt_data_t *sent_data, int error)
{
    cb->snd_done(sess, sent_data, error);
    set->put_session(this);
}

void lt_session_cli_safe::disconnected(lt_session *sess)
{
    cb->disconnected(sess);
}

void lt_session_cli_safe::connected(lt_session *sess)
{
    cb->connected(sess);
}

void lt_session_cli_safe::rcv(lt_data_t *data)
{
    AWE_MODULE_DEBUG("communicate", "before rcv lt_session_cli_safe::snd sess %p data %p", this, data);
    set->get_session(this);
    AWE_MODULE_DEBUG("communicate", "after getsess lt_session_cli_safe::snd sess %p data %p", this, data);
    lt_session::rcv(data);
    AWE_MODULE_DEBUG("communicate", "after rcv lt_session_cli_safe::snd sess %p data %p", this,data);
}

void lt_session_cli_safe::snd(lt_data_t *data)
{
    AWE_MODULE_DEBUG("communicate", "before snd lt_session_cli_safe::snd sess %p data %p", this, data);
    set->get_session(this);
    AWE_MODULE_DEBUG("communicate", "before lt_session::snd lt_session_cli_safe::snd sess %p data %p", this, data);
    lt_session::snd(data);
    AWE_MODULE_DEBUG("communicate", "after snd lt_session_cli_safe::snd sess %p data %p", this, data);
}

lt_session_cli_safe::~lt_session_cli_safe()
{
    AWE_MODULE_DEBUG("communicate", "lt_session_cli_safe::~lt_session_cli_safe %p", this);
}
