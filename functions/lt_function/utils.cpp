#ifndef WIN32
#include <assert.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <errno.h>
#include <dirent.h>


#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <map>

#include "utils.h"
#include <boost/asio.hpp>

//utf8标记

string int2string(int i, const char* format)
{
    char c[16];
    snprintf(c, 16, format, i);
    return c;
}

string _str_trim(const string& s)
{
    return trim_copy_if(s, is_space() || !is_print());
}

void _str_split(vector <string> &v, const string& s, const char* sep)
{
    string t = _str_trim(s);
    split(v, t, is_any_of(sep), token_compress_on);
}

int str_array_idx(const string& s, const string array[], int array_size)
{
    for(int i=0; i<array_size; ++i)
    {
        if(equals(s, array[i])) return i;
    }
    return -1;
}

int istr_array_idx(const string& s, const string array[], int array_size)
{
    for(int i=0; i<array_size; ++i)
    {
        if(iequals(s, array[i])) return i;
    }
    return -1;
}

int safe_system_cpp(const string &cmd)
{
#define SHELL "/bin/sh"
    pid_t pid;
    int state = -1;

    cerr << "debug safe_system_cpp(): cmd=[" << cmd << "]" << endl;

    pid = fork();
    if (pid < 0)
        return -1;
    if (pid == 0) {
        execl(SHELL, "sh", "-c", cmd.c_str(), NULL);
        exit(127);
    }
    wait(&state);

    return state;
}

int buf_exec_cpp(const string &cmd, string &buf)
{
    FILE *fp;
    int readed = 0;

//    cerr << "debug buf_exec_cpp(): cmd=[" << cmd << "]" << endl;

    buf.clear();
    fp = popen(cmd.c_str(), "r");
    if(fp == NULL)
        return -1;

    while (true)
    {
        char tmp[512];
        int n = fread(tmp, 1, 512, fp);
        if (n == 0) break;

        buf.append(tmp, n);
    }
    return (pclose(fp));
}

#define  MAX_RESULT_LEN 101

int system_call(const std::string &cmd, std::string &result)
{
    result = "";
    FILE *fp = popen(cmd.c_str(), "r");
    if ( fp == NULL )
        return -1;

    char buf[MAX_RESULT_LEN];

    while ( 1 )
    {
        memset(buf, 0, MAX_RESULT_LEN);
        size_t read_num = fread(buf, 1, MAX_RESULT_LEN - 1, fp);
        if ( read_num == 0 )
        {
            break;
        }
        result.append(buf);
    }
    pclose(fp);

    boost::trim(result);
    return 0;
}

int read_data_from_file(const char *file_path, char *buf, int buf_size)
{
    memset(buf, 0, buf_size);
    ifstream ifs(file_path, ios::binary);
    if (!ifs.is_open() )
        return -1;
    ifs.read(buf, buf_size);
    ifs.close();
    return 0;
}

int read_str_from_file(const char *file_path, char *buf, int buf_size)
{
    memset(buf, 0, buf_size);
    ifstream ifs(file_path, ios::binary);
    if (!ifs.is_open() )
        return -1;
    ifs.read(buf, buf_size);
    ifs.close();

    //some with \n[key] pending, remove it.
    char *p = strchr(buf, '\n');
    if (p)
        *p = 0;
    return 0;
}

int read_val_from_file(const char *file_path)
{
    int buf_size = 128;
    char buf[buf_size];
    memset(buf, 0, buf_size);

    read_str_from_file(file_path, buf, buf_size);
    return atoi(buf);
}

static inline int _write_str2file(const char *file_path, const char *str)
{
    ofstream ofs(file_path, ios::binary);
    if (!ofs.is_open() )
        return -1;
    ofs.write(str, strlen(str));
    int ret = 0;
    if ( ! ofs.good() )
        ret = -1;
    //printf("ret=%d %s %s\n", ret, file_path, str);
    ofs.close();
    return ret;
}

int write_val2file(const char *file_path, int val)
{
    char buf[20];
    sprintf(buf, "%d", val);
    return _write_str2file(file_path, buf);
}

int write_str2file(const char *file_path, const char *str)
{
    return _write_str2file(file_path, str);
}

int echo_str2file(const char *file_path, const char *str)
{
    char cmd_line[512];
    sprintf(cmd_line, "echo \"%s\" >%s", str, file_path );
    int ret= safe_system_cpp( string(cmd_line) );
    if (ret != 0)
        return -1;
    return 0;
}

int read_dirnames(const char *dir_path, vector<string> &dir_list)
{
    DIR *dp;
    struct dirent *dirp;
    dir_list.clear();

    if ((dp = opendir(dir_path)) == 0)
    {
        //printf("can't open %s\n", dir_path);
        return -1;
    }
    while ((dirp = readdir(dp)) != 0)
    {
        if (dirp->d_type == DT_DIR && dirp->d_name[0] != '.' )
            dir_list.push_back(dirp->d_name);
    }

    closedir(dp);
    return 0;
}

bool ip_valid(const char *ip)
{
    boost::system::error_code ec;
    boost::asio::ip::address::from_string(ip, ec);
    if (ec)
        return false;
    return true;
}
#endif