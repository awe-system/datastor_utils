#include "lt_reference.h"

#include "awe_log.h"
lt_reference::lt_reference():ref(0)
{
    AWE_MODULE_DEBUG("lt_ref","lt_reference::lt_reference this %p", this);
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
    if( 0 == __sync_fetch_and_add(&ref, 1) )
        return true;
    return false;
}

lt_reference::~lt_reference()
{
    AWE_MODULE_DEBUG("lt_ref","lt_reference::~lt_reference this %p", this);
}
