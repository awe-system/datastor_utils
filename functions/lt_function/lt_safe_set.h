//
// Created by root on 19-2-1.
//

#ifndef DATASTOR_UTILS_LT_SAFE_SET_H
#define DATASTOR_UTILS_LT_SAFE_SET_H
#include <set>
#include <boost/smart_ptr/detail/spinlock.hpp>
#include <boost/thread.hpp>
#include <boost/concept_check.hpp>

template <class T>
class lt_safe_set
{
    boost::detail::spinlock splck;
    std::set<T> set;
public:
    bool insert(T item)
    {
        boost::unique_lock<boost::detail::spinlock> lck(splck);
        auto res = set.insert(item);
        return res.second;
    }
    
    bool erase(T & item)
    {
        boost::unique_lock<boost::detail::spinlock> lck(splck);
        auto it = set.find(item);
        if(it == set.end())
            return false;
        item = *it;
        return true;
    }
    
    bool first(T & item)
    {
        boost::unique_lock<boost::detail::spinlock> lck(splck);
        if(set.empty())
            return false;
        auto it = set.begin();
        item = *it;
        return true;
    }
};


#endif //DATASTOR_UTILS_LT_SAFE_SET_H
