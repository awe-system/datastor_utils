#include "lt_session.h"
#include "../lt_function_error.h"
#include "../log/include/awe_log.h"

lt_session::lt_session(boost::asio::io_service *_io_service,
                       lt_session_callback *_cb) :
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
    lt_data_t *sentdata = static_cast<lt_data_t *>(data->pop_private());
    data->push_private(sentdata);
    AWE_MODULE_DEBUG("communicate",
                     "enter lt_session::rcv sess %p data %p sentdata %p",
                     this, data, sentdata);
    if ( is_connected() )
    {
        AWE_MODULE_DEBUG("communicate",
                         "is_connected lt_session::rcv sess %p data %p sentdata %p",
                         this, data, sentdata);
        start_rcv(data);
        AWE_MODULE_DEBUG("communicate",
                         "after start_rcv lt_session::rcv sess %p data %p sentdata %p",
                         this, data, sentdata);
    }
    else
    {
        cb->rcv_done(this, data, -RPC_ERROR_TYPE_CONNECT_FAIL);
        let_it_down();
    }
    AWE_MODULE_DEBUG("communicate", "leave lt_session::rcv sess %p data %p",
                     this, data);
}

void lt_session::snd(lt_data_t *data)
{
    AWE_MODULE_DEBUG("communicate", "enter lt_session::snd sess %p data %p",
                     this, data);
    if ( is_connected() )
    {
        AWE_MODULE_DEBUG("communicate", "before start_snd_data sess %p data %p",
                         this, data);
        start_snd_data(data);
        AWE_MODULE_DEBUG("communicate", "after start_snd_data sess %p data %p",
                         this, data);
    }
    else
    {
        AWE_MODULE_DEBUG("communicate", "before snd_done failed sess %p", this);
        cb->snd_done(this, data, -RPC_ERROR_TYPE_CONNECT_FAIL);
        AWE_MODULE_DEBUG("communicate", "after snd_done failed sess %p", this);
    }
    AWE_MODULE_DEBUG("communicate", "leave lt_session::snd sess %p", this);
}


void lt_session::start_rcv(lt_data_t *data)
{
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::start_rcv sess %p",
                     this);
    rcv_queue.begin_to(
            boost::bind(&lt_session::start_rcv_head_unsafe, this, data),
            boost::bind(&lt_session::rcv_done, this, data,
                        boost::asio::error::network_down));
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::start_rcv sess %p",
                     this);
}

void
lt_session::rcv_done(lt_data_t *data, const boost::system::error_code &error)
{
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::rcv_done sess %p",
                     this);
    int err = boost_err_translate(error);
    if ( err )
    {
        AWE_MODULE_DEBUG("communicate",
                         "lt_session::rcv_done sess %p err %d data %p",
                         this, err, data);
        cb->rcv_done(this, data, -RPC_ERROR_TYPE_CONNECT_FAIL);
        AWE_MODULE_DEBUG("communicate",
                         "lt_session::rcv_done sess %p err %d data %p",
                         this, err, data);
        let_it_down();
    }
    else
    {
        AWE_MODULE_DEBUG("communicate",
                         "Success lt_session::rcv_done sess %p data %p",
                         this, data);
        cb->rcv_done(this, data, RPC_ERROR_TYPE_OK);
        AWE_MODULE_DEBUG("communicate",
                         "Success lt_session::rcv_done sess %p  data %p",
                         this, data);
        mark_received();
    }
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::rcv_done sess %p",
                     this);
}

void lt_session::start_rcv_head_unsafe(lt_data_t *data)
{
    AWE_MODULE_DEBUG("communicate",
                     "--enter lt_session::start_rcv_head_unsafe sess %p data %p",
                     this, data);
    boost::asio::async_read(_socket,
                            boost::asio::buffer(&(data->_length),
                                                sizeof(data->_length)),
                            boost::bind(&lt_session::rcv_head_done_unsafe, this,
                                        data,
                                        boost::asio::placeholders::error));
    AWE_MODULE_DEBUG("communicate",
                     "--leave lt_session::start_rcv_head_unsafe sess %p data %p",
                     this, data);
}

