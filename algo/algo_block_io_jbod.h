#ifndef _ALGO_BLOCK_IO_JBOD_H
#define _ALGO_BLOCK_IO_JBOD_H

#include <vector>
#include <mutex>

using namespace std;
namespace ServerSan_Algo
{
//这里假设do_request与扩容之间没有并发 关于这点是有外部引用计数保证的

class algo_block_io_jbod : public block_io, public block_io_callback
{
private:
   algo_block_io_jbod_nosuper *nosuper;
   algo_block_vec_t super_blocks;
   algo_block_multi_super multi_super;
private:
    void clear_super_blocks();

    void gen_super_blocks(algo_block_vec_t &_blocks);
    algo_block_io_super * add_super_block(block_io *_block);
public:
    ~algo_block_io_jbod();
    algo_block_io_jbod(algo_block_vec_t &_blocks);

    int add_block(block_io *block);

    unsigned long long get_sector_num()const;

    int open(void);

    void close(void);

    void do_request(request_t *request);

    void request_done(request_t *request, int err);

    void read_super(json_obj &obj);//一言不合就 throw

    void write_super(const json_obj &obj);//一言不合就 throw
   
};

} //namespace
#endif