//
// Created by root on 2021/1/27.
//

#ifndef DATASTOR_UTILS_PSYNC_SERVICE_H
#define DATASTOR_UTILS_PSYNC_SERVICE_H

#include <lt_function/thread_pool.hpp>
#include "algo.h"

namespace ServerSan_Algo
{
class psync_service
{
    data_channel::thread_pool pool;
private:
    void do_request_inpool(psync_device *block, request_t * request);
public:
    psync_service();
    virtual ~psync_service();
    void do_request(psync_device *block, request_t * request);
};

psync_service * get_psync_service();
};

#endif //DATASTOR_UTILS_PSYNC_SERVICE_H
