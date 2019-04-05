//
// Created by root on 19-3-22.
//
#include "../../algo/algo.h"
#include "../../algo/debug.h"

//static int case_xxx()
//{
//    try
//    {
//        dbg << "case_xxx " << color_green << "ok" << end_dbg;
//    }
//    catch (...)
//    {
//        dbg << "case_xxx " << color_red << "red" << end_dbg;
//        return -1;
//    }
//    return 0;
//}


int case_plus_minus()
{
    try
    {
        ServerSan_Algo::algo_sections sections(50);
        sections += ServerSan_Algo::algo_section(70, 20);
        sections += ServerSan_Algo::algo_sections(50,20);
        assert(sections.sections[0].start == 0);
        assert(sections.sections[0].len == 90);
        assert(sections.size() == 90);
        sections -= ServerSan_Algo::algo_sections(60,20);
        sections -= ServerSan_Algo::algo_sections(ServerSan_Algo::algo_section(0,60));
        sections -= ServerSan_Algo::algo_sections(80,10);
        assert(sections.size() == 0);
        assert(sections.is_empty());
        dbg << "case_plus_minus " << color_green << "ok" << end_dbg;
    }
    catch (...)
    {
        dbg << "case_plus_minus " << color_red << "red" << end_dbg;
        return -1;
    }
    return 0;
}

int case_plus_minus_intersection()
{
    try
    {
        ServerSan_Algo::algo_sections sections(50);
        sections += ServerSan_Algo::algo_section(70, 20);
        sections += ServerSan_Algo::algo_sections(50,20);
        assert(sections.sections[0].start == 0);
        assert(sections.sections[0].len == 90);
        assert(sections.size() == 90);
        sections -= ServerSan_Algo::algo_sections(60,20);
        sections -= ServerSan_Algo::algo_sections(ServerSan_Algo::algo_section(0,70));
        sections -= ServerSan_Algo::algo_sections(70,20);
        assert(sections.size() == 0);
        assert(sections.is_empty());
        dbg << "case_plus_minus_intersection " << color_green << "ok" << end_dbg;
    }
    catch (...)
    {
        dbg << "case_plus_minus_intersection " << color_red << "red" << end_dbg;
        return -1;
    }
    return 0;
}

int case_plus_minus_special()
{
    try
    {
        ServerSan_Algo::algo_sections sections(4);
        ServerSan_Algo::algo_sections sections_minus(0,1);
        ServerSan_Algo::algo_sections sections2_2(2,2);
        sections_minus += sections2_2;
        assert(sections_minus.size() == 3);
        sections -= sections_minus;
        assert(sections.size() == 1);
        assert(sections.alloc_point() == 1);
        assert(sections.size() == 0);
        dbg << "case_plus_minus_special " << color_green << "ok" << end_dbg;
    }
    catch (...)
    {
        dbg << "case_plus_minus_special " << color_red << "red" << end_dbg;
        return -1;
    }
    return 0;
}

void swap(unsigned long arr[], int a, int b)
{
    unsigned long tmp = arr[a];
    arr[a] = arr[b];
    arr[b] = tmp;
}

void random_arr(unsigned long arr[], int arr_size)
{
    for ( int i = 0; i < arr_size * 2; ++i )
    {
        int a = rand() % arr_size;
        int b = rand() % arr_size;
        swap(arr, a, b);
    }
}

bool is_point_diff(unsigned long arr[], int arr_size, unsigned long new_point)
{
    for ( int i = 0; i < arr_size; ++i )
    {
        if ( arr[i] == new_point )
        { return false; }
    }
    return true;
}

int case_alloc_insert_erase()
{
    try
    {
        ServerSan_Algo::algo_sections sections(50);
        unsigned long                 points[50];
        
        for ( int                     i = 0; i < 50; ++i )
        {
            points[i] = sections.alloc_point();
            assert(points[i] < 50);
            assert(is_point_diff(points, i, points[i]));
        }
        assert(sections.size() == 0 && sections.is_empty());
        assert(sections.size() == 0 && sections.is_empty());
        random_arr(points, 50);
        for ( int i = 0; i < 50; ++i )
        {
            assert(!sections.is_point_insections(points[i]));
        }
        for ( int i = 0; i < 50; ++i )
        {
            sections.insert_point(points[i]);
        }
        assert(sections.size() == 50);
        for ( int i = 0; i < 50; ++i )
        {
            assert(sections.is_point_insections(points[i]));
        }
        random_arr(points, 50);
        for ( int i = 0; i < 50; ++i )
        {
            sections.erase_point(points[i]);
        }
        assert(sections.size() == 0 && sections.is_empty());
        for ( int i = 0; i < 50; ++i )
        {
            assert(!sections.is_point_insections(points[i]));
        }
        dbg << "case_alloc_insert_erase " << color_green << "ok" << end_dbg;
    }
    catch (...)
    {
        dbg << "case_alloc_insert_erase " << color_red << "red" << end_dbg;
        return -1;
    }
    return 0;
}