void lt_session::rcv_head_done_unsafe(lt_data_t *data,
                                      const boost::system::error_code &error)
{
    AWE_MODULE_DEBUG("communicate",
                     "--enter lt_session::rcv_head_done_unsafe sess %p data %p",
                     this, data);
    int err = boost_err_translate(error);
    if ( err )
    {
        AWE_MODULE_DEBUG("communicate",
                         "--Err leave lt_session::start_rcv_head_unsafe sess %p ERROR %d data %p",
                         this, err, data);
        rcv_done(data, boost::asio::error::network_down);
        AWE_MODULE_DEBUG("communicate",
                         "--Err leave lt_session::start_rcv_head_unsafe sess %p ERROR %d data %p",
                         this, err, data);
        return;
    }
    
    data->realloc_buf();
    AWE_MODULE_DEBUG("communicate",
                     "after realloc_buf in lt_session::rcv_head_done_unsafe sess %p data %p",
                     this, data);
    start_rcv_data_unsafe(data);
    AWE_MODULE_DEBUG("communicate",
                     "--leave lt_session::rcv_head_done_unsafe sess %p data %p",
                     this, data);
    
}

void lt_session::start_rcv_data_unsafe(lt_data_t *data)
{
    boost::asio::async_read(_socket,
                            boost::asio::buffer(data->get_buf(), data->_length),
                            boost::bind(&lt_session::rcv_data_done_unsafe, this,
                                        data,
                                        boost::asio::placeholders::error));
}

void lt_session::rcv_data_done_unsafe(lt_data_t *data,
                                      const boost::system::error_code &error)
{
    AWE_MODULE_DEBUG("communicate",
                     "--enter lt_session::rcv_data_done_unsafe sess %p data %p",
                     this, data);
    if ( error )
    {
        AWE_MODULE_DEBUG("communicate",
                         "--Err leave lt_session::rcv_data_done_unsafe sess %p ERROR data %p",
                         this, data);
        rcv_done(data, error);
        AWE_MODULE_DEBUG("communicate",
                         "--Err leave lt_session::rcv_data_done_unsafe sess %p ERROR data %p",
                         this, data);
        let_it_down();
        return;
    }
    
    AWE_MODULE_DEBUG("communicate",
                     "lt_session::rcv_data_done_unsafe before rcv_done  sess %p data %p",
                     this, data);
    
    rcv_done(data, error);
    
    AWE_MODULE_DEBUG("communicate",
                     "lt_session::rcv_data_done_unsafe before continue_to  sess %p data %p",
                     this, data);
    rcv_queue.continue_to();
    AWE_MODULE_DEBUG("communicate",
                     "lt_session::rcv_data_done_unsafe before continue_to  sess %p data %p",
                     this, data);
    AWE_MODULE_DEBUG("communicate",
                     "--Err leave lt_session::rcv_data_done_unsafe success sess %p data %p",
                     this, data);
}

void lt_session::let_it_up()
{
    _connect.set(true);
}

void lt_session::let_it_down()
{
    AWE_MODULE_DEBUG("communicate", "-->enter lt_session::let_it_down sess %p",
                     this);
    _connect.set(false);
    AWE_MODULE_DEBUG("communicate", "<--leave lt_session::let_it_down sess %p",
                     this);
}

void lt_session::connected()
{
    reset();
    start_monitor();
    cb->connected(this);
}

void lt_session::disconnected()
{
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::disconnected sess %p",
                     this);
    //FIXME:加入flag控制 使得在断开后不会出现新的 rcv
    _socket.close();
    AWE_MODULE_DEBUG("communicate", "after _socket.close(); sess %p", this);
    cb->disconnected(this);
    AWE_MODULE_DEBUG("communicate", "after cb->disconnected(this); sess %p",
                     this);
    queue.clear();
    AWE_MODULE_DEBUG("communicate", "after queue.clear(); sess %p", this);
    stop_monitor();
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::disconnected sess %p",
                     this);
}

