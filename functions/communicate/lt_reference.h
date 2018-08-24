#ifndef TEST_COMMUNICATE_LT_REFERENCE_H
#define TEST_COMMUNICATE_LT_REFERENCE_H


#include <atomic>

class lt_reference
{
private:
    std::atomic<int> ref;
public:
    explicit lt_reference();

    lt_reference* get();

    bool put();
};


#endif //TEST_COMMUNICATE_LT_REFERENCE_H
