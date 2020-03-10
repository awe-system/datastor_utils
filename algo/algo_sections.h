//
// Created by root on 19-3-22.
//

#ifndef DATASTOR_UTILS_ALGO_SECTIONS_H
#define DATASTOR_UTILS_ALGO_SECTIONS_H

namespace ServerSan_Algo
{

/**
 * 为了alloc和free过程不做vector搬移 把小的section放在尾部
 *
 *
 * **/

class algo_sections: public algo_obj
{
private:
    unsigned long len;
public:
    std::vector<algo_section > sections;
public:
    algo_sections():len(0){};
    
    explicit algo_sections(algo_section section);
    
    algo_sections(const algo_sections &other);
    
    explicit algo_sections(unsigned long len);
    
    explicit algo_sections(unsigned long offset, unsigned long len);
    
    void from_json_obj(const json_obj &obj) override;
    
    json_obj to_json_obj() const override;
    
private:
    algo_section &  last_section();
    
    void insert_at_pos(const algo_section & new_section, unsigned int pos);
    
    void erase_at_pos(unsigned int pos);
    
    bool try_merge_left(algo_section &section, unsigned int pos);
    
    bool try_merge_right(algo_section &section, unsigned int pos);
 
    //成功返回pos 在范围内 则is_in_vec=true 没找到则返回
    unsigned int find_pos(unsigned long offset, bool &is_in_vec);
    
    void add_section(const algo_section &section, unsigned int pos);
    
public:
    void intersections(const algo_sections &other, algo_sections &res);
    
    //字节在other中的补集
    void supplesections(const algo_sections &other, algo_sections &res);
    
    bool is_section_insections(const algo_section & section);
    
    bool is_point_insections(unsigned long offset);
    
    void insert_point(unsigned long offset);
    
    void erase_point(unsigned long offset);
   
    algo_sections &operator-=(const algo_section &section);
    
    algo_sections &operator+=(const algo_section &section);
    
    algo_sections &operator-=(const algo_sections &other);
    
    algo_sections &operator+=(const algo_sections &other);
    
    algo_sections &operator+=(unsigned long offset);

public:
    unsigned long alloc_point();
    
    void free_point(unsigned long offset);
    
    algo_sections alloc(unsigned long len);
    
    //范围有交叉时失败
    void free(const algo_sections sections);
    
    unsigned long size() const;
    
    bool is_empty() const;
    
    bool operator==(const algo_sections &other) const;
    
    bool operator!=(const algo_sections &other) const;
};

}
#endif //DATASTOR_UTILS_ALGO_SECTIONS_H