void lt_session::start_snd_data(lt_data_t *data)
{
    AWE_MODULE_DEBUG("communicate",
                     "--enter before beginto lt_session::start_snd_data sess %p data %p",
                     this, data);
    queue.begin_to(boost::bind(&lt_session::start_snd_data_unsafe, this, data),
                   boost::bind(&lt_session::snd_data_done, this, data,
                               boost::asio::error::network_down));
    AWE_MODULE_DEBUG("communicate",
                     "--level after begin to lt_session::start_snd_data sess %p data %p",
                     this, data);
}

void lt_session::snd_data_done(lt_data_t *data,
                               const boost::system::error_code &error)
{
    AWE_MODULE_DEBUG("communicate",
                     "--enter lt_session::snd_data_done %p data %p",
                     this, data);
    int err = boost_err_translate(error);
    AWE_MODULE_DEBUG("communicate",
                     "lt_session::snd_data_done %p data %p err %d",
                     this, data, err);
    if ( err )
    {
        
        cb->snd_done(this, data, err);
        AWE_MODULE_DEBUG("communicate",
                         "lt_session::snd_data_donef ater snd_done  %p data %p err %d",
                         this, data, err);
        let_it_down();
    }
    else
    {
        cb->snd_done(this, data, RPC_ERROR_TYPE_OK);
        AWE_MODULE_DEBUG("communicate",
                         "lt_session::snd_data_donef ater snd_done  %p data %p err %d",
                         this, data, err);
        mark_sent();
    }
    AWE_MODULE_DEBUG("communicate",
                     "--level lt_session::snd_data_done %p data %p",
                     this, data);
}


void lt_session::start_snd_data_unsafe(lt_data_t *data)
{
    boost::asio::async_write(_socket,
                             boost::asio::buffer(data->get_data(),
                                                 data->data_len()),
                             boost::bind(&lt_session::snd_data_done_unsafe,
                                         this,
                                         data,
                                         boost::asio::placeholders::error));
}

void lt_session::snd_data_done_unsafe(lt_data_t *data,
                                      const boost::system::error_code &error)
{
    AWE_MODULE_DEBUG("communicate",
                     "--enter lt_session::snd_data_done_unsafe sess %p data %p",
                     this, data);
    int err = boost_err_translate(error);
    AWE_MODULE_DEBUG("communicate",
                     "--enter lt_session::snd_data_done_unsafe sess %p data %p err %d",
                     this, data, err);
    if ( err )
    {
    
        AWE_MODULE_DEBUG("communicate",
                         "before snd_data_done sess %p data %p err %d",
                         this, data, err);
        snd_data_done(data, error);
        AWE_MODULE_DEBUG("communicate",
                         "after snd_data_done sess %p data %p err %d",
                         this, data,err);
        let_it_down();
    }
    else
    {
        AWE_MODULE_DEBUG("communicate",
                         "before snd_data_done sess %p data %p",
                         this, data);
        snd_data_done(data, error);
        AWE_MODULE_DEBUG("communicate",
                         "after snd_data_done sess %p data %p",
                         this, data);
        queue.continue_to();
    }
    
    AWE_MODULE_DEBUG("communicate",
                     "--leave lt_session::snd_data_done_unsafe sess %p data %p",
                     this, data);
    
}

void lt_session::state_changed(const bool &is_con)
{
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::state_changed sess %p",
                     this);
    if ( !is_con )
    {
        disconnected();
    }
    else
    {
        connected();
    }
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::state_changed sess %p",
                     this);
    
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

lt_session::~lt_session()
{
    _socket.close();
}

void *lt_session_description_imp::get_session_private() const
{
    return description_internal_pri;
}

void lt_session_description_imp::set_session_private(void *_pri)
{
    description_internal_pri = _pri;
}

lt_session_description_imp::~lt_session_description_imp()
{
    AWE_MODULE_DEBUG("communicate",
                     "lt_session_description_imp::~lt_session_description_imp %p",
                     this);
}

lt_session_description_imp::lt_session_description_imp()
{
    AWE_MODULE_DEBUG("communicate",
                     "lt_session_description_imp::lt_session_description_imp %p",
                     this);
}

