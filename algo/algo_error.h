#ifndef _ALGO_ERROR_H
#define _ALGO_ERROR_H

#include <stdlib.h>

namespace ServerSan_Algo
{
//LOG_PRIORITY_ERROR 0 ~ 99
typedef enum
{
    ERROR_TYPE_OK = 0,
    ERROR_TYPE_NOTSUPPORT =1,
    ERROR_TYPE_MEMORY,
    ERROR_TYPE_SAMENAME,
    ERROR_TYPE_NOTFOUND,
    ERROR_TYPE_NOTOPEN,
    ERROR_TYPE_DEVICE,
    ERROR_TYPE_WRONGTYPE,
    ERROR_TYPE_WRONGLENGTH,
    ERROR_TYPE_SUPER_MISMATCH,
    ERROR_TYPE_WRONGDEVICE,
    ERROR_TYPE_GENJOBD,
    ERROR_TYPE_NOPOOL,
    ERROR_TYPE_NOVOL,
    ERROR_TYPE_NODEVICE,
    ERROR_TYPE_REGROUP,
    ERROR_TYPE_TOOBIGSTRING,
    ERROR_TYPE_WRITE_SUPER,
    ERROR_TYPE_READ_SUPER,
    ERROR_TYPE_UPDATE_SUPER,
    ERROR_TYPE_CREATE_NOSUPER,
    ERROR_TYPE_NULL_DEV,
    ERROR_TYPE_ERR_POOLNAME,
    ERROR_TYPE_ERR_EXPAND,
    ERROR_TYPE_ERR_NOCAPACITY,
    ERROR_TYPE_JSON_FORMAT,
    ERROR_TYPE_COPYNUM,
    ERROR_TYPE_UNKNOWN = 99,
} ERROR_T;

static inline std::string error_str(int err)
{
    switch ( abs(err))
    {
        case ERROR_TYPE_OK:
            return std::string("OK\n");
        case ERROR_TYPE_NOTSUPPORT:
            return std::string("Not Support\n");
        case ERROR_TYPE_UNKNOWN:
        default:
            return std::string("Unkown error\n");
    }
}
};

#endif