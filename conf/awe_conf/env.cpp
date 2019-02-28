#include "env.h"

#define MAX_SIXE   1024
env::env(const char *repo_name,const char *key)
{
#ifdef WIN32
#else
    char tmp_buf[MAX_SIXE] = {0};
    string cmd = "/bin/bash /usr/bin/awe_env ";
    cmd += string(repo_name) + string(" ");
    cmd += string(key);
    // cout<<cmd<<endl;

    FILE *pf = popen(cmd.c_str(), "r");//和fopen一样的
    fgets(tmp_buf,MAX_SIXE,pf);
    if(0 == strcmp(tmp_buf,"")) abort();
    string tmp = string(tmp_buf);
    val = tmp.substr(0,tmp.find_last_not_of('\n')+1);
    pclose(pf);
#endif
}

int env::get_int()
{
    return atoi(val.c_str());
}

string env::get_string()
{
    return val;
}

#ifdef ENV_TEST

int main()
{

	env sb_size("ss-metadata","sb_sec_per_disk");
	printf("%d",sb_size.get_int());
}

#endif