#ifndef JSON_OBJ_H
#define JSON_OBJ_H

#include <map>
#include <vector>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <boost/function.hpp>

using namespace std;

class json_obj;

typedef std::map <json_obj, json_obj> json_dic;
typedef std::pair <json_obj, json_obj> json_pair;
typedef json_dic::iterator json_dic_it;
typedef json_dic::const_iterator json_dic_const_it;
typedef std::vector <json_obj> json_array;
typedef json_array::const_iterator json_array_const_it;

enum json_obj_type
{
    json_obj_type_string,
    json_obj_type_int,
    json_obj_type_llong,
    json_obj_type_bool,
    json_obj_type_dic,
    json_obj_type_array,
    json_obj_type_unknown,
};

class json_obj
{
private:
    json_obj_type _type;
public:
    std::string s_val;
    long long ll_val;
    int i_val;
    bool b_val;
    json_dic dic_val;
    json_array array_val;
private:
    bool are_types_same(json_obj_type type_a, json_obj_type type_b) const;
private:
    void update_type_by_doc(const rapidjson::Document &doc);

    void update_type_by_val(const rapidjson::Value &val);

private:
    bool compare_array(const json_obj &other) const;

    bool compare_dic(const json_obj &other) const;

public:
    bool is_empty() const;

    void dumps(rapidjson::Document &doc, rapidjson::Value &val) const;

    void dump_string(rapidjson::Document &doc) const;

    void dump_int(rapidjson::Document &doc) const;

    void dump_llong(rapidjson::Document &doc) const;

    void dump_bool(rapidjson::Document &doc) const;

    void dump_dic(rapidjson::Document &doc) const;

    void dump_array(rapidjson::Document &doc) const;

    void dump_string(rapidjson::Document &doc, rapidjson::Value &val) const;

    void dump_int(rapidjson::Document &doc, rapidjson::Value &val) const;

    void dump_llong(rapidjson::Document &doc, rapidjson::Value &val) const;

    void dump_bool(rapidjson::Document &doc, rapidjson::Value &val) const;

    void dump_dic(rapidjson::Document &doc, rapidjson::Value &val) const;

    void dump_array(rapidjson::Document &doc, rapidjson::Value &val) const;

private:
    void dumps(rapidjson::Document &doc) const;

private:
    void load_dic(const rapidjson::Value &val);

    void load_array(const rapidjson::Value &val);

    void load_int(const rapidjson::Value &val);

    void load_llong(const rapidjson::Value &val);

    void load_bool(const rapidjson::Value &val);

    void load_string(const rapidjson::Value &val);

    void load_dic(const rapidjson::Document &doc);

    void load_array(const rapidjson::Document &doc);

    void load_int(const rapidjson::Document &doc);

    void load_llong(const rapidjson::Document &doc);

    void load_bool(const rapidjson::Document &doc);

    void load_string(const rapidjson::Document &doc);

public:
    json_obj(const char *val);

    json_obj(const rapidjson::Value &val);

    json_obj(const std::string &val);

    json_obj(const int val);

    json_obj(const long long val);

    json_obj(const bool val);

    json_obj(const json_obj &key, const json_obj &value);

    json_obj(const char *key, const json_obj &value);

    json_obj(const string &key, const json_obj &value);

    json_obj(const char *key, const char *value);

    json_obj(const char *key, const string &value);

    json_obj(const string &key, const char *value);

    json_obj(const string &key, const string &value);

    json_obj(const char *key, const int value);

    json_obj(const char *key, const long long value);

    json_obj(const string &key, const int value);

    json_obj(const string &key, const long long value);

    json_obj(const char *key, const bool value);

    json_obj(const string &key, const bool value);

    // json_obj(json_dic &dic);
    // json_obj(json_array &array);

    json_obj();

    json_obj(const json_obj &other);

    void clear();

    bool operator<(const json_obj &other) const;


    bool operator!=(const json_obj &other) const;

    bool operator==(const json_obj &other) const;

    json_obj &operator[](const char *key);

    json_obj &operator[](const json_obj &key);

    json_obj &operator[](const string &key);

    json_obj &operator[](const int &k);


    const json_obj &operator[](const char *key)const;

    const json_obj &operator[](const json_obj &key)const;

    const json_obj &operator[](const string &key)const;

    const json_obj &operator[](const int &k)const;

    json_obj_type type() const;

    void set_dic();

    void set_array();

    int size()const;

    void append(const char *val);

    void append(const int val);

    void append(const long long val);

    void append(const string &val);

    void append(bool val);

    void append(const json_obj &other);

    void merge(const json_obj &other);

    void loads(std::string &str);

    void loads(const char *s);

    std::string dumps() const;

    void traversal_array_const(boost::function<void (const json_obj &)> f) const;

    void traversal_array(boost::function<void (json_obj &)> f);

    void traversal_dic(boost::function<void (const json_obj &,json_obj &)> f);

    void traversal_dic_const(boost::function<void (const json_obj &,const json_obj &)> f) const;

    long long get_number() const;
};

#endif