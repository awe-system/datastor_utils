#ifndef _COMMUNICATE_LT_SERVER_SERVICE_H
#define _COMMUNICATE_LT_SERVER_SERVICE_H


#include "lt_session.h"
#include "lt_session_serv.h"
#include "lt_thread_server.h"


class lt_server_callback
{
public:
    virtual void do_func(lt_data_t *data, lt_session_serv *sess) = 0;
    virtual void disconnected(lt_session_description *sess) = 0;
};

class lt_server_service : public lt_session_callback
{
private:
    lt_thread_server server;
    lt_server_callback *cb;
    unsigned short port;
    boost::asio::ip::tcp::endpoint end;
    boost::asio::ip::tcp::acceptor _acceptor;
private:
    void put_session(lt_session_serv *sess);

    lt_session_serv *get_session();

    lt_session_serv *get_session(lt_session_serv *sess);

public:
    lt_server_service(int thread_num, unsigned short _port, lt_server_callback *_cb);

    void start_accept();

    int snd(lt_session_serv *sess, boost::function<int(lt_data_t *)> gendata_f);

private:
    void accepted(lt_session_serv *sess, const boost::system::error_code &error);

private:
    void rcv_done(lt_session *sess, lt_data_t *received_data, int error) override;

    void snd_done(lt_session *sess, lt_data_t *sent_data, int error) override;

    void disconnected(lt_session *sess) override;

    void connected(lt_session *sess) override;
};

#endif //TEST_COMMUNICATE_LT_SERVER_SERVICE_H
