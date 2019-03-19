//
// Created by root on 19-3-18.
//
#include <communicate/lt_session.h>
#include "conf/testclient.h"
#include "conf/testserver.h"
#include "../../algo/debug.h"


class testcli : public testclient::testclient
{
public:
    bool is_closing = false;
    void disconnected() override
    {
        cout<<"testcli::disconnected"<<endl;
        is_closing = true;
        cout<<"testcli::is_closing"<<endl;
        this->disconnect();
        cout<<"testclient::testclient done"<<endl;
        is_closing = false;
        cout<<"testcli::not_closing"<<endl;
    }
    
    int connect(const std::string &ip) override
    {
        if(is_closing) return -1;
    
        cout<<"when not closing connect"<<endl;
        return testclient::connect(ip);
    }
};





int main(int argc, const char *argv[])
{
    {
        testcli cli;
        int err = cli.connect("127.0.0.1");
        for(int i =0;i<2;++i)
        {
            if ( err )
            {
                sleep(1);
                err = cli.connect("127.0.0.1");
                if(err) continue;
            }
            unsigned int out;
            err = cli.test(i, out);
            if(err)
                cout<<"test err wait 1 second"<<endl;
            else
                cout<<"output"<<out<<endl;
        }
        cli.disconnect();
    
    }
    cout << "+++++++++++++++++++++++++" << endl;
    {
        testcli cli;
        int err = cli.connect("127.0.0.1");
        for(int i =0;i<20;++i)
        {
            if ( err )
            {
                sleep(1);
                err = cli.connect("127.0.0.1");
                if(err) continue;
            }
            unsigned int out;
            err = cli.test(i, out);
            if(err)
                cout<<"test err wait 1 second"<<endl;
            else
                cout<<"output"<<out<<endl;
        }
    }
    while(1)
    {
        sleep(1);
    }
}
