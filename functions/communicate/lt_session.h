#ifndef LT_SESSION_H
#define LT_SESSION_H

#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <mutex>
#include <condition_variable>
#include <list>
#include "lt_async_2safe_async.h"
#include "lt_reference.h"
#include "lt_safe_state.hpp"
#include "lt_session_dog.h"
#include "lt_data/lt_data_t.h"

#define DEFAULT_WAIT_SECONDS 600

class lt_session_description
{
public:
    virtual bool is_connected() const = 0;
    
    virtual void set_session_private(void *pri) = 0;

    virtual void *get_session_private() const = 0;
};

class lt_session_description_imp : public lt_session_description
{
private:
    void *description_internal_pri = nullptr;
public:
    void set_session_private(void *pri);
    
    void *get_session_private() const;
    
    virtual bool is_connected() const = 0;
};

class lt_session;

class lt_session_callback
{
public:
    virtual void rcv_done(lt_session *sess, lt_data_t *received_data, int error) = 0;

    virtual void snd_done(lt_session *sess, lt_data_t *sent_data, int error) = 0;

    virtual void disconnected(lt_session *sess) = 0;

    virtual void connected(lt_session *sess) = 0;
};

class lt_session : public lt_reference, public lt_session_dog,public lt_session_description_imp
{
private:
    int max_wait_seconds;
private:
    lt_session_callback *cb;
    boost::asio::io_service *io_service;
public:
    boost::asio::ip::tcp::socket _socket;
private:
    lt_safe_state<bool> _connect;
private:
    lt_async_2safe_async queue;
    lt_async_2safe_async rcv_queue;
private:
    void start_rcv_head_unsafe(lt_data_t *data);

    void start_rcv(lt_data_t *data);

    void rcv_done(lt_data_t *data, const boost::system::error_code &error);

    void rcv_head_done_unsafe(lt_data_t *data, const boost::system::error_code &error);

    void start_rcv_data_unsafe(lt_data_t *data);

    void rcv_data_done_unsafe(lt_data_t *data, const boost::system::error_code &error);

private:
    void start_snd_data_unsafe(lt_data_t *data);

    void snd_data_done_unsafe(lt_data_t *data, const boost::system::error_code &error);

    void start_snd_data(lt_data_t *data);

    void snd_data_done(lt_data_t *data, const boost::system::error_code &error);

private:
    void state_changed(const bool &is_con);

public:
    bool is_connected() const override;
protected:
    virtual void disconnected();

    virtual void connected();

public:
    void let_it_up();

    void let_it_down();

public:
    lt_session(boost::asio::io_service *_io_service, lt_session_callback *_cb);

    virtual void rcv(lt_data_t *data);

    virtual void snd(lt_data_t *data);

    void set_max_wait_seconds(int sec);

    bool is_to_feed() const override;

    void handle_event() override;
    
    virtual ~lt_session();

};

#endif
