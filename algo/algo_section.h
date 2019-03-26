#ifndef _ALGO_SECTION_H
#define _ALGO_SECTION_H

namespace ServerSan_Algo
{
class algo_section;
typedef vector<algo_section> algo_section_vec_t;
typedef vector<algo_section>::iterator algo_section_vec_it;
typedef vector<algo_section>::const_iterator algo_section_const_it;

class algo_section :algo_obj//与一个io对应的
{
public:
    unsigned long start;
    unsigned long len;
public:
    algo_section();

public:
    void from_json_obj(const json_obj &obj) override;

    json_obj to_json_obj() const override;

public:
    
    algo_section(request_t *request);

    algo_section(unsigned long _start, unsigned long _len);

    algo_section(const algo_section &other);

    void dump() const;
    
    bool is_neighbour(const algo_section &other);
    
    void merge(const algo_section &other);
    
    //区域不重叠且other在this的右侧
    bool operator<(const algo_section& other)const;
    //区域不重叠且other在this的左侧
    bool operator>(const algo_section& other)const;
    
    bool is_is_point_insections(unsigned long offset);
    
    algo_section alloc(unsigned long len);
    
    //交集
    void intersection(const algo_section& other, algo_section &res);
    //从small的把section分割开来
    void split(const algo_section& small_sec, algo_section_vec_t & sections) const;
};

void dump_algo_sections(const algo_section_vec_t &sections);

};
#endif