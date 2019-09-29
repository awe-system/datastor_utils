#include "algo.h"

using namespace ServerSan_Algo;
#ifdef TEST_ALGO_RANGE
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <boost/algorithm/string.hpp>
using namespace std;
#endif

/********************************************************************
*********************************************************************
****************************algo_range*******************************
*********************************************************************
*********************************************************************/
#ifdef TEST_ALGO_RANGE
void algo_range::dump()const
{
    cout<<setw(4)<<org_start<<" -> "<<setw(4)<<start<<" ("<<len<<")";
}
#endif

algo_range::algo_range(unsigned long _org_start, unsigned long _start, unsigned long _len) :
        org_start(_org_start),
        start(_start),
        len(_len)
{
}

algo_range::algo_range(const algo_range &other_range)
{
    org_start = other_range.org_start;
    start = other_range.start;
    len = other_range.len;
}

algo_range &algo_range::operator+=(const algo_range &other)
{
    assert(is_map_neighbour(other));
    len += other.len;
    unsigned long other_start = other.start;
    unsigned long other_org = other.org_start;
    start = (start < other_start) ? start : other_start;
    org_start = (org_start < other_org) ? org_start : other_org;
    return *this;
}

bool algo_range::is_neighbour(const algo_range &other)
{
    // cout<<"self  start:"<<start<<" len:"<<len<<" org_start:"<<org_start<<endl;
    // cout<<"other start:"<<other.start<<" len:"<<other.len<<" org_start:"<<other.org_start<<endl;
    return ((start + len == other.start && org_start + len == other.org_start) ||
            (start == other.start + other.len && org_start == other.org_start + other.len));
}

bool algo_range::is_map_neighbour(const algo_range &other)
{
    return (start + len == other.start ||
            start == other.start + other.len);
}

void algo_range::increase(unsigned long _add_size)
{
    len += _add_size;
}

algo_range algo_range::decrease(unsigned long del_len)
{
    // cout<<"len:"<<len<<" del_len:"<<del_len<<endl;
    assert(del_len < len && del_len > 0);
    algo_range range(org_start, start, del_len);
    start += del_len;
    org_start += del_len;
    len -= del_len;
    return range;
}

unsigned long algo_range::map_address(unsigned long org_addr)
{
    return org_addr + start - org_start;
}

void algo_range::map_section(algo_section &org_section, algo_section &section)
{
    unsigned long _org_start = (org_section.start > org_start) ? org_section.start : org_start;
    unsigned long _org_end = (org_section.start + org_section.len < org_start + len) ?
                             (org_section.start + org_section.len) : (org_start + len);
    section.len = _org_end - _org_start;
    section.start = map_address(_org_start);
}
// unsigned long algo_range::unmap_address(unsigned long map_addr)
// {
// 	return map_addr - start + org_start; 
// };

int algo_range::compare_org_addr(unsigned long org_addr)
{
    if ( org_addr < org_start )
        return -1;
    if ( org_addr >= org_start + len )
        return 1;
    return 0;
}


json_obj algo_range::to_json_obj()const
{
    json_obj obj;
    obj.append(to_string(org_start));
    obj.append(to_string(start));
    obj.append(to_string(len));
    return obj;
}

algo_range::algo_range(const json_obj &obj_input)
{
    json_obj obj = obj_input;
    org_start = atoll(obj[0].s_val.c_str());
    start = atoll(obj[1].s_val.c_str());
    len = atoll(obj[2].s_val.c_str());;
}

// int algo_range::compare_map_addr(unsigned long map_addr)
// {
// 	if(map_addr < start) return -1;
// 	if(map_addr >= start + len) return 1;
// 	return 0;
// }


/********************************************************************
*********************************************************************
****************************algo_ranges******************************
*********************************************************************
*********************************************************************/
unsigned long algo_ranges::length()const
{
    return len;
}


