#include "lt_server_service.h"


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


int lt_server_service::snd(lt_session_serv *sess, boost::function<int(lt_data_t *)> gendata_f)
{
    lt_data_t *data = NULL;
    try
    {
        data = new lt_data_t();
        int err = gendata_f(data);
        if ( err )
            throw err;
        get_session(sess)->snd(data);

    } catch (...)
    {
        delete data;
        return -RPC_ERROR_TYPE_MEMORY;
    }
}

void lt_server_service::snd_done(lt_session *sess, lt_data_t *sent_data, int error)
{
    lt_session_serv *session = (lt_session_serv *) sess;
    put_session(session);
    delete sent_data;
}

void lt_server_service::disconnected(lt_session *sess)
{
    //do_nothing
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
        disconnected(sess);
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

void lt_server_service::put_session(lt_session_serv *sess)
{
    if ( sess->put())
    {
        delete sess;
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
