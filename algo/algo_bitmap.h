//
// Created by root on 19-3-22.
//

#ifndef DATASTOR_UTILS_ALGO_BITMAP_H
#define DATASTOR_UTILS_ALGO_BITMAP_H

#include "lt_data/lt_data_t.h"

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif
//FIXME 暂时不实现
class algo_bitmap
{
    unsigned char *page;
public:
    explicit algo_bitmap();
    
    explicit algo_bitmap(unsigned char *page_data);
    
    bool test_bit(unsigned long offset);
    
    void set_bit(unsigned  long offset);
    
    virtual void clear_bit(unsigned  long offset) = 0;
    
    virtual unsigned long first_1_after() = 0;
    
    virtual unsigned long first_0_after() = 0;
};


#endif //DATASTOR_UTILS_ALGO_BITMAP_H
