//
// Created by root on 20-3-23.
//

#ifndef DATASTOR_UTILS_LT_MEMPOOL_H
#define DATASTOR_UTILS_LT_MEMPOOL_H
//lib tool memory pool
#include "assert.h"
#include "lt_stack.h"
#include "../../algo/debug.h"

class lt_mempool
{
    lt_stack stack;
public:
    lt_mempool(int unit_size, int unit_cnt)
    {
        assert(unit_size != 0);
        assert(unit_cnt != 0);
        for (auto i = 0; i< unit_cnt; ++i)
        {
            void *buf = malloc(unit_size);
            assert(buf);
//            dbg << "init:" << buf << end_dbgl;
            stack.push_private(buf);
        }
    }
    
    ~lt_mempool()
    {
//        dbg << "~lt_mempool()" << end_dbgl;
        void *buf = nullptr;
        do
        {
            buf = stack.pop_private();
            if(buf)
            {
//                dbg << "destroy:" << buf << end_dbgl;
                ::free(buf);
            }
        }while(buf);
//        dbg << "~lt_mempool() end" << end_dbgl;
    }
    
    void *alloc()
    {
        return stack.pop_private();
    }
    
    void free(void *buf)
    {
        stack.push_private(buf);
    }
};

#endif //DATASTOR_UTILS_LT_MEMPOOL_H
