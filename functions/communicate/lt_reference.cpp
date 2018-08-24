#include "lt_reference.h"

lt_reference::lt_reference()
{
    ref.store(0);
}

lt_reference *lt_reference::get()
{
    ++ref;
    return this;
}

bool lt_reference::put()
{
    if(--ref == 0)
    {
        return true;
    }
    return false;
}
