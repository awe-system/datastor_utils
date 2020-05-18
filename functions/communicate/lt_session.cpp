#include "lt_session.h"
#include "../lt_function_error.h"
#include "../log/include/awe_log.h"

lt_session::lt_session(boost::asio::io_service *_io_service, lt_session_callback *_cb) :
        lt_reference(),
        lt_session_dog(_io_service),
        max_wait_seconds(DEFAULT_WAIT_SECONDS),
        cb(_cb),
        io_service(_io_service),
        _socket(*_io_service),
        _connect(false, boost::bind(&lt_session::state_changed, this, _1))
{
    AWE_MODULE_INFO("comunicate","struct lt_session %p", this);
    std::cout << "session : this : " << __FUNCTION__ << this << std::endl;
}

void lt_session::rcv(lt_data_t *data)
{
    assert_legal();
    if ( is_connected())
        start_rcv(data);
    else
    {
        AWE_MODULE_ERROR("comunicate","lt_session::rcv_done %p err [%d]", this, -RPC_ERROR_TYPE_CONNECT_FAIL);
        cb->rcv_done(this, data, -RPC_ERROR_TYPE_CONNECT_FAIL);
    }
    
}

void lt_session::snd(lt_data_t *data)
{
    assert_legal();
    AWE_MODULE_DEBUG("communicate", "enter lt_session::snd sess %p", this);
    if ( is_connected())
    {
        start_snd_data(data);
    }
    else
    {
        cb->snd_done(this, data, -RPC_ERROR_TYPE_CONNECT_FAIL);
    }
    AWE_MODULE_DEBUG("communicate", "leave lt_session::snd sess %p", this);
}


void lt_session::start_rcv(lt_data_t *data)
{
    assert_legal();
    if (is_connected()) {
        rcv_queue.begin_to(boost::bind(&lt_session::start_rcv_head_unsafe, this, data),
                           boost::bind(&lt_session::rcv_done, this, data, boost::asio::error::network_down));
    }
    else {
        AWE_MODULE_INFO("communicate", "rcv_done sess %p error network_down", this);
        rcv_done(data, boost::asio::error::network_down);
    }
}

void lt_session::rcv_done(lt_data_t *data, const boost::system::error_code error)
{
    assert_legal();
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::rcv_done sess %p", this);
    unsigned err = 0;
    if (error || (!is_connected())) {
        err = -RPC_ERROR_TYPE_NET_BROKEN;
        let_it_down();
        rcv_queue.clear();
    } else {
        mark_received();
    }
    
    if(error)
    {
        AWE_MODULE_ERROR("comunicate","lt_session::rcv_done %p err [%d]", this, err);
    }
    cb->rcv_done(this, data, err);
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::rcv_done sess %p", this);
}

void lt_session::start_rcv_head_unsafe(lt_data_t *data)
{
    assert_legal();
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::start_rcv_head_unsafe sess %p", this);
    boost::asio::async_read(_socket,
                            boost::asio::buffer(&(data->_length), sizeof(data->_length)),
                            boost::bind(&lt_session::rcv_head_done_unsafe, this,
                                        data, boost::asio::placeholders::error));
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::start_rcv_head_unsafe sess %p", this);
}

void lt_session::rcv_head_done_unsafe(lt_data_t *data, const boost::system::error_code error)
{
    assert_legal();
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::rcv_head_done_unsafe sess %p", this);
    int err = boost_err_translate(error);
    if ( err )
    {
        rcv_queue.clear();
        AWE_MODULE_INFO("communicate", "rcv_done sess %p error network_down", this);
        rcv_done(data, boost::asio::error::network_down);
        return;
    }

    data->realloc_buf();
    start_rcv_data_unsafe(data);
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::rcv_head_done_unsafe sess %p", this);
}

