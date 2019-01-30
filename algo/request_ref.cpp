#include "algo.h"

using namespace ServerSan_Algo;

#include <atomic>
#include <boost/concept_check.hpp>
#include "debug.h"

request_ref::request_ref(request_t *_request)
{
    _ref.store(0);
    request = _request;
    err = ERROR_TYPE_OK;
//    dbg<<color_purple<<"request_ref no size"<<end_dbgl;
}

request_ref::request_ref(request_t *_request,int ref)
{
    _ref.store(ref);
    request = _request;
    err = ERROR_TYPE_OK;
//    dbg<<color_purple<<"request_ref ref "<<ref<<end_dbgl;
}

//引用计数加一
request_ref *request_ref::get()
{
    ++_ref;
//    dbg<<color_purple<<"request_ref::get() ref "<<_ref<<end_dbgl;
    return this;
}

//引用计数为0时返回request
request_t *request_ref::put()
{
//dbg<<color_purple<<"request_ref::put() ref "<<_ref<<end_dbgl;
    int org = atomic_fetch_sub(&_ref, 1);
//dbg<<color_purple<<"request_ref::put() org "<<org<<end_dbgl;
    assert(org > 0);
    if ( org == 1 )
        return request;
    return NULL;
}