int case_alloc_free_point()
{
    try
    {
        ServerSan_Algo::algo_sections sections(50);
        unsigned long                 points[50];
        for ( int                     i = 0; i < 50; ++i )
        {
            points[i] = sections.alloc_point();
            assert(points[i] < 50);
            assert(is_point_diff(points, i, points[i]));
        }
        assert(sections.size() == 0 && sections.is_empty());
        random_arr(points, 50);
        
        for ( int i = 0; i < 25; ++i )
        {
            sections.free_point(points[i]);
        }
        assert(sections.size() == 25);
        for ( int i = 0; i < 25; ++i )
        {
            points[i] = sections.alloc_point();
            assert(points[i] < 50);
            assert(is_point_diff(points, i, points[i]));
        }
        assert(sections.size() == 0 && sections.is_empty());
        random_arr(points, 50);
        for ( int i = 0; i < 50; ++i )
        {
            sections.free_point(points[i]);
        }
        assert(sections.size() == 50 && sections.sections.size() == 1);
        
        dbg << "case_alloc_free_point " << color_green << "ok" << end_dbg;
    }
    catch (...)
    {
        dbg << "case_alloc_free_point " << color_red << "red" << end_dbg;
        return -1;
    }
    return 0;
}

int case_alloc_free()
{
    try
    {
        ServerSan_Algo::algo_sections sections(50);
        ServerSan_Algo::algo_sections new_sections = sections.alloc(20);
        assert(sections.size() == 30);
        assert(new_sections.size() == 20);
        sections.free(new_sections);
        assert(sections.size() == 50);
        new_sections = sections.alloc(80);
        assert(new_sections.size() == 0 && new_sections.is_empty());
        ServerSan_Algo::algo_sections sections_0_9 = sections.alloc(10);
        assert(sections_0_9.size() == 10);
        ServerSan_Algo::algo_sections sections_10_19 = sections.alloc(10);
        assert(sections_10_19.size() == 10);
        ServerSan_Algo::algo_sections sections_20_29 = sections.alloc(10);
        assert(sections_20_29.size() == 10);
        ServerSan_Algo::algo_sections sections_30_39 = sections.alloc(10);
        assert(sections_30_39.size() == 10);
        ServerSan_Algo::algo_sections sections_40_49 = sections.alloc(10);
        assert(sections_40_49.size() == 10);
        assert(sections.is_empty());
        sections.free(sections_10_19);
        assert(sections.size() == 10);
        sections.free(sections_30_39);
        assert(sections.size() == 20);
        ServerSan_Algo::algo_sections sections_10_19_2 = sections.alloc(10);
        assert(sections_10_19_2.size() == 10 &&
               sections_10_19_2.sections[0].start ==
               sections_10_19.sections[0].start);
        ServerSan_Algo::algo_sections sections_30_39_2 = sections.alloc(10);
        assert(sections_30_39_2.size() == 10);
        assert(sections.size() == 0 && sections.is_empty());
        sections.free(sections_10_19_2);
        sections.free(sections_30_39_2);
        ServerSan_Algo::algo_sections sections_20 = sections.alloc(20);
        assert(sections.size() == 0 && sections.is_empty());
        assert(sections_20.size() == 20);
        assert(sections_20.sections[0].start ==
               sections_30_39_2.sections[0].start);
        assert(sections_20.sections[0].len == sections_30_39_2.sections[0].len);
        assert(sections_20.sections[1].start ==
               sections_10_19_2.sections[0].start);
        assert(sections_20.sections[1].len == sections_10_19_2.sections[0].len);
        
        dbg << "case_alloc_free " << color_green << "ok" << end_dbg;
    }
    catch (...)
    {
        dbg << "case_alloc_free " << color_red << "red" << end_dbg;
        return -1;
    }
    return 0;
}


int case_size()
{
    try
    {
        int                           size = 33;
        ServerSan_Algo::algo_sections sections(size);
        if ( sections.size() != size )
        { throw 1; }
        dbg << "case_size " << color_green << "ok" << end_dbg;
    }
    catch (...)
    {
        dbg << "case_size " << color_red << "red" << end_dbg;
        return -1;
    }
    return 0;
}

int case_empty()
{
    try
    {
        ServerSan_Algo::algo_sections sections(0);
        if ( !sections.is_empty() )
        { throw 1; }
        
        dbg << "case_empty " << color_green << "ok" << end_dbg;
    }
    catch (...)
    {
        dbg << "case_empty " << color_red << "red" << end_dbg;
        return -1;
    }
    return 0;
}

int main(int argc, const char *argv[])
{
    sleep(1);
    assert(0 == case_empty());
    
    assert(0 == case_size());
    
    assert(0 == case_alloc_free());
    
    assert(0 == case_alloc_free_point());
    
    assert(0 == case_alloc_insert_erase());
    
    assert(0 == case_plus_minus());
    
    assert(0 == case_plus_minus_intersection());
    
    assert(0 == case_plus_minus_special());
    
    dbg << color_yellow << "\nALL test " << color_green << "[OK]" << end_dbg;
}

