#ifndef TEST_COMMUNICATE_LT_CLIENT_SERVICE_H
#define TEST_COMMUNICATE_LT_CLIENT_SERVICE_H

#include "lt_session_cli_safe.h"
#include "lt_thread_server.h"

class lt_client_service : public lt_session_callback
{
    boost::asio::io_service *io_service;
public:
    lt_client_service(boost::asio::io_service *_io_service,
                      unsigned short port);
    
    void set_ioservice(boost::asio::io_service *_io_service);
    
    int
    snd(lt_session_cli_safe *sess, boost::function<int(lt_data_t *)> gendata_f);

private:
    void
    rcv_done(lt_session *sess, lt_data_t *received_data, int error) override;
    
    void snd_done(lt_session *sess, lt_data_t *sent_data, int error) override;
    
    void disconnected(lt_session *sess) override;
    
    void connected(lt_session *sess) override;

private:
    virtual void handler_by_output(lt_data_t *received_data) = 0;
    
    virtual void handler_by_input(lt_data_t *sent_data, int error) = 0;
    
};


#endif
