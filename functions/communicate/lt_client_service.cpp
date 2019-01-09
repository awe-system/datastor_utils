#include "lt_client_service.h"
#include "../lt_function_error.h"

void lt_client_service::rcv_done(lt_session *sess, lt_data_t *received_data, int error)
{
    lt_data_t *sent_data = (lt_data_t *) received_data->pop_private();

    if ( error )
    {
        handler_by_input(sent_data, error);
    }
    else
    {
        handler_by_output(received_data);
    }
    delete sent_data;
    delete received_data;
}

void lt_client_service::snd_done(lt_session *sess, lt_data_t *sent_data, int error)
{
    if ( error )
    {
        handler_by_input(sent_data, error);
        delete sent_data;
        return;
    }

    lt_data_t *received_data = new lt_data_t();
    received_data->push_private(sent_data);

    lt_session_cli_safe *session = (lt_session_cli_safe *) sess;
    session->rcv(received_data);

}

void lt_client_service::disconnected(lt_session *sess)
{//NOTE:do nothing
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
        delete data;
        return -RPC_ERROR_TYPE_MEMORY;
    }
    return 0;
}

void lt_client_service::set_ioservice(boost::asio::io_service *_io_service)
{
    io_service = _io_service;
}
