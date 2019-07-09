#ifndef TEST_COMMUNICATE_LT_REFERENCE_H
#define TEST_COMMUNICATE_LT_REFERENCE_H


#include <atomic>

class lt_reference
{
private:
   int ref;
public:
    explicit lt_reference();
    virtual ~lt_reference();
    
    bool get_check_first();

    lt_reference* get();

    bool put();
};


#endif //TEST_COMMUNICATE_LT_REFERENCE_H
