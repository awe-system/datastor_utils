//
// Created by root on 18-4-23.
//

#ifndef SS_CONF_SS_CONF_ERROR_H
#define SS_CONF_SS_CONF_ERROR_H
#include <iostream>
using namespace std;
//LOG_PRIORITY_ERROR 800~899
typedef enum
{
    CONF_ERROR_TYPE_OK = 0,
    CONF_ERROR_TYPE_START =800,
    CONF_ERROR_TYPE_UNKNOWN = 899
} CONF_ERROR_T;
#endif //SS_CONF_SS_CONF_ERROR_H
