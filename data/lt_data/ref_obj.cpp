//
// Created by root on 18-3-15.
//

#include "ref_obj.h"

ref_obj::ref_obj()
{
    ref = 0;
}

void ref_obj::get()
{
    ++ref;
}

void ref_obj::put()
{
    --ref;
}

bool ref_obj::is_ref()
{
    if(0==ref)
        return false;
    return true;
}
