#include "env.h"

#define MAX_SIXE   1024
env::env(const char *repo_name,const char *key)
{
    char tmp_buf[MAX_SIXE] = {0};
    string cmd = "/usr/bin/sudo /usr/bin/serversan_list ";
    cmd += string(repo_name) + string(" ");
    cmd += string(key);
    // cout<<cmd<<endl;

    FILE *pf = popen(cmd.c_str(), "r");//和fopen一样的
    fgets(tmp_buf,MAX_SIXE,pf);
    if(0 == strcmp(tmp_buf,"")) abort();
    // cout<<tmp_buf<<endl;
    val = string(tmp_buf);
    // cout<<val<<endl;
    pclose(pf);
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