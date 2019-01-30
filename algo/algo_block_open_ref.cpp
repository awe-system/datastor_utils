#include "algo.h"
using namespace ServerSan_Algo;
#include <iostream>
using namespace std;

algo_block_open_ref::algo_block_open_ref(const algo_block_open_ref & other):
block_io(other)
{
	org_block = other.org_block;
	ref = other.ref;
	org_block->set_io_callback((block_io_callback*)this);
}

algo_block_open_ref::algo_block_open_ref(block_io *_block):org_block(_block),ref(0)
{
	org_block->set_io_callback((block_io_callback*)this);
}

unsigned long long algo_block_open_ref::get_sector_num()const
{
	return org_block->get_sector_num();
}

int algo_block_open_ref::open(void)
{
	int err = ERROR_TYPE_OK;
	std::unique_lock <std::mutex> lck(open_lock);
	if(0 != ref)
	{
		ref++;
		return err;
	} 
	err = org_block->open();
	if(err) return err;
	ref++;
	return err;
}

void algo_block_open_ref::close(void)
{
	int err = ERROR_TYPE_OK;
	std::unique_lock <std::mutex> lck(open_lock);
	ref--;
	if(0 == ref) org_block->close();
}

void algo_block_open_ref::do_request(request_t *request)
{
	org_block->do_request(request);
}

void algo_block_open_ref::request_done(request_t *request, int err)
{
	complete_request(request,err);
}

#ifdef TEST_ALGO_OPEN_REF
#include <iostream>
using namespace std;
#include "test_algo_device.h"
class io_callback_service:public block_io_callback
{
	void request_done(request_t *request, int err)
	{
		cout<<"request_done err "<<error_str(err)<<endl;
	}
};

int main(int argc, char const *argv[])
{
	assert(argc>=4);
	unsigned int super_len = 512;
	unsigned char super_buf[512];
	
	io_callback_service callback_service;
	string path1 = argv[1];
	string path2 = argv[2];
	unsigned long sec = atoi(argv[3]);
	assert(sec > 1);
	string ser = "ser";
	test_algo_device dev_1(ser,path1,sec);
	test_algo_device dev_2(ser,path2,sec);
	algo_block_open_ref device1(&dev_1);
	algo_block_open_ref device2(&dev_2);
	device1.set_io_callback(&callback_service);
	device2.set_io_callback(&callback_service);
	assert( 0 == device1.open());
	assert( 0 == device2.open());

cout<<"begin"<<endl;
	unsigned char *buf = new unsigned char[512];
	request_t *request = new request_t(512,buf);
cout<<"device1.get_sector_num():"<<device1.get_sector_num()<<endl;
	for(unsigned long i=0;i<device1.get_sector_num();i++)
	{//这个测试时同步的情况
		request->offset = i;
		request->type = REQUEST_SYNC_READ;
		device1.do_request(request);
		request->type = REQUEST_SYNC_WRITE;
		device2.do_request(request);
	}
	delete buf;
	delete request;
	device1.close();
	device2.close();
	return 0;
}
#endif