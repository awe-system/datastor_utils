//
// Created by root on 19-12-17.
//

#ifndef DATASTOR_UTILS_DATA_CHECKSUM_H
#define DATASTOR_UTILS_DATA_CHECKSUM_H


#include <string>
#include <lt_data/lt_data_t.h>

class data_checksum
{
public:
    static std::string md5(const lt_data_t &data);
    static std::string bxor(const lt_data_t &data);
};



#endif //DATASTOR_UTILS_CHECKSUM_H
