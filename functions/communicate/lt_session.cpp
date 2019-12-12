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
    std::cout << "session : this : " << __FUNCTION__ << this << std::endl;
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
    AWE_MODULE_DEBUG("communicate", "enter lt_session::snd sess %p", this);
    if ( is_connected())
    {
        AWE_MODULE_DEBUG("communicate", "before start_snd_data sess %p", this);
        start_snd_data(data);
        AWE_MODULE_DEBUG("communicate", "after start_snd_data sess %p", this);
    }
    else
    {
        AWE_MODULE_DEBUG("communicate", "before snd_done failed sess %p", this);
        cb->snd_done(this, data, -RPC_ERROR_TYPE_CONNECT_FAIL);
        AWE_MODULE_DEBUG("communicate", "after snd_done failed sess %p", this);
    }
    AWE_MODULE_DEBUG("communicate", "leave lt_session::snd sess %p", this);
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
    /*
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::start_rcv sess %p", this);
    if (is_connected()) {
        std::cout << __LINE__ << "=====================" << std::this_thread::get_id() << std::endl;
        */
    //get();
        rcv_queue.begin_to(boost::bind(&lt_session::start_rcv_head_unsafe, this, data),
                           boost::bind(&lt_session::rcv_done, this, data, boost::asio::error::network_down));
        /*
    }
    else {
        rcv_done(data, boost::asio::error::network_down);
    }
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::start_rcv sess %p", this);
         */
    
}

void lt_session::rcv_done(lt_data_t *data, const boost::system::error_code error)
{
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::rcv_done sess %p", this);
    //check_disconnect_when_done(error);
    unsigned err = 0;
    if (error || (!is_connected())) {
        err = -RPC_ERROR_TYPE_NET_BROKEN;
        let_it_down();
    } else {
        mark_received();
    }
    
    cb->rcv_done(this, data, err);
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::rcv_done sess %p", this);
}

void lt_session::start_rcv_head_unsafe(lt_data_t *data)
{
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::start_rcv_head_unsafe sess %p", this);
    boost::asio::async_read(_socket,
                            boost::asio::buffer(&(data->_length), sizeof(data->_length)),
                            boost::bind(&lt_session::rcv_head_done_unsafe, this,
                                        data, boost::asio::placeholders::error));
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::start_rcv_head_unsafe sess %p", this);
}

void lt_session::rcv_head_done_unsafe(lt_data_t *data, const boost::system::error_code error)
{
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::rcv_head_done_unsafe sess %p", this);
    //std::cout << "****" << __LINE__ << "  " <<  __FILE__ << " " << this << "--------------------------------------error : -" << error <<"  data : " << data->to_string() << std::endl;
    int err = boost_err_translate(error);
    if ( err )
    {
        //rcv_queue.continue_to();
        rcv_queue.clear();
        rcv_done(data, boost::asio::error::network_down);
        AWE_MODULE_DEBUG("communicate", "--Err leave lt_session::start_rcv_head_unsafe sess %p", this);
        return;
    }

    data->realloc_buf();
    //std::cout << "data : " << data->to_string() << std::endl;
    start_rcv_data_unsafe(data);
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::rcv_head_done_unsafe sess %p", this);
}

void lt_session::start_rcv_data_unsafe(lt_data_t *data)
{
   
   // std::cout << "data : " << data->to_string() << std::endl;
    
    boost::asio::async_read(_socket,
                            boost::asio::buffer(data->get_buf(), data->_length),
                            boost::bind(&lt_session::rcv_data_done_unsafe, this,
                                        data, boost::asio::placeholders::error));
}

void lt_session::rcv_data_done_unsafe(lt_data_t *data, const boost::system::error_code error)
{
    //std::cout << "****" << __LINE__ << "  " <<  __FILE__ << " " << this << "--------------------------------------error : -" << error <<"  data : " << data->to_string() << std::endl;
    boost::system::error_code err = error ;
    if (!is_connected()) {
        err = boost::asio::error::network_down;
        rcv_queue.clear();
    }
    rcv_queue.continue_to();
    /*
    if( !err) {
        std::cout << __LINE__ << "=====================" <<std::this_thread::get_id()<< std::endl;
        rcv_queue.continue_to();
    }
     */
    rcv_done(data, err);
}

void lt_session::let_it_up()
{
    _connect.set(true);
}

void lt_session::let_it_down()
{
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::let_it_down sess %p", this);
    _connect.set(false);
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::let_it_down sess %p", this);
}

void lt_session::connected()
{
    reset();
    start_monitor();
    cb->connected(this);
}

void lt_session::disconnected()
{
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::disconnected sess %p", this);
    //FIXME:加入flag控制 使得在断开后不会出现新的 rcv
    boost::system::error_code ec;
    
    // shutdown function will close the socket graceful and cause of program crash.
    //_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
  
    //_socket.close(ec);
    if (ec) {
    
    }
    AWE_MODULE_DEBUG("communicate", "after _socket.close(); sess %p", this);
    cb->disconnected(this);
    queue.clear();
    rcv_queue.clear();
    
    AWE_MODULE_DEBUG("communicate", "after cb->disconnected(this); sess %p", this);
    AWE_MODULE_DEBUG("communicate", "after queue.clear(); sess %p", this);
    stop_monitor();
    
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::disconnected sess %p", this);
}

void lt_session::start_snd_data(lt_data_t *data)
{
    //if (is_connected()) {
     //   std::cout << __LINE__ << "=====================" << std::this_thread::get_id() << std::endl;
        queue.begin_to(boost::bind(&lt_session::start_snd_data_unsafe, this, data),
                       boost::bind(&lt_session::snd_data_done, this, data, boost::asio::error::network_down));
        /*
    }
    else {
        snd_data_done(data, boost::asio::error::network_down);
    }
         */
}

void lt_session::snd_data_done(lt_data_t *data, const boost::system::error_code &error)
{
    int err = 0;
    if (error || (!is_connected())) {
        let_it_down();
        err = -RPC_ERROR_TYPE_NET_BROKEN;
    } else {
        mark_sent();
    }
    
    //check_disconnect_when_done(error);
    //std::string tmp = data->to_string();
    cb->snd_done(this, data, err);
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
    AWE_MODULE_DEBUG("communicate", "--enter lt_session::snd_data_done_unsafe sess %p", this);
    //std::cout << "****" << __LINE__ << "  " <<  __FILE__ << " " << this << "--------------------------------------error : -" << error <<"  data : " << data->to_string() << std::endl;
    boost::system::error_code err = error;
    if (!is_connected()) {
        queue.clear();
        err = boost::asio::error::network_down;
    }
    
    queue.continue_to();  //FIXME 不立即调done，有可能引发超时
    /*
    if ( !err ) {
        std::cout << __LINE__ << " send done =====================" << std::this_thread::get_id() << std::endl;
        queue.continue_to();  //FIXME 不立即调done，有可能引发超时
    }
     */
    
    snd_data_done(data, err);
    AWE_MODULE_DEBUG("communicate", "--leave lt_session::snd_data_done_unsafe sess %p", this);
}

void lt_session::state_changed(const bool &is_con)
{
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
    std::cout << "session : this : " << __FUNCTION__ << this << std::endl;
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