void algo_ranges::merge()//将所有前后都一致的内容进行合并
{    //要想合并至少vec中应该存在两个元素
    algo_range_it_t last = org_addr_vec.begin();
    if ( last == org_addr_vec.end())
        return;
    algo_range_it_t it = last + 1;
    while ( it != org_addr_vec.end())
    {
        algo_range &last_range = *last;
        algo_range &cur_range = *it;
        if ( last_range.is_neighbour(cur_range))
        {
            last_range += cur_range;
            it = org_addr_vec.erase(it);
            // merge_map_range(last_range.start);
        } else
        {
            last = it;
            ++it;
        }
    }
}

#ifdef TEST_ALGO_RANGE
void algo_ranges::dump()const
{
    int cnt = 0;
    for(algo_range_const_it_t it = org_addr_vec.begin();
        it != org_addr_vec.end();
        ++it)
    {
        algo_range range = *it;
        range.dump();
        if(++cnt %16 == 0) cout<<endl;
        else cout<<"\t";
    }
    cout<<endl;
}
#endif

algo_ranges::algo_ranges() : len(0)
{

}

algo_ranges::algo_ranges(const json_obj &obj_input)
{
    json_obj obj = obj_input;
    len = atoll(obj["len"].s_val.c_str());
    const json_obj &vec_obj = obj["vec"];
    for( json_array_const_it it = vec_obj.array_val.begin();
        it != vec_obj.array_val.end();
        ++it)
    {
        const json_obj &item = *it;
        org_addr_vec.push_back(algo_range(item));
    }
}

json_obj algo_ranges::to_json_obj() const
{
    json_obj obj("len",to_string(len));
    json_obj vec_obj;
    vec_obj.set_array();
    for(algo_range_const_it_t it = org_addr_vec.begin();
        it!= org_addr_vec.end();
        ++it)
    {
        const algo_range &range = *it;
        json_obj item = range.to_json_obj();;
        vec_obj.append(item);
    }

    obj.merge(json_obj("vec",vec_obj));
    return obj;
}

algo_ranges::algo_ranges(const algo_range &range)
{
    // insert_range_to_map(range);
    insert_range_to_org(range);
    len = range.len;
}

algo_ranges::algo_ranges(const algo_ranges &other)
{
    // map_addr_vec.clear();
    org_addr_vec.clear();
    for ( algo_range_const_it_t it = other.org_addr_vec.begin(); it != other.org_addr_vec.end(); ++it )
    {
        algo_range range = *it;
        // insert_range_to_map(range);
        insert_range_to_org(range);
    }
    len = other.len;
}

//加入的过程会将所有other的org_start重新置入
algo_ranges &algo_ranges::operator+=(const algo_ranges &other)
{
    for ( algo_range_const_it_t it = other.org_addr_vec.begin(); it != other.org_addr_vec.end(); ++it )
    {
        algo_range range = *it;
        range.org_start = len;
        len += range.len;
        // insert_range_to_map(range);
        // cout<<"insert_range_to_org"<<endl;
        insert_range_to_org(range);
    }
    // cout<<"merge"<<endl;
    merge();
    return *this;
}

algo_range &algo_ranges::range_by_pos(algo_range_vec_t &range_vec, int pos)
{
    return range_vec[pos];
}

algo_range_it_t algo_ranges::find_from_org(unsigned long org_addr)
{
    int start = 0;
    int end = org_addr_vec.size();
    int mid = (start + end) / 2;
    if ( end == 0 )
        return org_addr_vec.end();
    algo_range_it_t it = org_addr_vec.begin();
    while ( 1 )
    {
        if ( org_addr_vec[mid].start == org_addr )
            return it + mid;
        else if ( org_addr_vec[mid].start > org_addr )
            end = mid;
        else
            start = mid + 1;
        if ( start == mid )
            return org_addr_vec.end();
        mid = (start + end) / 2;
    }
    abort();
}

void algo_ranges::insert_range_to_org(const algo_range &range)
{
    int start = 0;
    int end = org_addr_vec.size();
    int mid = (start + end) / 2;
    if ( end == 0 )
    {
        org_addr_vec.push_back(range);
        return;
    }
    algo_range_it_t it = org_addr_vec.begin();
    // dump();
    // range.dump();
    while ( 1 )
    {
        assert(org_addr_vec[mid].org_start != range.org_start);
        if ( start == mid )
        {
            if ( org_addr_vec[mid].org_start > range.org_start )
                org_addr_vec.insert(it + mid, range);
            else
                org_addr_vec.insert(it + mid + 1, range);
            return;
        }
        if ( org_addr_vec[mid].org_start > range.org_start )
            end = mid;
        else
            start = mid;
        mid = (start + end) / 2;
    }
}

