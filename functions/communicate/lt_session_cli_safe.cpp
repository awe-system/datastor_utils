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
    cb->disconnected(this);
    AWE_MODULE_ERROR("communicate", "disconnected_inthread in %p", this);
}


void lt_session_cli_safe::rcv_done(lt_session *sess, lt_data_t *received_data,
                                   int error)
{
    assert_legal();
    if ( error )
    {
        AWE_MODULE_ERROR("communicate", "rcv_done err[%d] %p", error, sess);
    }
    AWE_MODULE_DEBUG("communicate",
                     "lt_session_cli_safe::rcv_done err[%d] sess[%pp received_data[%p]",
                     error, sess, received_data);
    cb->rcv_done(sess, received_data, error);
    AWE_MODULE_DEBUG("communicate",
                     "lt_session_cli_safe::rcv_done err[%d] sess[%pp received_data[%p]",
                     error, sess, received_data);
    __sync_sub_and_fetch(&pending_cnt, 1);
    set->put_session_internal(this);
}

void
lt_session_cli_safe::snd_done(lt_session *sess, lt_data_t *sent_data, int error)
{
    assert_legal();
    AWE_MODULE_DEBUG("communicate",
                     "lt_session_cli_safe::snd_done err[%d] sess[%pp sent_data[%p]",
                     error, sess, sent_data);
    cb->snd_done(sess, sent_data, error);
    AWE_MODULE_DEBUG("communicate",
                     "lt_session_cli_safe::snd_done err[%d] sess[%pp sent_data[%p]",
                     error, sess, sent_data);
    __sync_sub_and_fetch(&pending_cnt, 1);
    set->put_session_internal(this);
}

void lt_session_cli_safe::disconnected(lt_session *sess)
{
    lt_session_cli_safe *sess_this = dynamic_cast<lt_session_cli_safe *>(sess);
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
                     "before rcv lt_session_cli_safe::snd sess %p data[%p]",
                     this, data);
    if ( !is_down_connected )
    {
        cb->rcv_done(this, data, -RPC_ERROR_TYPE_NETDOWN_ALREADY);
        return;
    }
    
    set->get_session_internal(this);
    __sync_add_and_fetch(&pending_cnt, 1);
    lt_session::rcv(data);
    AWE_MODULE_DEBUG("communicate",
                     "after rcv lt_session_cli_safe::snd sess %p data[%p]",
                     this, data);
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

lt_session_cli_safe::~lt_session_cli_safe()
{
    clear();
    long pending = 0;
    do
    {
        pending = pending_cnt;
        if ( pending != 0 )
        {
            usleep(1000);
            if ( ++wait_disconn_num > MAX_DICONNECT_WAIT_NUM )
            {
                AWE_MODULE_WARN("communicate",
                                "~lt_session_cli_safe wait too long %p",
                                this);
                wait_disconn_num = 0;
            }
            continue;
        }
        else
        {
            AWE_MODULE_INFO("communicate",
                            "~lt_session_cli_safe before assert_queue_empy %p",
                            this);
            
            assert_queue_empy();
            AWE_MODULE_INFO("communicate", "~lt_session_cli_safe out");
        }
    } while ( pending );
}
