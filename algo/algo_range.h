#ifndef _ALGO_RANGE_H
#define _ALGO_RANGE_H

namespace ServerSan_Algo
{
//代表一种映射关系从org_start,/start
class algo_range
{
public:
    unsigned long org_start;
    unsigned long start;
    unsigned long len;

public:
    algo_range(const json_obj &obj);

    algo_range(unsigned long _org_start, unsigned long _start, unsigned long _len);

    algo_range(const algo_range &other_range);

    algo_range &operator+=(const algo_range &other);

    bool is_neighbour(const algo_range &other);

    bool is_map_neighbour(const algo_range &other);

    void increase(unsigned long _add_size);

    algo_range decrease(unsigned long del_len);

    unsigned long map_address(unsigned long org_addr);

    //去org_section与range org_start len区间的交集的映射
    void map_section(algo_section &org_section, algo_section &section);

    int compare_org_addr(unsigned long org_addr);
#ifdef TEST_ALGO_RANGE
    void dump()const;
#endif

    json_obj to_json_obj()const;
    algo_range(json_obj &obj);
};

typedef vector<algo_range> algo_range_vec_t;
typedef vector<algo_range>::iterator algo_range_it_t;
typedef vector<algo_range>::const_iterator algo_range_const_it_t;

//外部的ranges
class algo_ranges
{
public:
    algo_range_vec_t org_addr_vec;
private:
    unsigned long len;
private:

    void merge();

    algo_range &range_by_pos(algo_range_vec_t &range_vec, int pos);

    algo_range_it_t find_from_org(unsigned long org_addr);

    void insert_range_to_org(const algo_range &range);

public:
#ifdef TEST_ALGO_RANGE
    void dump()const;
#endif

    int rangepos_by_orgaddr(unsigned long org_addr);

    unsigned long length()const;

    algo_ranges(const json_obj &obj);

    algo_ranges();

    algo_ranges(const algo_range &range);

    algo_ranges(const algo_ranges &other);

    //加入的过程会将所有other的org_start重新置入
    algo_ranges &operator+=(const algo_ranges &other);

    unsigned long map_address(unsigned long org_addr);

    void map_section(algo_section &org_section, algo_section_vec_t &secions);
    // unsigned long unmap_address(unsigned long map_addr);
    int get_range_num();
    
    algo_ranges(json_obj &obj);

    json_obj to_json_obj()const;

    void from_json_obj(const json_obj &obj);
};

//解决从一个池中划分多个vol后每个vol映射的问题
class algo_use_ranges
{
    unsigned long len;
    unsigned long left;
    /*在empty中映射关系无意义也就是说org_start无意义*/
    algo_range_vec_t map_addr_vec;
private:
    void insert(const algo_range &range);

    void merge();

public:
#ifdef TEST_ALGO_RANGE
    void dump()const;
#endif

    unsigned long left_size();

    algo_use_ranges(){};

    algo_use_ranges(const json_obj &obj);

    algo_use_ranges(const algo_use_ranges& other);

    algo_use_ranges(unsigned long _len);

    int alloc_ranges(unsigned long _size, algo_ranges &ranges);

    void free_ranges(algo_ranges &ranges);

    void increase(unsigned long _add_size);

    //从empty中扩展某个range的size
    int expand_ranges(algo_ranges &ranges, unsigned long _add_size);

    json_obj to_json_obj()const;

    algo_use_ranges(json_obj &obj);
};

};
#endif