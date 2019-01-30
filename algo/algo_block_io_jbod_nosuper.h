#ifndef _ALGO_BLOCK_IO_JBOD_NO_SUPER_H
#define _ALGO_BLOCK_IO_JBOD_NO_SUPER_H

#include <vector>
#include <mutex>

using namespace std;
namespace ServerSan_Algo
{
//这里假设do_request与扩容之间没有并发 关于这点是有外部引用计数保证的

class algo_block_io_jbod_nosuper : public block_io, public block_io_callback
{
private:
    std::mutex m;
    algo_block_vec_t _block_vector;
    algo_nocopy_request_mapper _request_mapper;
    unsigned long _sector_count;
    bool _is_open;
    
public:
    algo_block_io_jbod_nosuper(algo_block_vec_t &_blocks);
    ~algo_block_io_jbod_nosuper();

    int add_block(block_io *block);

    unsigned long long get_sector_num() const;

    int open(void);

    void close(void);

    void do_request(request_t *request);

    void request_done(request_t *request, int err);
};

} //namespace
#endif