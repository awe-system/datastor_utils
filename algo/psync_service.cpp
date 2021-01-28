//
// Created by root on 2021/1/27.
//

#include <awe_log.h>
#include "psync_service.h"

#define MAX_PSYNC_THREAD_NUM 100

using namespace ServerSan_Algo;

psync_service::~psync_service()
{
    AWE_MODULE_DEBUG("algo", "~psync_service");
}

psync_service::psync_service() : pool(MAX_PSYNC_THREAD_NUM)
{
    AWE_MODULE_DEBUG("algo", "psync_service");
}

void psync_service::do_request(psync_device *block, request_t *request)
{
    AWE_MODULE_DEBUG("algo", "psync_service::do_reques block [%p] request [%p] [%s]",
                     block, request, request->to_json_obj().dumps().c_str());
    pool.submit_task(boost::bind(&psync_service::do_request_inpool, this, block, request));
}

void psync_service::do_request_inpool(psync_device *block, request_t *request)
{
    AWE_MODULE_DEBUG("algo", "IN do_request_inpool block [%p] request [%p] [%s]",
                     block, request, request->to_json_obj().dumps().c_str());
    AWE_MODULE_DEBUG("algo", "before do_request_withcb block [%p] request [%p] [%s]",
                     block, request, request->to_json_obj().dumps().c_str());
    block->do_request_sync(request);
}

