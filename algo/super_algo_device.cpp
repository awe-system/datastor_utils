#include "super_algo_device.h"
#include <string>
using namespace std;

#ifdef TEST_SUPER
#include <iostream>
using namespace std;
using namespace ServerSan_Algo;
#include "test_algo_device.h"
#endif

namespace ServerSan_Algo
{
super_algo_device::super_algo_device(const super_algo_device & other):
pool_secs(other.pool_secs),
disk_secs(other.disk_secs),
block_no_disk_info( (block_io*)other.device, disk_secs),
block((block_io *)&block_no_disk_info, pool_secs),
device(other.device)
{
    device->set_event_callback((block_event_callback*)this);
    block.set_io_callback((block_io_callback*)this);
    // cout<<"copy device"<<device<<endl;
    // cout<<"copy block_no_disk_info"<<&block_no_disk_info<<endl;
    // cout<<"copy block"<<&block<<endl;
    // cout<<"copy this"<<this<<endl;

}


super_algo_device::super_algo_device(algo_device *_device,
            unsigned long disk_info_secs,
            unsigned long pool_info_secs):
pool_secs(pool_info_secs),
disk_secs(disk_info_secs),
block_no_disk_info( (block_io*)_device, disk_info_secs),
block((block_io *)&block_no_disk_info, pool_info_secs),
device(_device)
{
    device->set_event_callback((block_event_callback*)this);
    block.set_io_callback((block_io_callback*)this);
}

unsigned long long super_algo_device::get_sector_num()const
{
    return block.get_sector_num();
}

int super_algo_device::open(void)
{
    return block.open();
}

void super_algo_device::close(void)
{
    block.close();
}

void super_algo_device::do_request(request_t *request)
{
    block.do_request(request);
}

void super_algo_device::request_done(request_t *request, int err)
{
    // cout<<"super_algo_device::request_done"<<endl;
    complete_request(request,err);
    // cout<<"after super_algo_device::request_done"<<endl;
}

string super_algo_device::get_host_name(void) const
{
    return device->get_host_name();
}

bool super_algo_device::is_local(void) const
{
    return device->is_local();
}

string super_algo_device::get_device_id(void) const
{
     return device->get_device_id();
}

int super_algo_device::handle_offline(block_event *block)
{
    offline();
}

int super_algo_device::handle_online(block_event *block)
{
    online();
}

int super_algo_device::write_pool_info(const json_obj & info)
{
    try
    {
        block.write_super(info);
    }
    catch (...)
    {
        return -ERROR_TYPE_WRITE_SUPER;
    }

}

json_obj super_algo_device::read_pool_info()
{
    json_obj obj("type",POOL_TYPE_UNKNOWN);
    try
    {
        block.read_super(obj);
        return obj;
    }
    catch(...)
    {
        return obj;
    }
}

};


#ifdef TEST_SUPER

class io_callback_service:public block_io_callback
{
	void request_done(request_t *request, int err)
	{
		cout<<"request_done err "<<error_str(err)<<endl;
	}
};

int main(int argc, const char *argv[])
{
	assert(argc>=4);

	io_callback_service callback_service;
	string path1 = argv[1];
	string path2 = argv[2];
	unsigned long sec = atoi(argv[3]);
	assert(sec > disk_info_sec.get_int()+pool_info_sec.get_int());

	string ser = "ser";
	test_algo_device dev_1(ser,path1,"host1",sec,true);
    // ser = string("ser");
	test_algo_device dev_2(ser,path2,"host2",sec,true);


	algo_device_list_t device_list;
	device_list.push_back(&dev_1);
	device_list.push_back(&dev_2);
	super_algo_device super_1(&dev_1);
	super_algo_device super_2(&dev_2);
	cout<<"1 get_sector_num:"<< super_1.get_sector_num()<<endl;
	cout<<"1 get_host_name:"<< super_1.get_host_name()<<endl;
	cout<<"1 is_local:"<< super_1.is_local()<<endl;
	cout<<"1 get_serial_num:"<< super_1.get_serial_num()<<endl;

    cout<<"2 get_sector_num:"<< super_2.get_sector_num()<<endl;
    cout<<"2 get_host_name:"<< super_2.get_host_name()<<endl;
    cout<<"2 is_local:"<< super_2.is_local()<<endl;
    cout<<"2 get_serial_num:"<< super_2.get_serial_num()<<endl;

	super_1.set_io_callback(&callback_service);
	super_2.set_io_callback(&callback_service);
cout<<"after super_1 set_io_callback"<<endl;
    super_algo_device super1 = super_1;
    super_algo_device super2 = super_2;
cout<<"before super1 open"<<endl;
	assert( 0 == super1.open());
	assert( 0 == super2.open());

cout<<"begin"<<endl;
// 	unsigned char *buf = new unsigned char[512];
// 	request_t *request = new request_t(512,buf);
// 	for(unsigned long i=0;i<super1.get_sector_num();i++)
// 	{//这个测试时同步的情况
// 		request->offset = i;
// 		request->type = REQUEST_SYNC_READ;
// 		super1.do_request(request);
// 		request->type = REQUEST_SYNC_WRITE;
// 		super2.do_request(request);
// 	}
// 	delete buf;
// 	delete request;

	if(argc > 4)
	{
        cout<<"after do request"<<endl;
		if(strcmp(argv[4],"read") == 0)
		{
            cout<<"before read_pool_info "<<endl;
			json_obj obj = super1.read_pool_info();
			cout<<"after read_pool_info "<<endl;
            cout<<obj.dumps()<<endl;
            cout<<"super1 pinfo->dumps():"<<endl;
            obj = super2.read_pool_info();
            cout<<"super2 pinfo->dumps():"<<endl;
            cout<<obj.dumps()<<endl;
		}
		else
		{
			algo_copy_info algo_info(device_list,2);
            cout<<"before obj"<<endl;
			json_obj obj("type",(int)algo_info.get_type());
            json_obj arr;
            arr.set_array();
            for(algo_device_it_t it = algo_info.algo_device_list.begin();
                it != algo_info.algo_device_list.end();
                ++it)
            {
                string seri = (*it)->get_serial_num();
                arr.append(seri);
            }

            json_obj ser("ser",arr);
            obj.merge(ser);
            cout<<"before read_pool_info "<<endl;
			int err = super1.write_pool_info(obj);
			cout<<"err:"<<err<<endl;
            err = super2.write_pool_info(obj);
            cout<<"err:"<<err<<endl;
		}
	}
	super1.close();
	super2.close();
    getchar();
	return 0;

}

#endif