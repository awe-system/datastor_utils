//
// Created by root on 20-6-17.
//

#ifndef DATASTOR_UTILS_SAFE_VOIDP_MAP_H
#define DATASTOR_UTILS_SAFE_VOIDP_MAP_H

#include <map>
#include <mutex>

class safe_voidp_map
{
    std::mutex m;
    std::map<void *, void*> _map;
public:
    virtual ~safe_voidp_map() = default;
    void mv(std::map<void *, void*> &out_map);
    void cp(std::map<void *, void*> &out_map);
    void in(void *key, void *val);
    void * out(void *key);
};

class comm_io_contexts_t
{
public:
    void* context(void *org_pri);
    
    void *org_pri(void * context);
};

extern comm_io_contexts_t comm_io_contexts;


#endif //DATASTOR_UTILS_SAFE_VOIDP_MAP_H
