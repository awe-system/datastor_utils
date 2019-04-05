//
// Created by root on 19-3-22.
//

#include "algo.h"
#include "algo_sections.h"
#include "debug.h"


namespace ServerSan_Algo
{


unsigned int algo_sections::find_pos(unsigned long offset, bool &is_in_vec)
{//NOTE:大数在前小数在后
    unsigned int cur_pos = 0;
    if ( 0 == sections.size() )
    {
        is_in_vec = false;
        return cur_pos;
    }
    if ( sections[sections.size() - 1].start > offset )
    {
        is_in_vec = false;
        return sections.size();
    }
    
    unsigned int start = 0;
    unsigned int end   = static_cast<unsigned int>(sections.size() - 1);
    cur_pos = (start + end) / 2;
    do
    {
        if ( sections[cur_pos].is_point_insections(offset) )
        {
            is_in_vec = true;
            return cur_pos;
        }
        else if ( start == cur_pos && cur_pos == end )
        {
            is_in_vec = false;
            return cur_pos;
        }
        else if ( sections[cur_pos].start > offset )
        {//向右侧移动
            start = cur_pos + 1;
        }
        else if ( sections[cur_pos].start + sections[cur_pos].len <= offset )
        {//向左侧移动
            end = cur_pos;
        }
        else
        {
            abort();
        }
        cur_pos = (start + end) / 2;
    } while ( 1 );
}

bool algo_sections::is_point_insections(unsigned long offset)
{
    bool         is_in_vec;
    unsigned int pos = find_pos(offset, is_in_vec);
    return is_in_vec;
}

algo_sections &algo_sections::operator-=(const algo_section &section)
{
    bool         is_in_vec;
    unsigned int pos = find_pos(section.start, is_in_vec);
    assert(is_in_vec);
    algo_section_vec_t sec_parts;
    sections[pos].split(section, sec_parts);
    assert(sec_parts.size() <= 2);
    if ( 0 == sec_parts.size() )
    {
        erase_at_pos(pos);
    }
    else if ( 1 == sec_parts.size() )
    {
        sections[pos] = sec_parts[0];
    }
    else if ( 2 == sec_parts.size() )
    {
        sections[pos] = sec_parts[0];
        insert_at_pos(sec_parts[1], pos);
    }
    len -= section.len;
    return *this;
}

algo_sections &algo_sections::operator+=(const algo_section &section)
{
    bool         is_in_vec;
    unsigned int pos = find_pos(section.start, is_in_vec);
    assert(!is_in_vec);
    algo_section tmp_section = section;
    int          merge_num   = 0;
    
    if ( try_merge_right(tmp_section, pos) )
    {
        ++merge_num;
    }
    if ( try_merge_left(tmp_section, pos) )
    {//NOTE:顺序不能颠倒 因为如果两个都merge了会把right的数据吞掉 所以left中的数据必须最后合并的
        ++merge_num;
    }
    
    if ( 0 == merge_num )
    {
        insert_at_pos(tmp_section, pos);
    }
    else if ( 2 == merge_num )
    {
        erase_at_pos(pos);
    }
    len += section.len;
}

algo_sections &algo_sections::operator-=(const algo_sections &other)
{
    algo_sections tmp_sections;
    intersections(other, tmp_sections);
    for ( auto it : tmp_sections.sections )
    {//FIXME: erase 可以优化
        *this -= (it);
    }
    return *this;
}

algo_sections &algo_sections::operator+=(const algo_sections &other)
{
    for ( auto it : other.sections )
    {
        *this += (it);
    }
    return *this;
}

algo_sections algo_sections::alloc(unsigned long _len)
{
    algo_sections new_sections(0);
    if ( _len > len )
    {
        return new_sections;
    }
    
    unsigned long cur_len = 0;
    while ( cur_len < _len )
    {
        algo_section  &section    = last_section();
        unsigned long this_len    = min(_len - cur_len, section.len);
        algo_section  cur_section = section.alloc(this_len);
        if ( 0 == section.len )
        {
            sections.pop_back();
        }
        new_sections += cur_section;
        cur_len += this_len;
        len -= this_len;
    }
    return new_sections;
}

void algo_sections::free(const algo_sections _sections)
{
    *this += _sections;
}

void algo_sections::free_point(unsigned long offset)
{
    algo_sections free_sec(offset, 1);
    this->free(free_sec);
}

unsigned long algo_sections::alloc_point()
{
    assert(len > 0);
    algo_sections res = alloc(1);
    assert(res.len == 1);
    return res.sections[0].start;
}

algo_sections::algo_sections(algo_section section)
{
    sections.push_back(section);
    len = section.len;
}

algo_sections::algo_sections(const algo_sections &other) :
        len(other.len), sections(other.sections)
{
}

algo_sections::algo_sections(unsigned long _len)
{
    len = _len;
    if ( len )
    {
        algo_section section(0, len);
        sections.push_back(section);
    }
}

algo_sections::algo_sections(unsigned long offset, unsigned long _len) :
        len(_len)
{
    algo_section section(offset, len);
    sections.push_back(section);
}

unsigned long algo_sections::size() const
{
    return len;
}

bool algo_sections::is_empty() const
{
    return 0 == len;
}

algo_section &algo_sections::last_section()
{
    assert(!is_empty());
    return sections[sections.size() - 1];
}

void algo_sections::insert_point(unsigned long offset)
{
    free_point(offset);
}

void algo_sections::erase_point(unsigned long offset)
{
    *this -= algo_section(offset, 1);
}

void
algo_sections::insert_at_pos(const algo_section &new_section, unsigned int pos)
{
    sections.insert(sections.begin() + pos, new_section);
}

void algo_sections::erase_at_pos(unsigned int pos)
{
    sections.erase(sections.begin() + pos);
}

bool algo_sections::try_merge_left(algo_section &section, unsigned int pos)
{
    if ( 0 == pos )
    {//没有左侧端点值
        return false;
    }
    
    if ( section.is_neighbour(sections[pos - 1]) )
    {
        section.merge(sections[pos - 1]);
        sections[pos - 1] = section;
        return true;
    }
    return false;
}

bool algo_sections::try_merge_right(algo_section &section, unsigned int pos)
{
    if ( pos == sections.size() )
    {//没有右侧端点值
        return false;
    }
    
    if ( section.is_neighbour(sections[pos]) )
    {
        section.merge(sections[pos]);
        sections[pos] = section;
        return true;
    }
    return false;
}

void algo_sections::intersections(const algo_sections &other,
                                  algo_sections &res)
{
    int i = 0, j = 0;
    while(i<other.sections.size() && j < sections.size())
    {
        const algo_section &cur_other = other.sections[i];
        const algo_section &cur_self = sections[j];   //other的当前比 section的大
        if (cur_other < cur_self)
        {
            ++j;
        }
        else if (cur_other > cur_self)
        {
            ++i;
        }
        else
        {
            algo_section tmp_section = cur_self;
            algo_section intersection;
            tmp_section.intersection(cur_other, intersection);
            res += intersection;
            if(cur_other.start< cur_self.start)
            {
                ++j;
            }
            else if(cur_other.start > cur_self.start )
            {
                ++i;
            }
            else
            {
                ++i;
                ++j;
            }
        }
    }
}

}