#include "algo.h"
#include "debug.h"
using namespace ServerSan_Algo;

algo_block_multi_super::algo_block_multi_super()
{}
algo_block_multi_super::algo_block_multi_super(algo_block_vec_t &blocks)
{
    super_blocks = blocks;
}
algo_block_multi_super::algo_block_multi_super(const algo_block_multi_super & other)
{
    super_blocks = other.super_blocks;
}

void algo_block_multi_super::read_super(json_obj &obj)
{
    json_obj last_obj;
    for(algo_block_vec_it it = super_blocks.begin();
        it!=super_blocks.end();
        ++it)
    {
        algo_block_io_super * super = (algo_block_io_super *)*it;
        super->read_super(obj);
        if(!last_obj.is_empty() && last_obj != obj) throw ERROR_TYPE_READ_SUPER;
        last_obj = obj;
    }
}

void algo_block_multi_super::write_super(const json_obj &obj)
{
    for(algo_block_vec_it it = super_blocks.begin();
        it!=super_blocks.end();
        ++it)
    {
        algo_block_io_super * super = (algo_block_io_super *)*it;
        super->write_super(obj);
        //printf("algo_block_multi_super::write_super super = %p\n", super);
    }
//dbg<<"write_super"<<color_green<<"done"<<end_dbg;
}
