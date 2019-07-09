//
// Created by root on 19-2-1.
//

#ifndef DATASTOR_UTILS_LT_SAFE_SET_H
#define DATASTOR_UTILS_LT_SAFE_SET_H
#include <set>
#include <boost/smart_ptr/detail/spinlock.hpp>
#include <boost/thread.hpp>
#include <boost/concept_check.hpp>
#include "awe_log.h"
template <class T>
class lt_safe_set
{
    boost::mutex splck;
    std::set<T> set;
public:
    bool insert(T item)
    {
        AWE_MODULE_DEBUG("lt_safe_set", "IIIIIIIIIIIIIIIIIII enter insert  this %p item %p", this, item);
        boost::unique_lock<boost::mutex> lck(splck);
        AWE_MODULE_DEBUG("lt_safe_set", "IIIIIIIIIIIIIIII after lock this %p item %p", this, item);
        auto it = set.find(item);
        AWE_MODULE_DEBUG("lt_safe_set", "IIIIIIIIIIIIIIII after find this %p item %p", this, item);
        if(it == set.end())
        {
            AWE_MODULE_DEBUG("lt_safe_set", "IIIIIIIIIIIIIIII after new item %p item %p", this, item);
            set.insert(item);
            AWE_MODULE_DEBUG("lt_safe_set", "IIIIIIIIIIIIIIII after insert item %p item %p", this, item);
            return true;
        }
       
        AWE_MODULE_DEBUG("cond", "IIIIIIIIIIIIIIIIIIII insert failed!! this %p item %p", this, item);
        return false;
    }
    
    bool erase(T & item)
    {
        AWE_MODULE_DEBUG("cond", "EEEEEEEEEEEEEEEEEEEE enter erase  this %p item %p", this, item);
        boost::unique_lock<boost::mutex> lck(splck);
        AWE_MODULE_DEBUG("cond", "EEEEEEEEEEEEEEEEEEEE erase after lock this %p item %p", this, item);
        auto it = set.find(item);
        AWE_MODULE_DEBUG("cond", "EEEEEEEEEEEEEEEEEEEE erase after find this %p item %p", this, item);
        if(it == set.end())
        {
            AWE_MODULE_DEBUG("cond", "EEEEEEEEEEEEEEEEEEEE erase not found this %p item %p", this, item);
            return false;
        }
        
        item = *it;
        set.erase(it);
        AWE_MODULE_DEBUG("cond", "EEEEEEEEEEEEEEEEEEEE erase success this %p item %p", this, item);
        return true;
    }
    
    bool first(T & item)
    {
        AWE_MODULE_DEBUG("cond", "FFFFFFFFFFFFFFFFFFFFF enter first this %p ", this);
        boost::unique_lock<boost::mutex> lck(splck);
        AWE_MODULE_DEBUG("cond", "FFFFFFFFFFFFFFFFFFFFF first after lock this %p ", this);
        if(set.empty())
        {
            AWE_MODULE_DEBUG("cond", "FFFFFFFFFFFFFFFFFFFFF first empty this %p ", this);
            return false;
        }
        
        for(auto set_mem : set)
        {
            AWE_MODULE_DEBUG("cond", "FFFFFFFFFFFFFFFFFFFFF first mem this %p  set_mem %p", this, set_mem);
        }
        
        auto it = set.begin();
        item = *it;
        AWE_MODULE_DEBUG("cond", "FFFFFFFFFFFFFFFFFFFFF first find this %p item %p", this, item);
        return true;
    }
    
    virtual ~lt_safe_set()
    {
        AWE_MODULE_DEBUG("cond", "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~first find this %p ", this);
    }
};


#endif //DATASTOR_UTILS_LT_SAFE_SET_H
