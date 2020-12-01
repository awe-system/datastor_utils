#ifndef _ENV_H
#define _ENV_H

#include <cstring>
#include <iostream>

using namespace std;
class env{
    string val;
public:
    env(const char *repo_name,const char *key);
    int get_int();
    string get_string();
};

bool is_conf_different(const char *repo_name, const char *md5sum);


#endif