int algo_ranges::rangepos_by_orgaddr(unsigned long org_addr)
{
    int start = 0;
    int end = org_addr_vec.size();
    int mid = (start + end) / 2;
    assert(end != 0);
    while ( 1 )
    {
        algo_range &range = range_by_pos(org_addr_vec, mid);
        int comp = range.compare_org_addr(org_addr);
        if ( comp == 0 )
            return mid;
        assert(start != mid);//相等时一定已经返回
        if ( comp < 0 )
            end = mid;
        else
            start = mid + 1;
        mid = (start + end) / 2;
    }
}

unsigned long algo_ranges::map_address(unsigned long org_addr)
{
    int pos = rangepos_by_orgaddr(org_addr);
    algo_range &range = range_by_pos(org_addr_vec, pos);
    return range.map_address(org_addr);

};


void algo_ranges::map_section(algo_section &org_section, algo_section_vec_t &sections)
{
    unsigned long cur_len = 0;
    // org_section.dump();
    // cout<<endl;
    for ( int pos = rangepos_by_orgaddr(org_section.start); cur_len != org_section.len; pos++ )
    {
        // cout<<"in for"<<endl;
        algo_section section;
        algo_range &range = range_by_pos(org_addr_vec, pos);
        range.map_section(org_section, section);
        // section.dump();
        // cout<<endl;
        sections.push_back(section);
        cur_len += section.len;
    }
}

int algo_ranges::get_range_num()
{
    return (int) org_addr_vec.size();
}

void algo_ranges::from_json_obj(const json_obj &obj_input)
{
    json_obj obj = obj_input;
    len = atoll(obj["len"].s_val.c_str());
    const json_obj &vec_obj = obj["vec"];
    org_addr_vec.clear();     //TODO 这里全部清空。
    for( json_array_const_it it = vec_obj.array_val.begin();
         it != vec_obj.array_val.end();
         ++it)
    {
        const json_obj &item = *it;
        org_addr_vec.push_back(algo_range(item));
    }
}



/********************************************************************
*********************************************************************
****************************algo_use_ranges**************************
*********************************************************************
*********************************************************************/
#ifdef TEST_ALGO_RANGE
void algo_use_ranges::dump()const
{
    cout<<"len:"<<len<<" left:"<<left<<endl;
    int cnt = 0;
    for(algo_range_const_it_t it = map_addr_vec.begin();
        it != map_addr_vec.end();
        ++it)
    {
        algo_range range = *it;
        cout<<setw(4)<<range.start<<" -- "<<setw(4)<<range.start+range.len<<" ("<<range.len<<")";
        if(++cnt %16 == 0) cout<<endl;
        else cout<<"\t";
    }
    cout<<endl;
}
#endif

void algo_use_ranges::merge()
{
    algo_range_it_t last = map_addr_vec.begin();
    if ( last == map_addr_vec.end())
        return;
    algo_range_it_t it = last + 1;
    while ( it != map_addr_vec.end())
    {
        algo_range &last_range = *last;
        algo_range &cur_range = *it;
        if ( last_range.is_map_neighbour(cur_range))
        {
            last_range += cur_range;
            it = map_addr_vec.erase(it);
        } else
            last = it++;
    }
}

void algo_use_ranges::insert(const algo_range &range)
{
    int start = 0;
    int end = map_addr_vec.size();
    int mid = (start + end) / 2;
    if ( end == 0 )
    {
        map_addr_vec.push_back(range);
        return;
    }
    algo_range_it_t it = map_addr_vec.begin();
    while ( 1 )
    {
        assert(map_addr_vec[mid].start != range.start);
        if ( start == mid )
        {
            if ( map_addr_vec[mid].start > range.start )
                map_addr_vec.insert(it + mid, range);
            else
                map_addr_vec.insert(it + mid + 1, range);
            return;
        };
        if ( map_addr_vec[mid].start > range.start )
            end = mid;
        else
            start = mid;
        mid = (start + end) / 2;
    }
}

