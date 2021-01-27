//
// Created by root on 2021/1/27.
//

#include <awe_log.h>
#include "psync_service.h"

#define MAX_PSYNC_THREAD_NUM 100

using namespace ServerSan_Algo;

static void request_type2sync(ServerSan_Algo::request_t *request)
{
    switch(request->type)
    {
        case REQUEST_ASYNC_WRITE:
            request->type = REQUEST_SYNC_WRITE;
            return;
        case REQUEST_ASYNC_READ:
            request->type = REQUEST_SYNC_READ;
            return;
        default:
            abort();
    }
}

static void request_type2async(ServerSan_Algo::request_t *request)
{
    switch(request->type)
    {
        case REQUEST_SYNC_WRITE:
            request->type = REQUEST_ASYNC_WRITE;
            return;
        case REQUEST_SYNC_READ:
            request->type = REQUEST_ASYNC_READ;
            return;
        default:
            abort();
    }
}

psync_service::~psync_service()
{
    AWE_MODULE_DEBUG("algo", "~psync_service");
}

psync_service::psync_service() : pool(MAX_PSYNC_THREAD_NUM)
{
    AWE_MODULE_DEBUG("algo", "psync_service");
}

void psync_service::do_request(block_io *block, request_t *request)
{
    AWE_MODULE_DEBUG("algo", "psync_service::do_reques block [%p] request [%p] [%s]",
                     block, request, request->to_json_obj().dumps().c_str());
    pool.submit_task(boost::bind(&psync_service::do_request_inpool, this, block, request));
}

void psync_service::request_done(request_t *request, int err)
{
    AWE_MODULE_DEBUG("algo", "IN psync_service::request_done request [%p] [%s]",
                     request, request->to_json_obj().dumps().c_str());
    request_type2async(request);
    block_io *block = static_cast<block_io *>(request->pop_private());
    ServerSan_Algo::algo_obj* obj = reinterpret_cast<algo_obj *>(block);
    obj->assert_legal();
    AWE_MODULE_DEBUG("algo", "OUT psync_service::request_done request [%p] [%s]",
                     request, request->to_json_obj().dumps().c_str());
    block->complete_request(request, err);
}

void psync_service::do_request_inpool(block_io *block, request_t *request)
{
    AWE_MODULE_DEBUG("algo", "IN do_request_inpool block [%p] request [%p] [%s]",
                     block, request, request->to_json_obj().dumps().c_str());
    request->push_private(block);
    request_type2sync(request);
    AWE_MODULE_DEBUG("algo", "before do_request_withcb block [%p] request [%p] [%s]",
                     block, request, request->to_json_obj().dumps().c_str());
    block->do_request_withcb(request, this);
}

static psync_service *service;
psync_service *ServerSan_Algo::get_psync_service()
{
    if(service)
    {
        return service;
    }
    return new(std::nothrow)psync_service();
}
