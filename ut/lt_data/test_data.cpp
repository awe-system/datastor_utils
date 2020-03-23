//
// Created by root on 20-3-23.
//
#include <lt_function/utils.h>
#include "lt_mempool.h"
#include "../../algo/debug.h"

int main(int argc, const char *argv[])
{
    int unit_size = 128 * 1024 * 1024;
    int unit_cnt = 5;
    int max_cnt = 200;
    int cnt = max_cnt;
    while (cnt--)
    {
        lt_stack stack;
        void * buf = nullptr;
        lt_mempool pool(unit_size, unit_cnt);
        dbg << "lt_mempool struct ok " <<max_cnt - cnt<< end_dbgl;
        int this_cnt = unit_cnt;
        while(this_cnt--)
        {
            buf = pool.alloc();
            assert(buf);
            stack.push_private(buf);
        }
        this_cnt = unit_cnt;
        while(this_cnt--)
        {
            buf = pool.alloc();
            assert(!buf);
        }
        this_cnt = 0;
        do
        {
            buf = stack.pop_private();
            if(buf)
            {
                pool.free(buf);
                ++this_cnt;
            }
        }while(buf);
        assert(this_cnt ==  unit_cnt);
        dbg<<"lt_mempool before destroy left cnt "<<max_cnt - cnt<<end_dbgl;
        string free_res ;
        buf_exec_cpp("free -h",free_res);
        dbg<<end_dbg<<free_res<<end_dbgl;
    }
    
}