//
// Created by root on 20-6-17.
//

#include <assert.h>
#include "safe_voidp_map.h"

void safe_voidp_map::mv(std::map<void *, void *> &out_map)
{
    std::unique_lock<std::mutex> lck(m);
    for( auto it : _map)
    {
        out_map.insert(std::make_pair(it.first,it.second));
    }
    _map.clear();
}

void safe_voidp_map::in(void *key, void *val)
{
    assert(key && val);
    std::unique_lock<std::mutex> lck(m);
    auto it = _map.insert(std::make_pair(key,val));
    assert(it.second);
}

void *safe_voidp_map::out(void *key)
{
    assert(key);
    std::unique_lock<std::mutex> lck(m);
    auto it = _map.find(key);
    if(it == _map.end()) return nullptr;
    assert(it->second);
    return it->second;
}

void safe_voidp_map::cp(std::map<void *, void *> &out_map)
{
    std::unique_lock<std::mutex> lck(m);
    for( auto it : _map)
    {
        out_map.insert(std::make_pair(it.first,it.second));
    }
}

struct comm_io_context
{
    void *org_pri;
};

void *comm_io_contexts_t::context(void * org_pri)
{
    comm_io_context * context = new comm_io_context;
    assert(context);
    context->org_pri = org_pri;
    return (void *)context;
}

void *comm_io_contexts_t::org_pri(void *context)
{
    assert(context);
    comm_io_context * con = static_cast<comm_io_context *>(context);
    void *org = con->org_pri;
    delete con;
    return org;
}

comm_io_contexts_t comm_io_contexts;
