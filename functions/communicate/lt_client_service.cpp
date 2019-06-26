#include "lt_client_service.h"
#include "../lt_function_error.h"
#include "../log/include/awe_log.h"
//NOTE: 前提是disconect被调用后不会出现新的rcv_done
void lt_client_service::rcv_done(lt_session *sess, lt_data_t *received_data, int error)
{
    AWE_MODULE_DEBUG("communicate", "enter lt_client_service::rcv_done %p error %d", sess, error);
    if( !rcvdata_set.erase(received_data))
    {
        AWE_MODULE_DEBUG("communicate", "leave lt_client_service::rcv_done %p error no received_data", sess);
        return;
    }
    lt_data_t *sent_data = (lt_data_t *) received_data->pop_private();

    if ( error )
    {
        AWE_MODULE_DEBUG("communicate", "before handler_by_input %p error %d", sess, error);
        handler_by_input(sent_data, error);
        AWE_MODULE_DEBUG("communicate", "after handler_by_input %p error %d", sess, error);
    }
    else
    {
        AWE_MODULE_DEBUG("communicate", "before handler_by_output %p no error", sess);
        handler_by_output(received_data);
        AWE_MODULE_DEBUG("communicate", "before handler_by_output %p no error", sess);
    }
    delete sent_data;
    delete received_data;
}

void lt_client_service::snd_done(lt_session *sess, lt_data_t *sent_data, int error)
{
   
    if ( error )
    {
        AWE_MODULE_DEBUG("communicate", "before snd_done handler_by_input sess [%p] error %d", sess, error);
        handler_by_input(sent_data, error);
        AWE_MODULE_DEBUG("communicate", "after snd_done handler_by_input sess [%p] error %d", sess, error);
        delete sent_data;
        return;
    }
    AWE_MODULE_DEBUG("communicate", "enter lt_client_service::snd_done sess [%p] snddone no err", sess);
    lt_data_t *received_data = new lt_data_t();
    received_data->push_private(sent_data);

    lt_session_cli_safe *session = (lt_session_cli_safe *) sess;
    assert(rcvdata_set.insert(received_data));
    session->rcv(received_data);
    AWE_MODULE_DEBUG("communicate", "leave lt_client_service::snd_done sess [%p] snddone no err", sess);
}

void lt_client_service::disconnected(lt_session *sess)
{
    AWE_MODULE_DEBUG("communicate", "enter lt_client_service::disconnected sess [%p]", sess);
    lt_data_t * rcv_data;
    while(rcvdata_set.first(rcv_data))
    {
        AWE_MODULE_DEBUG("communicate", "before  rcv_done sess [%p]", sess);
        rcv_done(sess, rcv_data, -RPC_ERROR_TYPE_NET_BROKEN);
        AWE_MODULE_DEBUG("communicate", "after rcv_done %p", sess);
    }
}

void lt_client_service::connected(lt_session *sess)
{//NOTE:do nothing
}

lt_client_service::lt_client_service(boost::asio::io_service *_io_service, unsigned short port)
{
}

int lt_client_service::snd(lt_session_cli_safe *sess, boost::function<int(lt_data_t *)> gendata_f)
{
    lt_data_t *data = NULL;
    try
    {
        lt_data_t *data = new lt_data_t;
        int err = gendata_f(data);
        if ( err )
            throw err;
        sess->snd(data);
    }
    catch (...)
    {
        AWE_MODULE_ERROR("communicate", "lt_client_service::snd sess %p", sess);
        delete data;
        return -RPC_ERROR_TYPE_MEMORY;
    }
    return 0;
}

void lt_client_service::set_ioservice(boost::asio::io_service *_io_service)
{
    io_service = _io_service;
}
