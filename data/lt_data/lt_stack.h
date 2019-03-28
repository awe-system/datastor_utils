//
// Created by root on 19-3-27.
//

#ifndef DATASTOR_UTILS_LT_STACK_H
#define DATASTOR_UTILS_LT_STACK_H

#include <vector>

class lt_stack
{
    void                *private_data = nullptr;
    std::vector<void *> data_stack;
public:
    void push_private(void *new_private)
    {
        data_stack.push_back(private_data);
        private_data = new_private;
    }
    
    void *pop_private()
    {
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
