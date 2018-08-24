#ifndef _COMMUNICATE_LT_BIDRECTIONAL_MAP_H
#define _COMMUNICATE_LT_BIDRECTIONAL_MAP_H

#include "map"

template <typename Tkey_, typename Tval_>
class lt_bidirectional_map
{
public:
    std::map<Tkey_,Tval_> k_map;
    std::map<Tval_,Tkey_> v_map;
public:
    lt_bidirectional_map()
    {
    }

    Tval_ val_by_key(Tkey_ key)
    {
        return k_map[key];
    }

    Tkey_ key_by_val(Tval_ val)
    {
        return v_map[val];
    }

    void insert(Tkey_ key, Tval_ val)
    {
        k_map.insert(std::make_pair(key,val));
        v_map.insert(std::make_pair(val,key));
    }

    void clear()
    {
        k_map.clear();
        v_map.clear();
    }

    void erase(Tkey_ key)
    {
        v_map.erase(k_map[key]);
        k_map.erase(key);
    }

    void erase_val(Tval_ val)
    {
        k_map.erase(v_map[val]);
        v_map.erase(val);
    }
};


#endif //TEST_COMMUNICATE_LT_BIDRECTIONAL_MAP_H
