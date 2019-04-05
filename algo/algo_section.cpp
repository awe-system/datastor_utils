#include "algo.h"
#include "algo_section.h"

#include <iostream>
#include <iomanip>

namespace ServerSan_Algo
{
#define MIN(a, b) (a<b)?a:b
#define MAX(a, b) (a>b)?a:b

algo_section::algo_section() : start(0), len(0)
{
}

algo_section::algo_section(request_t *request)
{
    start = request->offset;
    len   = size_to_sec(request->len);
}

algo_section::algo_section(unsigned long _start, unsigned long _len) : start(
        _start), len(_len)
{
}

algo_section::algo_section(const algo_section &other)
{
    start = other.start;
    len   = other.len;
}

void algo_section::dump() const
{
    cout << setw(4) << start << " -- " << setw(4) << len;
}

void algo_section::split(const algo_section &small_sec,
                         algo_section_vec_t &sections) const
{
    assert(start <= small_sec.start && len >= small_sec.len);
    if ( start < small_sec.start )
    {
        sections.push_back(algo_section(start, small_sec.start - start));
    }
    if ( start + len > small_sec.start + small_sec.len )
    {
        sections.push_back(algo_section(small_sec.start + small_sec.len,
                                        start + len - small_sec.start -
                                        small_sec.len));
    }
}

void algo_section::intersection(const algo_section &other, algo_section &res)
{
    res.start = 0;
    res.len   = 0;
    if ( start >= other.start + other.len || other.start >= start + len )
    {
        return;
    }
    res.start = MAX(start, other.start);
    res.len   = MIN(start + len, other.start + other.len) - res.start;
}

bool algo_section::operator<(const algo_section &other) const
{
    return start + len <= other.start;
}

bool algo_section::operator>(const algo_section &other) const
{
    return start >= other.len + other.start;
}

void algo_section::from_json_obj(const json_obj &obj)
{
    if ( obj.type() != json_obj_type_array || obj.size() != 2 )
    {
        throw (int) -ServerSan_Algo::ERROR_TYPE_JSON_FORMAT;
    }
    start = obj[0].ll_val;
    len   = obj[1].ll_val;
}

json_obj algo_section::to_json_obj() const
{
    json_obj obj;
    obj.set_array();
    obj.append(json_obj((long long) start));
    obj.append(json_obj((long long) len));
    return obj;
}

bool algo_section::is_point_insections(unsigned long offset)
{
    algo_section res;
    algo_section section(offset, 1);
    intersection(section, res);
    if ( res.len == 0 )
    { return false; }
    return true;
}

algo_section algo_section::alloc(unsigned long _len)
{
    assert(_len != 0 && _len <= len);
    algo_section section(start, _len);
    len -= _len;
    start += _len;
    return section;
}

bool algo_section::is_neighbour(const algo_section &other)
{
    return (start + len == other.start || start == other.start + other.len);
}

void algo_section::merge(const algo_section &other)
{
    if ( start + len == other.start )
    {
        len += other.len;
    }
    else if ( start == other.start + other.len )
    {
        start = other.start;
        len += other.len;
    }
}


void dump_algo_sections(const algo_section_vec_t &sections)
{
    for ( algo_section_const_it it = sections.begin(); it != sections.end();
          ++it )
    {
        it->dump();
        cout << "\t";
    }
    cout << endl;
}

} //namespace