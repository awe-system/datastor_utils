#ifndef _ALGO_BLOCK_IO_SUPER_H
#define _ALGO_BLOCK_IO_SUPER_H
#include <condition_variable>
#include "lt_function/stronger_cond.h"

namespace ServerSan_Algo
{

class algo_block_io_super:public algo_block_open_ref
{
private:
	unsigned long long super_sec;

	std::mutex sync_m;
	stronger_cond super_cond;
private:
	int sync_request(request_t *request);
	void sync_request_done(request_t *request, int err);
public:
	algo_block_io_super(block_io *_block,unsigned long _size);

	algo_block_io_super(const algo_block_io_super & other);

	unsigned long long get_sector_num() const;

	void do_request(request_t *request);

	void request_done(request_t *request, int err);

	int read_super(unsigned int len,unsigned char* buf);

	int write_super(unsigned int len,const unsigned char* buf);

	void read_super(json_obj &obj);//一言不合就 throw

    void write_super(const json_obj &obj);//一言不合就 throw
    
    virtual ~algo_block_io_super(){};
};

typedef vector<algo_block_io_super *> io_super_vector_t;
typedef vector<algo_block_io_super *>::iterator io_super_vector_it;
};
#endif