//
// Created by root on 18-3-15.
//

#ifndef LT_DATA_REF_OBJ_H
#define LT_DATA_REF_OBJ_H
#include <atomic>

class ref_obj
{
private:
    std::atomic<int> ref;
public:
    ref_obj();
    void get();
    void put();
    bool is_ref();
};


#endif //LT_DATA_REF_OBJ_H
