#include "lt_session.h"
#include "../lt_function_error.h"

lt_session::lt_session(boost::asio::io_service *_io_service, lt_session_callback *_cb) :
        lt_reference(),
        lt_session_dog(_io_service),
        max_wait_seconds(DEFAULT_WAIT_SECONDS),
        cb(_cb),
        io_service(_io_service),
        _socket(*_io_service),
        _connect(false, boost::bind(&lt_session::state_changed, this, _1))

{
}

void lt_session::rcv(lt_data_t *data)
{
    if ( is_connected())
        start_rcv(data);
    else
        cb->rcv_done(this, data, -RPC_ERROR_TYPE_CONNECT_FAIL);
}

void lt_session::snd(lt_data_t *data)
{
    if ( is_connected())
        start_snd_data(data);
    else
        cb->snd_done(this, data, -RPC_ERROR_TYPE_CONNECT_FAIL);
}

#define check_disconnect_when_done(error) \
do{\
    int err = boost_err_translate(error);\
    if(err)\
    {\
        let_it_down();\
        return;\
    }\
}while(0)


void lt_session::start_rcv(lt_data_t *data)
{
    rcv_queue.begin_to(boost::bind(&lt_session::start_rcv_head_unsafe, this, data),
                       boost::bind(&lt_session::rcv_done, this, data, boost::asio::error::network_down));
}

void lt_session::rcv_done(lt_data_t *data, const boost::system::error_code &error)
{
    check_disconnect_when_done(error);
    cb->rcv_done(this, data, RPC_ERROR_TYPE_OK);
    mark_received();
}

void lt_session::start_rcv_head_unsafe(lt_data_t *data)
{
    boost::asio::async_read(_socket,
                            boost::asio::buffer(&(data->_length), sizeof(data->_length)),
                            boost::bind(&lt_session::rcv_head_done_unsafe, this,
                                        data, boost::asio::placeholders::error));
}

void lt_session::rcv_head_done_unsafe(lt_data_t *data, const boost::system::error_code &error)
{
    int err = boost_err_translate(error);
    if ( err )
    {
        rcv_queue.continue_to();
        rcv_done(data, boost::asio::error::network_down);
        return;
    }

    data->realloc_buf();
    start_rcv_data_unsafe(data);
}

void lt_session::start_rcv_data_unsafe(lt_data_t *data)
{
    boost::asio::async_read(_socket,
                            boost::asio::buffer(data->get_buf(), data->_length),
                            boost::bind(&lt_session::rcv_data_done_unsafe, this,
                                        data, boost::asio::placeholders::error));
}

void lt_session::rcv_data_done_unsafe(lt_data_t *data, const boost::system::error_code &error)
{
    rcv_queue.continue_to();
    rcv_done(data, error);
}

void lt_session::let_it_up()
{
    _connect.set(true);
}

void lt_session::let_it_down()
{
    _connect.set(false);
}

void lt_session::connected()
{
    reset();
    start_monitor();
    cb->connected(this);
}

void lt_session::disconnected()
{
    cb->disconnected(this);
    queue.clear();
    _socket.close();
    stop_monitor();
}

void lt_session::start_snd_data(lt_data_t *data)
{
    queue.begin_to(boost::bind(&lt_session::start_snd_data_unsafe, this, data),
                   boost::bind(&lt_session::snd_data_done, this, data, boost::asio::error::network_down));
}

void lt_session::snd_data_done(lt_data_t *data, const boost::system::error_code &error)
{
    check_disconnect_when_done(error);
    cb->snd_done(this, data, RPC_ERROR_TYPE_OK);
    mark_sent();
}


void lt_session::start_snd_data_unsafe(lt_data_t *data)
{
    boost::asio::async_write(_socket,
                             boost::asio::buffer(data->get_data(), data->data_len()),
                             boost::bind(&lt_session::snd_data_done_unsafe, this,
                                         data, boost::asio::placeholders::error));
}

void lt_session::snd_data_done_unsafe(lt_data_t *data, const boost::system::error_code &error)
{
    queue.continue_to();  //FIXME 不立即调done，有可能引发超时
    snd_data_done(data, error);
}

void lt_session::state_changed(const bool &is_con)
{
    if ( !is_con )
        disconnected();
    else
        connected();
}

bool lt_session::is_connected() const
{
    return _connect.get();
}

void lt_session::set_max_wait_seconds(int sec)
{
    max_wait_seconds = sec;
}

bool lt_session::is_to_feed() const
{
    unsigned long since_last_snd = get_time_frame_since_last_snd();
    unsigned long since_last_rcv = get_time_frame_since_last_rcv();
    return std::min(since_last_snd, since_last_rcv) < max_wait_seconds;
}

void lt_session::handle_event()
{
    let_it_down();
}

