#include "ss_log.h"
#include <string>
#include <iostream>
using namespace std;

int main()
{
//    int a = std::to_string(3);
    LOG_DEBUG("test");
    LOG_INFO("test %d", 22);
    std::string a = "sdsd";
    LOG_WARN("test %s", a.c_str());

    int *i = new int(3);

    LOG_ERROR("test %p", i);


//    log4cpp::Category::shutdown();
}
