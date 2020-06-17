#ifndef _COMMUNICATE_LT_SESSION_CLI_SAFE_H
#define _COMMUNICATE_LT_SESSION_CLI_SAFE_H


#include "lt_session_cli.h"


#define CLI_DISCONNECT_WAIT_POOL_NUM 5
#define MAX_DICONNECT_WAIT_NUM      10000

class lt_session_cli_set;

class lt_session_cli_safe : public lt_session_cli, public lt_session_callback
{
    long wait_disconn_num = 0;
private:
    int connect_cnt =0;
    int disconnect_cnt =0;
    bool is_down_connected;
    long                pending_cnt;
    lt_session_callback *cb;
    lt_session_cli_set  *set;

private:
    void disconnected_inthread();
    
public:
    void connected(lt_session *sess) override;

public:
    void
    rcv_done(lt_session *sess, lt_data_t *received_data, int error) override;
    
    void snd_done(lt_session *sess, lt_data_t *sent_data, int error) override;
    
    void disconnected(lt_session *sess) override;

public:
    virtual ~lt_session_cli_safe();
    lt_session_cli_safe(lt_session_cli_set *_set,
                        boost::asio::io_service *_io_service,
                        lt_session_callback *_cb);
    
    //FIXNE connect 只会被调用一次
    void connect(const std::string &ip, unsigned short port) override;
    
    //NOTE： disconnect 只会被调用一次
    void disconnect() override;
    
    void rcv(lt_data_t *rcv_data) override;
    
    void snd(lt_data_t *data) override;
};


#endif