void lt_session::start_rcv_data_unsafe(lt_data_t *data)
{
    assert_legal();
    boost::asio::async_read(_socket,
                            boost::asio::buffer(data->get_buf(), data->_length),
                            boost::bind(&lt_session::rcv_data_done_unsafe, this,
                                        data, boost::asio::placeholders::error));
}

void lt_session::rcv_data_done_unsafe(lt_data_t *data, const boost::system::error_code error)
{
    assert_legal();
    boost::system::error_code err = error ;
    if (!is_connected()) {
        err = boost::asio::error::network_down;
        rcv_queue.clear();
    }

    if(error){
        AWE_MODULE_INFO("communicate", "rcv_done sess %p err [%d]", this, err);
    }
    rcv_done(data, err);
    rcv_queue.continue_to();
}

void lt_session::let_it_up()
{
    assert_legal();
    _connect.set(true);
}

void lt_session::let_it_down()
{
    assert_legal();
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::let_it_down sess %p", this);
    _connect.set(false);
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::let_it_down sess %p", this);
}

void lt_session::connected()
{
    assert_legal();
    reset();
    start_monitor();
    cb->connected(this);
}

void lt_session::disconnected()
{
    assert_legal();
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::disconnected sess %p", this);
    
    cb->disconnected(this);
    queue.clear();
    rcv_queue.clear();
    stop_monitor();
    
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::disconnected sess %p", this);
}

void lt_session::start_snd_data(lt_data_t *data)
{
    assert_legal();
    if (is_connected()) {
        queue.begin_to(boost::bind(&lt_session::start_snd_data_unsafe, this, data),
                       boost::bind(&lt_session::snd_data_done, this, data, boost::asio::error::network_down));
    }
    else {
        snd_data_done(data, boost::asio::error::network_down);
    }
}

void lt_session::snd_data_done(lt_data_t *data, const boost::system::error_code &error)
{
    assert_legal();
    int err = 0;
    if (error || (!is_connected())) {
        let_it_down();
        err = -RPC_ERROR_TYPE_NET_BROKEN;
    } else {
        mark_sent();
    }
    
    cb->snd_done(this, data, err);
}

void lt_session::start_snd_data_unsafe(lt_data_t *data)
{
    assert_legal();
    boost::asio::async_write(_socket,
                             boost::asio::buffer(data->get_data(), data->data_len()),
                             boost::bind(&lt_session::snd_data_done_unsafe, this,
                                         data, boost::asio::placeholders::error));
}

void lt_session::snd_data_done_unsafe(lt_data_t *data, const boost::system::error_code &error)
{
    assert_legal();
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::snd_data_done_unsafe sess %p", this);
    boost::system::error_code err = error;
    if (!is_connected()) {
        queue.clear();
        err = boost::asio::error::network_down;
    }
    
    snd_data_done(data, err);
    queue.continue_to();  //FIXME 不立即调done，有可能引发超时
    
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::snd_data_done_unsafe sess %p", this);
}

void lt_session::state_changed(const bool &is_con)
{
    assert_legal();
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::state_changed sess %p", this);
    if ( !is_con )
        disconnected();
    else
        connected();
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::state_changed sess %p", this);
    
}

bool lt_session::is_connected() const
{
    return _connect.get();
}

void lt_session::set_max_wait_seconds(int sec)
{
    assert_legal();
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
    assert_legal();
    let_it_down();
}

lt_session::~lt_session()
{
    assert_legal();
    AWE_MODULE_INFO("comunicate","~lt_session %p", this);
    std::cout << "session : this : " << __FUNCTION__ << this << std::endl;
    queue.clear();
    rcv_queue.clear();
    _socket.close();
}

void lt_session::from_json_obj(const json_obj &obj)
{

}

json_obj lt_session::to_json_obj() const
{
    return json_obj();
}

void *lt_session_description_imp::get_session_private() const
{
    return description_internal_pri;
}

void lt_session_description_imp::set_session_private(void *_pri)
{
    description_internal_pri = _pri;
}

