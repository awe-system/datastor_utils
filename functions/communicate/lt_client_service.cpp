#include "lt_client_service.h"
#include "../lt_function_error.h"
#include "../log/include/awe_log.h"

//NOTE: 前提是disconect被调用后不会出现新的rcv_done
void
lt_client_service::rcv_done_inthread(lt_session *sess, lt_data_t *received_data,
                                     int error)
{
    AWE_MODULE_DEBUG("communicate",
                     "<<<<<<<<<<<<<<<<<<<<<<<<lt_client_service::rcv_done_inthread before lock sess [%p] snddone no err received_data %p",
                     sess, received_data);
    std::unique_lock<std::mutex> lck(m);
    AWE_MODULE_DEBUG("communicate",
                     ">>>>>>>>>>>>>>>>>>>>>>>>lt_client_service::rcv_done_inthread after lock sess [%p] snddone no err received_data %p",
                     sess, received_data);
    rcv_done_nolock(sess, received_data, error);
    AWE_MODULE_DEBUG("communicate",
                     ">>>>>>>>>>>>>>>>>>>>>>>>lt_client_service::rcv_done_inthread after rcv_done_nolock sess [%p] snddone no err",
                     sess);
}


void lt_client_service::rcv_done(lt_session *sess, lt_data_t *received_data,
                                 int error)
{
    AWE_MODULE_DEBUG("communicate",
                     "<<<<<<<<<<<<<<<<<<<<<<<<lt_client_service::rcv_done before post sess [%p] snddone no err received_data %p",
                     sess, received_data);
    pool.submit_task(
            boost::bind(&lt_client_service::rcv_done_inthread, this, sess,
                        received_data, error));
    AWE_MODULE_DEBUG("communicate",
                     ">>>>>>>>>>>>>>>>>>>>>>>>lt_client_service::rcv_done after post sess [%p] snddone no err received_data %p",
                     sess, received_data);
}


void
lt_client_service::rcv_done_nolock(lt_session *sess, lt_data_t *received_data,
                                   int error)
{
    AWE_MODULE_DEBUG("communicate",
                     "enter lt_client_service::rcv_done %p error %d received_data %p",
                     sess, error, received_data);
    if ( !rcvdata_set.erase(received_data) )
    {
        AWE_MODULE_DEBUG("communicate",
                         "leave lt_client_service::rcv_done %p error no received_data %p",
                         sess, received_data);
        return;
    }
    lt_data_t *sent_data = (lt_data_t *) (received_data->pop_private());
    
    if ( error )
    {
        AWE_MODULE_DEBUG("communicate",
                         "before handler_by_input %p error %d sent_data %p received_data %p",
                         sess, error, sent_data, received_data);
        handler_by_input(sent_data, error);
        AWE_MODULE_DEBUG("communicate",
                         "after handler_by_input %p error %d sent_data %p received_data %p",
                         sess, error, sent_data, received_data);
    }
    else
    {
        AWE_MODULE_DEBUG("communicate",
                         "before handler_by_output %p no error received_data [%p] sent_data [%p]",
                         sess, received_data, sent_data);
        handler_by_output(received_data);
        AWE_MODULE_DEBUG("communicate",
                         "before handler_by_output %p no error received_data [%p] sent_data [%p]",
                         sess, received_data, sent_data);
    }
    AWE_MODULE_DEBUG("communicate",
                     "before memest received_data %p no error received_data [%p] data[%p] data_len [%lu] sent_data [%p] ",
                     sess, received_data, received_data->get_data(),
                     received_data->data_len(), sent_data);
    if ( received_data->get_data() )
    {
        memset(received_data->get_data(), 0xcc, received_data->data_len());
    }
    AWE_MODULE_DEBUG("communicate",
                     "before memest sent_data %p no error received_data [%p] sent_data [%p]",
                     sess, received_data, sent_data);
    memset(sent_data->get_data(), 0xdd, sent_data->data_len());
    delete sent_data;
    delete received_data;
}

