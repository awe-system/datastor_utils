#include "lt_reference.h"

lt_reference::lt_reference():ref(0)
{
}

lt_reference *lt_reference::get()
{
    __sync_add_and_fetch(&ref, 1);
    return this;
}

bool lt_reference::put()
{
    return __sync_sub_and_fetch(&ref, 1) == 0;
}

bool lt_reference::get_check_first()
{
    return __sync_fetch_and_add(&ref, 1) == 0;
}