unsigned long algo_use_ranges::left_size()
{
    return left;
}


json_obj algo_use_ranges::to_json_obj()const
{
    json_obj obj("len",to_string(len));
    obj.merge(json_obj("left",to_string(left)));
    json_obj vec_obj;
    vec_obj.set_array();
    for(algo_range_const_it_t it = map_addr_vec.begin();
        it!= map_addr_vec.end();
        ++it)
    {
        const algo_range &range = *it;
        json_obj item = range.to_json_obj();
        vec_obj.append(item);
    }

    obj.merge(json_obj("vec",vec_obj));
    return obj;
}

algo_use_ranges::algo_use_ranges(const json_obj &obj_input)
{
    json_obj obj = obj_input;
    len = atoll(obj["len"].s_val.c_str());
    left = atoll(obj["left"].s_val.c_str());
    const json_obj &vec_obj = obj["vec"];
    for( json_array_const_it it = vec_obj.array_val.begin();
        it != vec_obj.array_val.end();
        ++it)
    {
        const json_obj &item = *it;
        map_addr_vec.push_back(algo_range(item));
    }
}

algo_use_ranges::algo_use_ranges(const algo_use_ranges& other)
{
    len = other.len;
    left = other.left;
    map_addr_vec = other.map_addr_vec;
}


algo_use_ranges::algo_use_ranges(unsigned long _len) : len(_len), left(_len)
{
    algo_range range(0, 0, _len);
    map_addr_vec.push_back(range);
};

int algo_use_ranges::alloc_ranges(unsigned long _size, algo_ranges &ranges)
{
    if ( _size < 0 || _size > left )
        return -ERROR_TYPE_WRONGLENGTH;

    algo_range_it_t it = map_addr_vec.begin();
    unsigned long cur_len = 0;
    while ( it != map_addr_vec.end())
    {
        algo_range &cur_range = *it;
        if ( cur_range.len + cur_len > _size )
        {
            algo_range range = cur_range.decrease(_size - cur_len);
            ranges += algo_ranges(range);
            break;
        } else if ( cur_range.len + cur_len == _size )
        {
            ranges += algo_ranges(cur_range);
            map_addr_vec.erase(it);
            break;
        } else
        {
            cur_len += cur_range.len;
            ranges += algo_ranges(cur_range);
            it = map_addr_vec.erase(it);
        }
    }
    left -= _size;
    return ERROR_TYPE_OK;
}

void algo_use_ranges::free_ranges(algo_ranges &ranges)
{
    for ( algo_range_it_t it = ranges.org_addr_vec.begin(); it != ranges.org_addr_vec.end(); ++it )
    {
        algo_range &range = *it;
        insert(range);
    }
    merge();
    left += ranges.length();
}

void algo_use_ranges::increase(unsigned long _add_size)//将自身algo_use_ranges增加
{
    algo_range range(len, len, _add_size);
    insert(range);
    merge();
    len += _add_size;
    left += _add_size;
}

//从empty中扩展某个range的size
int algo_use_ranges::expand_ranges(algo_ranges &ranges, unsigned long _add_size)
{
    algo_ranges new_ranges;
    int err = alloc_ranges(_add_size, new_ranges);
    if ( err )
        return err;
    ranges += new_ranges;
    return ERROR_TYPE_OK;
}

#ifdef TEST_ALGO_RANGE

typedef vector<algo_ranges> algo_ranges_list_t;
typedef algo_ranges_list_t::iterator algo_ranges_it_t;

void dump_range_list(algo_ranges_list_t ranges)
{
    int cnt = 0;
    if(ranges.empty())
    {
        cout<<AWE_PURPLE;
        cout<<"The range list is empty"<<endl;
        cout<<AWE_RESET;
    }
    for(algo_ranges_it_t it = ranges.begin();
        it != ranges.end();
        ++it)
    {
        cout<<"["<<setw(4)<<cnt<<"] ";
        if(++cnt%3 == 0) cout<<AWE_RED;
        else if(cnt%3 == 1)cout<<AWE_YELLOW;
        else cout<<AWE_GREEN;
        it->dump();
        cout<<AWE_RESET;
    }

}

