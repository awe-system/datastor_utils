#ifndef _ALGO_BLOCK_OPEN_REF_H
#define _ALGO_BLOCK_OPEN_REF_H
#include <mutex>
// #include <condition_variable>
namespace ServerSan_Algo
{

class algo_block_open_ref: public block_io,public block_io_callback
{
public:
	block_io *org_block;
private:
	std::mutex open_lock;
	int ref;
public:
	algo_block_open_ref(block_io *_block);
	
	algo_block_open_ref(const algo_block_open_ref & other);

	unsigned long long get_sector_num()const;

	int open(void);

	void close(void);

	void do_request(request_t *request);

	void request_done(request_t *request, int err);
};

};
#endif