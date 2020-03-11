#ifndef _UTILS_H_
#define _UTILS_H_
#ifndef WIN32
#include <sys/types.h>

#include <sys/wait.h>


#include <iostream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <assert.h>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;


string int2string(int i, const char* format);
string _str_trim(const string& s);

#define BLANK_CHAR_STR " \a\b\f\n\r\t\v"
void _str_split(vector <string> &v, const string& s, const char* sep = BLANK_CHAR_STR);
int str_array_idx(const string& s, const string array[], int array_size);
int istr_array_idx(const string& s, const string array[], int array_size);
int safe_system_cpp(const string &cmd);
int buf_exec_cpp(const string &cmd, string &buf);
int system_call(const std::string &cmd, std::string &result);


std::string time_to_str(const time_t &t);

///file
int read_data_from_file(const char *file_path, char *buf, int buf_size);
int write_data_to_file(const char *file_path, char *buf, int buf_size);
//read a fullpath file's value:  return: -1 file open error, 0 ok
//NOTE: 1. buf will return NULL terminal.  2.strip \n
int read_str_from_file(const char *file_path, char *buf, int buf_size);
int read_val_from_file(const char *file_path);

int write_val2file(const char *file_path, int val);
int write_str2file(const char *file_path, const char * str);
int echo_str2file(const char *file_path, const char * str);

int read_dirnames(const char *dir_path, vector<string> &dir_list);

///check ip
bool ip_valid(const char *ip);

///string convert
template <class T>
void ConvertFromString(T &value, const string &s) {
  stringstream ss(s);
  ss >> value;
}

template <class T>
string ConvertToString(T value) {
  stringstream ss;
  ss << value;
  return ss.str();
}

///map
template <typename TMap, typename T>
        T* map_get(TMap &map, const char *user_id)
{
    typename TMap::iterator it = map.find(user_id);
    if (it == map.end())
        return 0;
    else
        return it->second;
}

template <typename TMap, typename T>
    int map_add(TMap &map, const char *user_id, T* pt)
{
    typename TMap::iterator it = map.find(user_id);
    if (it == map.end())
    {
        string str_id(user_id);
    map[str_id] = pt;
    return 0;
    }
    return -1;
}

template <typename TMap>
        int map_del(TMap &map, const char *user_id)
{
    typename TMap::iterator it = map.find(user_id);
    if (it != map.end() )
    {
        delete it->second;
    map.erase(it);
    return 0;
    }
    return -1;
}

template <typename TMap>
        void map_clean(TMap &map)
{
    for (typename TMap::iterator it = map.begin(); it != map.end(); ++it)
        delete it->second;
    map.clear();
}
#endif
#endif
