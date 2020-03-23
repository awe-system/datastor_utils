//
// Created by root on 19-3-27.
//

#ifndef DATASTOR_UTILS_LT_STACK_H
#define DATASTOR_UTILS_LT_STACK_H

#include <vector>
#include <mutex>

class lt_stack
{
    void                *private_data = nullptr;
    std::vector<void *> data_stack;
    std::mutex          m;
public:
    lt_stack():private_data(nullptr){}
    
    lt_stack(const lt_stack &other):
            private_data(other.private_data),data_stack(other.data_stack)
    {
    }
    
    void push_private(void *new_private)
    {
        std::unique_lock<std::mutex> lck(m);
        data_stack.push_back(private_data);
        private_data = new_private;
    }
    
    void *pop_private()
    {
        std::unique_lock<std::mutex> lck(m);
        void *cur_private = private_data;
        if ( data_stack.size() != 0 )
        {
            private_data = data_stack.back();
            data_stack.pop_back();
        }
        return cur_private;
    }
    
    void *top_private()
    {
        return private_data;
    }
};
#endif //DATASTOR_UTILS_LT_STACK_H
