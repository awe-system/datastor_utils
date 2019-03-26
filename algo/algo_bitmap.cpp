//
// Created by root on 19-3-22.
//

#include "algo_bitmap.h"

algo_bitmap::algo_bitmap()
{
    page = new unsigned char [PAGE_SIZE]();
    memset(page, 0, PAGE_SIZE);
}

algo_bitmap::algo_bitmap(unsigned char *page_data)
{
    page = new unsigned char [PAGE_SIZE]();
    assert(page_data);
    memcpy(page,page_data,PAGE_SIZE);
}

bool algo_bitmap::test_bit(unsigned long offset)
{
    //FIXME
    return false;
}

void algo_bitmap::set_bit(unsigned  long offset)
{//FIXME

}
