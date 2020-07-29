#include <lt_function/thread_pool.hpp>
#include "lt_server_service.h"
#include "../lt_function_error.h"
#include "lt_session_cli_safe.h"


void lt_server_service::connected(lt_session *sess)
{
    lt_session_serv *session = (lt_session_serv *) sess;
    lt_data_t *data = new lt_data_t();
    session->rcv(data);
}

void lt_server_service::rcv_done(lt_session *sess, lt_data_t *received_data, int error)
{
    lt_session_serv *session = (lt_session_serv *) sess;
    if ( error )
    {
        put_session(session);
        delete received_data;
        return;
    }
    cb->do_func(received_data, session);
    sess->rcv(received_data);
}

int lt_server_service::snd(lt_session_serv *sess, lt_data_t *data)
{
    get_session(sess)->snd(data);
    return RPC_ERROR_TYPE_OK;
}

void lt_server_service::snd_done(lt_session *sess, lt_data_t *sent_data, int error)
{
    lt_session_serv *session = (lt_session_serv *) sess;
    put_session(session);
    delete sent_data;
}

void lt_server_service::disconnected(lt_session *sess)
{
    lt_session_description *session = (lt_session_description *) sess;
    cb->disconnected(session);
}


void lt_server_service::start_accept()
{
    lt_session_serv *sess = get_session();
    _acceptor.async_accept(sess->_socket, boost::bind(&lt_server_service::accepted, this, sess, boost::asio::placeholders::error));
}

void lt_server_service::accepted(lt_session_serv *sess, const boost::system::error_code &error)
{
    int err = boost_err_translate(error);
    if ( err )
    {
        return;
    }
    sess->let_it_up();

    start_accept();
}

lt_server_service::lt_server_service(int thread_num, unsigned short _port, lt_server_callback *_cb) :
        port(_port), server(thread_num), cb(_cb),
        end(boost::asio::ip::tcp::v4(), port),
        _acceptor(*(server.get_io_service()), end)
{
}

static data_channel::thread_pool ser_delete_pool(1);

void ser_delete_func(lt_session_serv *sess)
{
    delete sess;
}

void lt_server_service::put_session(lt_session_serv *sess)
{
    if ( sess->put())
    {
        ser_delete_pool.submit_task(boost::bind(&ser_delete_func, sess));
    }
}

lt_session_serv *lt_server_service::get_session(lt_session_serv *sess)
{
    sess->get();
    return sess;
}

lt_session_serv *lt_server_service::get_session()
{
    lt_session_serv *sess = new lt_session_serv(server.get_io_service(), this);
    assert(sess);
    sess->get();
    return sess;
}
