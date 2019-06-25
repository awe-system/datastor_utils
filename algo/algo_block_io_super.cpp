#include <iostream>
#include "debug.h"
#include <unistd.h>
#include "algo.h"

using namespace ServerSan_Algo;
// #include <iostream>


class supper_context
{
	unsigned char *buf;
	unsigned int len;
private:
	unsigned int ceil_shift(unsigned int org, unsigned int shift);

	unsigned int upper_sec_length(unsigned int org);

public:
	supper_context(const char *data);
	supper_context(const char *data,unsigned int len);
	~supper_context();
	unsigned int super_len();
	unsigned char *super_buf();
};

unsigned int supper_context::ceil_shift(unsigned int org, unsigned int shift)
{
	assert(org != 0);
	return ((org-1)>>shift) + 1;
}

unsigned int supper_context::upper_sec_length(unsigned int org)
{
	return ceil_shift(org,9)<<9;
}

supper_context::supper_context(const char *data)
{
	len = upper_sec_length((unsigned int)strlen(data)+1);
	buf = new unsigned char [len];
	memset(buf,0,len);
	memcpy(buf,data,strlen(data));
}


supper_context::supper_context(const char *data, unsigned int _len)
{
	len = upper_sec_length(_len);
	buf = new unsigned char [len];
	memset(buf,0,len);
	memcpy(buf,data,strlen(data));
}

supper_context::~supper_context()
{
	delete buf;
}

unsigned char *supper_context::super_buf()
{
	return buf;
}

unsigned int supper_context::super_len()
{
	return len;
}

int algo_block_io_super::sync_request(request_t *request)
{
    std::unique_lock<mutex> lck(sync_m);
	int err = ERROR_TYPE_OK;
	request->private_data = (void*)&err;
	org_block->do_request(request);
	super_cond.wait();
	return err;
}

void algo_block_io_super::sync_request_done(request_t *request, int err)
{
	int *perr = (int *)request->private_data;
	*perr = err;

//dbg<<"sync_request_done before notify "<<this<<DEBUG_END;
//dbg<<perr<<" "<<err<<DEBUG_END;
	super_cond.notify();
//dbg<<"sync_request_done after notify "<<this<<DEBUG_END;
}

algo_block_io_super::algo_block_io_super(const algo_block_io_super & other):algo_block_open_ref(other)
{
	super_sec = other.super_sec;
	org_block->set_io_callback((block_io_callback *) this);
}

//这个地方调用了两次， 第一次是对的。
algo_block_io_super::algo_block_io_super(block_io *block_p, unsigned long _size) : algo_block_open_ref(block_p),super_sec(_size)
{
    org_block->set_io_callback((block_io_callback *) this);
}

unsigned long long algo_block_io_super::get_sector_num()const
{
//    cout<<"get_sector_num() : "<<org_block->get_sector_num()<<endl;
//    cout<<"super_sec: "<<super_sec<<endl;
	assert(org_block->get_sector_num() > super_sec);
    return org_block->get_sector_num() - super_sec;  //org_block 就是 algo_device
}

void algo_block_io_super::do_request(request_t *request)
{
//dbg<<color_red<<"do request this "<<(long )this<<request->to_json_obj().dumps()<<end_dbgl;
	request->offset += super_sec;
    org_block->do_request(request);
}

void algo_block_io_super::request_done(request_t *request, int err)
{
	if(request->offset < super_sec)
	{
//dbg<<"0 == request->offset "<<this<<DEBUG_END;
		sync_request_done(request, err);
//dbg<<"after sync_request_done"<<this<<DEBUG_END;
	}
	else
	{
		request->offset -= super_sec;
        complete_request(request,err);
	}
}

int algo_block_io_super::read_super(unsigned int len, unsigned char *buf)
{
    request_t request(len, buf);
    request.type = REQUEST_ASYNC_READ;
 // cout<<"before  sync_request;"<<this<<endl;
    int err = sync_request(&request);
 // cout<<"after  sync_request;"<<this<<" "<<err<<endl;  
    return err;
}

int algo_block_io_super::write_super(unsigned int len,const unsigned char* buf)
{
	request_t request(len,(unsigned char*)buf);
	request.type = REQUEST_ASYNC_WRITE;
//dbg<<"before write_super"<<&request<<end_dbgl;
//dbg<<request.data_stack<<end_dbgl;
	int err = sync_request(&request);
//dbg<<"after write_super"<<&request<<end_dbgl;
//dbg<<"err:"<<err<<end_dbgl;
//dbg<<request.data_stack<<end_dbgl;
	return  err;
}


void algo_block_io_super::read_super(json_obj &obj)//一言不合就 throw
{
	int err = open();
	if(err) throw  err;
    unsigned char * buf = nullptr;
    try{
		//FIXME:按照json的结构对将json序列化到固定的长度中去（最好可以处理增量）
		buf = new unsigned char[super_sec<<9];
		unsigned int len = (unsigned int)(super_sec<<9);
		int err = read_super(len, buf);
		if(err) throw err;
		try
		{
			obj.loads((const char *)buf);}
		catch (...)
		{
			//donothing no vols
			obj.clear();
		}
		delete buf;
		close();
	}
	catch (...)
	{
	    if(buf) delete buf;
		close();
		throw (int )-ERROR_TYPE_READ_SUPER;
	}
}


void algo_block_io_super::write_super(const json_obj &obj)//一言不合就 throw
{
	int err = open();
	if(err) throw  err;
	try
	{
		//FIXME:按照json的结构对将json序列化到固定的长度中去（增加）
		string str = obj.dumps();
		if ( str.length() + 1 > sec_to_size(super_sec) )
			throw (int)-ERROR_TYPE_TOOBIGSTRING;
		supper_context context(str.c_str(),sec_to_size(super_sec));
		int err = write_super(context.super_len(), context.super_buf());
		if ( err )
			throw err;
		close();
	}
	catch (...)
	{
		close();
		throw (int )-ERROR_TYPE_WRITE_SUPER;
	}
}

#ifdef TEST_ALGO_SUPER
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
    algo_block_io_super device1(&dev_1,1);
    algo_block_io_super device2(&dev_2,1);
    device1.set_io_callback(&callback_service);
    device2.set_io_callback(&callback_service);
    assert( 0 == device1.open());
    assert( 0 == device2.open());

    device1.read_super(super_len,super_buf);
    device2.write_super(super_len,super_buf);
cout<<"after super"<<endl;
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