void
lt_client_service::snd_done_inthread(lt_session *sess, lt_data_t *sent_data,
                                     int error)
{
    AWE_MODULE_DEBUG("communicate",
                     "<<<<<<<<<<<lt_client_service::snd_done_inthread before lock sess [%p] snddone no err sent_data %p",
                     sess, sent_data);
    std::unique_lock<std::mutex> lck(m);
    AWE_MODULE_DEBUG("communicate",
                     ">>>>>>>>>>lt_client_service::snd_done_inthread after lock sess [%p] snddone no err sent_data %p",
                     sess, sent_data);
    
    if ( error )
    {
        AWE_MODULE_DEBUG("communicate",
                         "before snd_done_inthread handler_by_input sess [%p] error %d sent_data %p",
                         sess, error, sent_data);
        handler_by_input(sent_data, error);
        AWE_MODULE_DEBUG("communicate",
                         "after snd_done_inthread handler_by_input sess [%p] error %d sent_data %p",
                         sess, error, sent_data);
        delete sent_data;
        return;
    }
    AWE_MODULE_DEBUG("communicate",
                     "enter lt_client_service::snd_done_inthread sess [%p] snddone no err",
                     sess);
    lt_data_t *received_data = new lt_data_t();
    received_data->push_private(sent_data);
    
    lt_session_cli_safe *session = (lt_session_cli_safe *) sess;
    
    if ( rcvdata_set.insert(received_data) )
    {
        AWE_MODULE_DEBUG("communicate",
                         "before rcv lt_client_service::snd_done_inthread sess [%p] snddone no err received_data [%p] data [%p] sent_data [%p]",
                         sess, received_data,received_data->get_data(), sent_data);
        session->rcv(received_data);
        AWE_MODULE_DEBUG("communicate",
                         "after rcv lt_client_service::snd_done_inthread sess [%p] snddone no err received_data [%p] sent_data [%p]",
                         sess, received_data, sent_data);
    }
    else
    {
        AWE_MODULE_DEBUG("communicate",
                         "!!!!!!!!!!!same received data in set lt_client_service::snd_done sess [%p] snddone no err received_data [%p] sent_data [%p]",
                         sess, received_data, sent_data);
    }
    
    AWE_MODULE_DEBUG("communicate",
                     "leave lt_client_service::snd_done_inthread sess [%p] snddone no err received_data [%p] sent_data [%p]",
                     sess, received_data, sent_data);
}

void
lt_client_service::snd_done(lt_session *sess, lt_data_t *sent_data, int error)
{
    AWE_MODULE_DEBUG("communicate",
                     "<<<<<<<<<<<lt_client_service::snd_done before post sess [%p] snddone no err sent_data %p",
                     sess, sent_data);
    pool.submit_task(
            boost::bind(&lt_client_service::snd_done_inthread, this, sess,
                        sent_data, error));
}

void lt_client_service::disconnected(lt_session *sess)
{
    AWE_MODULE_DEBUG("communicate",
                     "<====================       enter lt_client_service::disconnected sess [%p]",
                     sess);
    std::unique_lock<std::mutex> lck(m);
    AWE_MODULE_DEBUG("communicate",
                     "lt_client_service::disconnected after lock sess [%p] ",
                     sess);
    lt_data_t *rcv_data;
    while ( rcvdata_set.first(rcv_data) )
    {
        AWE_MODULE_DEBUG("communicate",
                         "before  rcv_done sess [%p] rcv_data %p", sess,
                         rcv_data);
        rcv_done_nolock(sess, rcv_data, -RPC_ERROR_TYPE_NET_BROKEN);
        AWE_MODULE_DEBUG("communicate", "after rcv_done %p rcv_data %p", sess,
                         rcv_data);
    }
    
    AWE_MODULE_DEBUG("communicate",
                     "=================>      lt_client_service::disconnected sess [%p]",
                     sess);
}

void lt_client_service::connected(lt_session *sess)
{//NOTE:do nothing
}

lt_client_service::lt_client_service(boost::asio::io_service *_io_service,
                                     unsigned short port) : pool(1)
{
}

int lt_client_service::snd(lt_session_cli_safe *sess,
                           boost::function<int(lt_data_t *)> gendata_f)
{
    lt_data_t *data = NULL;
    try
    {
        AWE_MODULE_DEBUG("communicate",
                         "$$$$$$$$$lt_client_service::snd sess %p this %p",
                         sess, this);
        lt_data_t *data = new lt_data_t();
        AWE_MODULE_DEBUG("communicate",
                         "$$$$$$$$$ after lt_client_service::snd sess %p this %p data %p",
                         sess, this, data);
        int err = gendata_f(data);
        if ( err )
        {
            throw err;
        }
        AWE_MODULE_DEBUG("communicate",
                         "############ before %p->snd(%p) this %p",
                         sess, data, this);
        sess->snd(data);
        AWE_MODULE_DEBUG("communicate",
                         "############ after %p->snd(%p) this %p",
                         sess, data, this);
    }
    catch (...)
    {
        AWE_MODULE_ERROR("communicate", "lt_client_service::snd sess %p -RPC_ERROR_TYPE_MEMORY", sess);
        delete data;
        return -RPC_ERROR_TYPE_MEMORY;
    }
    return 0;
}

void lt_client_service::set_ioservice(boost::asio::io_service *_io_service)
{
    io_service = _io_service;
}

