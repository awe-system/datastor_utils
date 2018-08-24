#ifndef LT_FUNCTION_LT_FUNCTION_ERROR_H
#define LT_FUNCTION_LT_FUNCTION_ERROR_H
//ERROR 1000~1099
typedef enum
{
    RPC_ERROR_TYPE_OK,
    RPC_ERROR_TYPE_NET_BROKEN = 1000,
    RPC_ERROR_TYPE_CONNECT_FAIL,
    RPC_ERROR_TYPE_MEMORY,
    FUN_ERROR_TYPE_UNKNOWN = 1099
} LT_FUNERROR_T;

static inline int boost_err_translate(const boost::system::error_code &error)
{
    if ( error )
    {
        return -RPC_ERROR_TYPE_NET_BROKEN;
    }
    return 0;
}


#endif //LT_FUNCTION_LT_FUNCTION_ERROR_H

