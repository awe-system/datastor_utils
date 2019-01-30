#ifndef _H_REQUEST_REF
#define _H_REQUEST_REF

#include <atomic>

namespace ServerSan_Algo
{

// using namespace std;
class request_ref
{
    request_t *request;
    std::atomic_int _ref;
public:
    int err;
public:
    request_ref(request_t *_request);
    request_ref(request_t *_request,int ref);

    //引用计数加一
    request_ref *get();

    //引用计数为0时返回request
    request_t *put();
};

};
#endif