void show_help()
{
    cout<<"---------------help---------------"<<endl;
    cout<<"h                    ;show this"<<endl;
    cout<<"du                   ;dump use ranges"<<endl;
    cout<<"de                   ;dump empty ranges"<<endl;
    cout<<"a <size>             ;alloc ranges"<<endl;
    cout<<"f <use_num>          ;free ranges by num"<<endl;
    cout<<"i <size>             ;increase pool with extra size"<<endl;
    cout<<"e <use_num> <size>   ;expand range with extra size"<<endl;
    cout<<"m <use_num> <addr>   ;show map address of addr"<<endl;
    cout<<"s <use_num> <s> <l>  ;show map section for [start,len]"<<endl;
}

int main(int argc, char const *argv[])
{
    if(argc <2)
    {
        cout<<"please input a number as pool size"<<endl;
        return -1;
    }
    unsigned long size = atoi(argv[1]);
    algo_use_ranges	range_pool(size);
    algo_ranges_list_t ranges_list;
    show_help();
    while(1)
    {
        string input;
        getline(std::cin, input);
        // cout<<input<<endl;
        if(boost::starts_with(input, "du"))
        {
            dump_range_list(ranges_list);
        }
        else if(boost::starts_with(input, "de"))
        {
            cout<<AWE_PURPLE;
            range_pool.dump();
            cout<<AWE_RESET<<endl;
        }
        else if(boost::starts_with(input, "a"))
        {
            vector<string> items;
            boost::split(items, input, boost::is_any_of(" "));
            if(items.size() < 2) continue;
            algo_ranges ranges;
            int err = range_pool.alloc_ranges(atoi(items[1].c_str()),ranges);
            if(err) cout<<"err is "<<error_str(err)<<endl;
            else
            {
                ranges_list.push_back(ranges);
            }
        }
        else if(boost::starts_with(input, "f"))
        {
            vector<string> items;
            boost::split(items, input, boost::is_any_of(" "));
            if(items.size() < 2) continue;
            int num = atoi(items[1].c_str());
            range_pool.free_ranges(ranges_list[num]);
            algo_ranges_it_t it = ranges_list.begin()+num;
            ranges_list.erase(it);

        }
        else if(boost::starts_with(input, "i"))
        {
            vector<string> items;
            boost::split(items, input, boost::is_any_of(" "));
            if(items.size() < 2) continue;
            int size = atoi(items[1].c_str());
            range_pool.increase(size);
        }
        else if(boost::starts_with(input, "e"))
        {
            vector<string> items;
            boost::split(items, input, boost::is_any_of(" "));
            if(items.size() < 3) continue;
            int num = atoi(items[1].c_str());
            int size = atoi(items[2].c_str());
            range_pool.expand_ranges(ranges_list[num],size);
        }
        else if(boost::starts_with(input, "m"))
        {
            vector<string> items;
            boost::split(items, input, boost::is_any_of(" "));
            if(items.size() < 3) continue;
            int num = atoi(items[1].c_str());
            int addr = atoi(items[2].c_str());
            cout<<setw(4)<<addr<<" -> "<<ranges_list[num].map_address(addr)<<endl;
        }
        else if(boost::starts_with(input, "s"))
        {
            vector<string> items;
            boost::split(items, input, boost::is_any_of(" "));
            if(items.size() < 4) continue;
            int num = atoi(items[1].c_str());
            int start = atoi(items[2].c_str());
            int len = atoi(items[3].c_str());
            algo_section section(start,len);
            algo_section_vec_t sections;
            ranges_list[num].map_section(section,sections);
            cout<<AWE_GREEN;
            dump_algo_sections(sections);
            cout<<AWE_RESET;
        }
        else
        {
            show_help();
        }
    }

    return 0;
}

#endif