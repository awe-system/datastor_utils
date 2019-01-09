//
// Created by root on 19-1-9.
//

#include <iostream>
#include "communicate/lt_thread_server.h"

void func1()
{
    std::cout<<"func1\n"<<std::endl;
}

void func2()
{
    std::cout<<"func2\n"<<std::endl;
}

void func3()
{
    std::cout<<"func3\n"<<std::endl;
}

int main(int argc, const char *argv[])
{
    lt_thread_server server(2);
    server.get_io_service()->post(func1);
    
    server.get_io_service()->post(func2);
    
    server.get_io_service()->post(func3);
    
    server.get_io_service()->post(func1);
    
    server.get_io_service()->post(func2);
    
    server.get_io_service()->post(func3);
    
    getchar();
    return 0;
}

