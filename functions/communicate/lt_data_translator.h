#ifndef LT_DATA_TRANSLATOR_H
#define LT_DATA_TRANSLATOR_H

#include <cstring>
#include <string>
#include <lt_data_t.h>

class lt_data_translator
{
public:
    static bool to_bool(unsigned char *&buf)
    {
        bool res;
        memcpy(&res, buf, sizeof(bool));
        buf += sizeof(bool);
        return res;
    }

    static unsigned int to_uint(unsigned char *&buf)
    {
        unsigned int res;
        memcpy(&res, buf, sizeof(unsigned int));
        buf += sizeof(unsigned int);
        return res;
    }

    static unsigned long to_ulong(unsigned char *&buf)
    {
        unsigned long res;
        memcpy(&res, buf, sizeof(unsigned long));
        buf += sizeof(unsigned long);
        return res;
    }

    static const char *to_char_p(unsigned char *&buf)
    {
        char *res = (char *) buf;
        size_t len = std::strlen(res);
        buf += len + 1;
        return res;
    }

    static std::string to_string(unsigned char *&buf)
    {
        const char *s = to_char_p(buf);
        return std::string(s);
    }


    static void *to_void_p(unsigned char *&buf)
    {
        void *res;
        memcpy(&res, buf, sizeof(void *));
        buf += sizeof(void *);
        return res;
    }

    static lt_data_t to_data(unsigned char *&buf)
    {
        unsigned long len = to_ulong(buf);
        lt_data_t res(len, buf);
        buf += len;
        return res;
    }

    static void by_bool(const bool &src, unsigned char *&buf)
    {
        memcpy(buf, &src, sizeof(bool));
        buf += sizeof(bool);
    }

    static void by_uint(const unsigned int &src, unsigned char *&buf)
    {
        memcpy(buf, &src, sizeof(unsigned int));
        buf += sizeof(unsigned int);
    }

    static void by_ulong(const unsigned long &src, unsigned char *&buf)
    {
        memcpy(buf, &src, sizeof(unsigned long));
        buf += sizeof(unsigned long);
    }

    static void by_void_p(const void *src, unsigned char *&buf)
    {
        memcpy(buf, &src, sizeof(void *));
        buf += sizeof(void *);
    }

    static void by_char_p(const char *src, unsigned char *&buf)
    {
        size_t len = strlen(src) + 1;
        memcpy(buf, src, len);
        buf += len;
    }

    static void by_string(const std::string &src, unsigned char *&buf)
    {
        by_char_p(src.c_str(), buf);
    }


    static void by_data(const lt_data_t &data, unsigned char *&buf)
    {
        by_ulong(data._length, buf);
        memcpy(buf, data.get_buf(), data._length);
        buf += data._length;
    }

    static void skip_bool(unsigned char *&buf)
    {
        buf += sizeof(bool);
    }

    static void skip_uint(unsigned char *&buf)
    {
        buf += sizeof(unsigned int);
    }

    static void skip_ulong(unsigned char *&buf)
    {
        buf += sizeof(unsigned long);
    }

    static void skip_char_p(unsigned char *&buf)
    {
        char *res = (char *) buf;
        size_t len = std::strlen(res) + 1;
        buf += len;
    }

    static void skip_string(unsigned char *&buf)
    {
        skip_char_p(buf);
    }


    static void skip_void_p(unsigned char *&buf)
    {
        buf += sizeof(void *);
    }

    static void skip_data(unsigned char *&buf)
    {
        unsigned long len = to_ulong(buf);
        buf += len;
    }

};
#endif //SERVER_LT_DATA_TRANSLATOR_H
