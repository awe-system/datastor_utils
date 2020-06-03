#include <lt_function_error.h>
#include <lt_function/thread_pool.hpp>
#include "lt_session_cli_safe.h"
#include "lt_session_cli_set.h"
#include "../log/include/awe_log.h"

static data_channel::thread_pool discon_pool(CLI_DISCONNECT_WAIT_POOL_NUM);

void lt_session_cli_safe::connect(const std::string &ip, unsigned short port)
{
    
    assert(++connect_cnt == 1);
    lt_session_cli::connect(ip, port);
    is_down_connected = true;
}

void lt_session_cli_safe::disconnect()
{
    assert(++disconnect_cnt == 1);
    lt_session_cli::disconnect();
}

lt_session_cli_safe::lt_session_cli_safe(lt_session_cli_set *_set,
                                         boost::asio::io_service *_io_service,
                                         lt_session_callback *_cb) :
        lt_session_cli(_io_service, (lt_session_callback *) this),
        is_down_connected(false), cb(_cb), set(_set), pending_cnt(0)
{
}

void lt_session_cli_safe::disconnected_inthread()
{
    assert_legal();
    
    AWE_MODULE_ERROR("communicate", "disconnected_inthread in %p", this);
    long pending = pending_cnt;
    if ( pending != 0 )
    {
        usleep(1000);
        if ( ++wait_disconn_num > MAX_DICONNECT_WAIT_NUM )
        {
            AWE_MODULE_WARN("communicate",
                            "disconnected_inthread wait too long %p",
                            this);
            wait_disconn_num = 0;
        }
        
        discon_pool.submit_task(
                boost::bind(&lt_session_cli_safe::disconnected_inthread,
                            this));
        
    }
    else
    {
        AWE_MODULE_INFO("communicate",
                        "disconnected_inthread before notice %p",
                        this);
        cb->disconnected(this);
        AWE_MODULE_INFO("communicate", "disconnected_inthread out");
    }
}


void lt_session_cli_safe::rcv_done(lt_session *sess, lt_data_t *received_data,
                                   int error)
{
    assert_legal();
    cb->rcv_done(sess, received_data, error);
    __sync_sub_and_fetch(&pending_cnt, 1);
    set->put_session_internal(this);
}

void
lt_session_cli_safe::snd_done(lt_session *sess, lt_data_t *sent_data, int error)
{
    assert_legal();
    cb->snd_done(sess, sent_data, error);
    __sync_sub_and_fetch(&pending_cnt, 1);
    set->put_session_internal(this);
}

void lt_session_cli_safe::disconnected(lt_session *sess)
{
    lt_session_cli_safe * sess_this = dynamic_cast<lt_session_cli_safe *>(sess);
    assert(sess == sess_this);
    is_down_connected = false;
    AWE_MODULE_INFO("communicate",
                    "disconnected_inthread post %p ", this);
    discon_pool.submit_task(
            boost::bind(&lt_session_cli_safe::disconnected_inthread, this));
}

void lt_session_cli_safe::connected(lt_session *sess)
{
    cb->connected(sess);
}

void lt_session_cli_safe::rcv(lt_data_t *data)
{
    AWE_MODULE_DEBUG("communicate",
                     "before rcv lt_session_cli_safe::snd sess %p", this);
    if ( !is_down_connected )
    {
        cb->rcv_done(this, data, -RPC_ERROR_TYPE_NETDOWN_ALREADY);
        return;
    }
    
    set->get_session_internal(this);
    __sync_add_and_fetch(&pending_cnt, 1);
    lt_session::rcv(data);
    AWE_MODULE_DEBUG("communicate",
                     "after rcv lt_session_cli_safe::snd sess %p", this);
}

void lt_session_cli_safe::snd(lt_data_t *data)
{
    AWE_MODULE_DEBUG("communicate",
                     "before snd lt_session_cli_safe::snd sess %p", this);
    if ( !is_down_connected )
    {
        cb->snd_done(this, data, -RPC_ERROR_TYPE_NETDOWN_ALREADY);
        return;
    }
    
    set->get_session_internal(this);
    __sync_add_and_fetch(&pending_cnt, 1);
    lt_session::snd(data);
    AWE_MODULE_DEBUG("communicate",
                     "after snd lt_session_cli_safe::snd sess %p", this);
}
