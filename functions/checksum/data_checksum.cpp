//
// Created by root on 19-12-17.
//

#include "data_checksum.h"
#include "MD5.h"

std::string data_checksum::md5(const lt_data_t &data)
{
    MD5 res(data.get_buf(), data._length);
    return res.toString();
}

std::string data_checksum::bxor(const lt_data_t &data)
{
    unsigned long long res = 0;
    for (ulong i =0;i< (data._length>>3);++i)
    {
        res ^= *((unsigned long *)(&data.get_buf()[i<<3]));
    }
    unsigned long long left = 0;
    auto lef_len = data._length %8;
    for(ulong i = 0;i < lef_len;++i)
    {
        unsigned long long tmp = data.get_buf()[data._length - lef_len +i];
        left ^= tmp<<(i*8);
    }
    res ^= left;
    string str;
    for (auto i = 0; i < 8; ++i) {
        char buf[3] = {0};
        unsigned long long tmp = (res >> ((7-i)*8)) & 0xff;
        sprintf(buf,"%02llx", tmp);
        str += buf;
    }
    return str;
}
