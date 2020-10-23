#include "json_obj.h"// #include <Python.h>
#include <stdio.h>
#include <lt_function/utils.h>
#include "lt_data_error.h"
#include "boost/bind.hpp"
#ifdef JSON_TEST
#include <iostream>
#endif

bool json_obj::is_empty() const
{
    return json_obj_type_unknown == _type;
}

json_obj::json_obj(const char *val) :
        _type((json_obj_type) json_obj_type_string),
        s_val(val)
{
}

json_obj::json_obj(const std::string &val) :
        _type(json_obj_type_string),
        s_val(val)
{
}

json_obj::json_obj(const int val) :
        _type(json_obj_type_int),
        i_val(val)
{
}


json_obj::json_obj(const long long val) :
        _type(json_obj_type_llong),
        ll_val(val)
{
}

json_obj::json_obj(const bool val) :
        _type(json_obj_type_bool),
        b_val(val)
{
}

json_obj::json_obj(const json_obj &key, const json_obj &value) :
        _type(json_obj_type_dic)
{
    dic_val.insert(json_pair(key, value));
}


json_obj::json_obj(const char *key, const json_obj &value) :
        _type(json_obj_type_dic)
{
    json_obj obj_key(key);
    dic_val.insert(json_pair(obj_key, value));
}

json_obj::json_obj(const string &key, const json_obj &value) :
        _type(json_obj_type_dic)
{
    json_obj obj_key(key);
    dic_val.insert(json_pair(obj_key, value));
}

json_obj::json_obj(const char *key, const char *value) :
        _type(json_obj_type_dic)
{
    json_obj obj_key(key);
    json_obj obj_value(value);
    dic_val.insert(json_pair(obj_key, obj_value));
}

json_obj::json_obj(const char *key, const string &value) :
        _type(json_obj_type_dic)
{
    json_obj obj_key(key);
    json_obj obj_value(value);
    dic_val.insert(json_pair(obj_key, obj_value));
}

json_obj::json_obj(const string &key, const char *value) :
        _type(json_obj_type_dic)
{
    json_obj obj_key(key);
    json_obj obj_value(value);
    dic_val.insert(json_pair(obj_key, obj_value));
}

json_obj::json_obj(const string &key, const string &value) :
        _type(json_obj_type_dic)
{
    json_obj obj_key(key);
    json_obj obj_value(value);
    dic_val.insert(json_pair(obj_key, obj_value));
}

json_obj::json_obj(const char *key, const int value) :
        _type(json_obj_type_dic)
{
    json_obj obj_key(key);
    json_obj obj_value(value);
    dic_val.insert(json_pair(obj_key, obj_value));
}

json_obj::json_obj(const char *key, const long long value) :
        _type(json_obj_type_dic)
{
    json_obj obj_key(key);
    json_obj obj_value(value);
    dic_val.insert(json_pair(obj_key, obj_value));
}

json_obj::json_obj(const string &key, const int value) :
        _type(json_obj_type_dic)
{
    json_obj obj_key(key);
    json_obj obj_value(value);
    dic_val.insert(json_pair(obj_key, obj_value));
}

json_obj::json_obj(const string &key, const long long value) :
        _type(json_obj_type_dic)
{
    json_obj obj_key(key);
    json_obj obj_value(value);
    dic_val.insert(json_pair(obj_key, obj_value));
}

json_obj::json_obj(const char *key, const bool value) :
        _type(json_obj_type_dic)
{
    json_obj obj_key(key);
    json_obj obj_value(value);
    dic_val.insert(json_pair(obj_key, obj_value));
}

json_obj::json_obj(const string &key, const bool value) :
        _type(json_obj_type_dic)
{
    json_obj obj_key(key);
    json_obj obj_value(value);
    dic_val.insert(json_pair(obj_key, obj_value));
}

json_obj::json_obj() :
        _type(json_obj_type_unknown)
{
}

json_obj::json_obj(const json_obj &other)
{
    clear();
    _type = other._type;
    switch ( _type )
    {
        case json_obj_type_array:
        case json_obj_type_dic:
            merge(other);
            break;
        case json_obj_type_string:
            s_val = other.s_val;
            break;
        case json_obj_type_int:
            i_val = other.i_val;
            break;
        case json_obj_type_llong:
            ll_val = other.ll_val;
            break;
        case json_obj_type_bool:
            b_val = other.b_val;
            break;
        default:
            break;
    }
}

void json_obj::clear()
{
    _type = json_obj_type_unknown;
    dic_val.clear();
    array_val.clear();
}

bool json_obj::operator!=(const json_obj &other) const
{
    return !(*this == other);
}


bool json_obj::compare_array(const json_obj &other) const
{
    if ( array_val.size() != other.array_val.size())
        return false;
    for ( json_array_const_it it = array_val.begin(), o_it = other.array_val.begin();
          it != array_val.end();
          ++it, ++o_it )
    {
        if ( *it != *o_it )
            return false;
    }
    return true;
}

bool json_obj::compare_dic(const json_obj &other) const
{
    if ( dic_val.size() != other.dic_val.size())
        return false;
    for ( json_dic_const_it it = dic_val.begin(),
                  o_it = other.dic_val.begin();
          it != dic_val.end();
          ++it, ++o_it )
    {
        const json_obj &key = it->first;
        const json_obj &o_key = o_it->first;
        const json_obj &value = it->second;
        const json_obj &o_value = o_it->second;
        if ( key != o_key )
            return false;
        if ( value != o_value )
            return false;
    }
    return true;
}

bool json_obj::operator==(const json_obj &other) const
{
    if ( !are_types_same(_type,other.type()))
        return false;
    switch ( _type )
    {
        case json_obj_type_string:
            return (s_val.compare(other.s_val) == 0);
            break;
        case json_obj_type_int:
            return ((long long)i_val == other.get_number());
            break;
        case json_obj_type_llong:
            return (ll_val == other.get_number());
            break;
        case json_obj_type_array:
            return compare_array(other);
            break;
        case json_obj_type_dic:
            return compare_dic(other);
            break;
        default:
            // cout<<"hello"<<endl;
            return false;
    }
    return false;
}

bool json_obj::operator<(const json_obj &other) const
{
    if ( !are_types_same(_type ,other.type()))
        //throw 500;
        throw (int)LT_DATA_ERROR_UNKNOWN;
    switch ( _type )
    {
        case json_obj_type_string:
            if ( s_val.compare(other.s_val) < 0 )
                return true;
            break;
        case json_obj_type_int:
            if ( (long long)i_val < other.get_number() )
                return true;
            break;
        case json_obj_type_llong:
            if ( ll_val < other.get_number() )
                return true;
            break;
        case json_obj_type_array:
        case json_obj_type_dic:
            if (dumps().compare(other.dumps()) < 0)
                return true;
            break;
        default:
            //throw 600;
            throw (int)LT_DATA_ERROR_UNKNOWN;
    }
    return false;
}

json_obj &json_obj::operator[](const json_obj &key)
{
    if(_type == json_obj_type_unknown)
        _type = json_obj_type_dic;
    if ( _type != json_obj_type_dic )
        //throw 700;
        throw (int)LT_DATA_ERROR_TYPE_DIC;
    return dic_val[key];
}

json_obj &json_obj::operator[](const char *key)
{
    if(_type == json_obj_type_unknown)
        _type = json_obj_type_dic;
    if ( _type != json_obj_type_dic )
        //throw 700;
        throw (int)LT_DATA_ERROR_TYPE_DIC;
    json_obj obj(key);
    return dic_val[obj];
}

json_obj &json_obj::operator[](const string &key)
{
    if(_type == json_obj_type_unknown)
        _type = json_obj_type_dic;
    if ( _type != json_obj_type_dic )
        //throw 700;
        throw (int)LT_DATA_ERROR_TYPE_DIC;
    json_obj obj(key);
    return dic_val[obj];
}

json_obj &json_obj::operator[](const int &k)
{
    if(_type == json_obj_type_unknown)
        _type = json_obj_type_array;
    if ( _type != json_obj_type_array )
        //throw 800;
        throw (int)LT_DATA_ERROR_TYPE_ARRARY;
    return array_val[k];
}

json_obj_type json_obj::type() const
{
    return _type;
}

void json_obj::set_dic()
{
    if ( _type != json_obj_type_unknown )
        //throw 100;
        throw  (int)LT_DATA_ERROR_UNKNOWN;
    _type = json_obj_type_dic;
}

void json_obj::set_array()
{
    if ( _type != json_obj_type_unknown )
        //throw 200;
        throw  (int)LT_DATA_ERROR_UNKNOWN;
    _type = json_obj_type_array;
}

int json_obj::size() const
{
    switch ( _type )
    {
        case json_obj_type_array:
            return array_val.size();
        case json_obj_type_dic:
            return dic_val.size();
        case json_obj_type_string:
            return s_val.size();
        default:
            break;
    }
    return 0;
}

void json_obj::append(const char *val)
{
    json_obj obj(val);
    append(obj);
}

void json_obj::append(const int val)
{
    json_obj obj(val);
    append(obj);
}

void json_obj::append(const long long val)
{
    json_obj obj(val);
    append(obj);
}

void json_obj::append(const string &val)
{
    json_obj obj(val);
    append(obj);
}


void json_obj::append(bool val)
{
    json_obj obj(val);
    append(obj);
}

void json_obj::append(const json_obj &other)
{
    if ( _type == json_obj_type_unknown )
        _type = json_obj_type_array;
    if ( _type != json_obj_type_array )
        //throw 300;
        throw (int)LT_DATA_ERROR_TYPE_ARRARY;
    array_val.push_back(other);
}

void json_obj::merge(const json_obj &other)
{
    if(_type == json_obj_type_unknown)
        _type = json_obj_type_dic;
    
    if ( !are_types_same(_type ,other.type()) )
        //throw 400;
        throw  (int)LT_DATA_ERROR_UNKNOWN;
    if ( _type == json_obj_type_dic )
    {
        for ( json_dic_const_it it = other.dic_val.begin();
              it != other.dic_val.end();
              ++it )
        {
            dic_val.insert(json_pair(it->first, it->second));
        }
    }
    else if ( _type == json_obj_type_array )
    {
        for ( json_array_const_it it = other.array_val.begin();
              it != other.array_val.end();
              ++it )
        {
            array_val.push_back(*it);
        }
    }
    else
        //throw 900;
        throw  (int)LT_DATA_ERROR_UNKNOWN;
}

void json_obj::update_type_by_doc(const rapidjson::Document &doc)
{
    if ( doc.IsBool())
        _type = json_obj_type_bool;
    else if ( doc.IsObject())
        _type = json_obj_type_dic;
    else if ( doc.IsArray())
        _type = json_obj_type_array;
    else if ( doc.IsInt() || doc.IsUint())
        _type = json_obj_type_int;
    else if ( doc.IsInt64() || doc.IsInt64())
        _type = json_obj_type_llong;
    else if ( doc.IsString())
        _type = json_obj_type_string;
    else
        //throw 1100;
        throw  (int)LT_DATA_ERROR_UNKNOWN;
}

void json_obj::update_type_by_val(const rapidjson::Value &val)
{
    if ( val.IsBool())
        _type = json_obj_type_bool;
    else if ( val.IsObject())
        _type = json_obj_type_dic;
    else if ( val.IsArray())
        _type = json_obj_type_array;
    else if ( val.IsInt() || val.IsUint())
        _type = json_obj_type_int;
    else if ( val.IsInt64() || val.IsUint64())
        _type = json_obj_type_llong;
    else if ( val.IsString())
        _type = json_obj_type_string;
    else
        //throw 1100;
        throw  (int)LT_DATA_ERROR_UNKNOWN;
}

json_obj::json_obj(const rapidjson::Value &val)
{
    // cout<<"update_type_by_val"<<endl;
    update_type_by_val(val);
    switch ( _type )
    {
        case json_obj_type_string:
            // cout<<"load_string"<<endl;
            load_string(val);
            break;
        case json_obj_type_int:
            // cout<<"load_int"<<endl;
            load_int(val);
            break;
        case json_obj_type_llong:
            // cout<<"load_int"<<endl;
            load_llong(val);
            break;
        case json_obj_type_bool:
            // cout<<"load_bool"<<endl;
            load_bool(val);
            break;
        case json_obj_type_dic:
            // cout<<"load_dic"<<endl;
            load_dic(val);
            break;
        case json_obj_type_array:
            // cout<<"load_array"<<endl;
            load_array(val);
            break;
        default:
            break;
    }
}

void json_obj::load_dic(const rapidjson::Value &val_input)
{
    for ( rapidjson::Value::ConstMemberIterator itr = val_input.MemberBegin();
          itr != val_input.MemberEnd();
          ++itr )
    {
        const rapidjson::Value &key = itr->name;
        const rapidjson::Value &val = itr->value;
        json_obj key_obi(key);
        json_obj val_obj(val);
        json_obj dic_obj(key, val);
        merge(dic_obj);
    }
}

void json_obj::load_array(const rapidjson::Value &val_input)
{
    // cout<<"In load_array(const rapidjson::Value& val)"<<endl;
    // if(!val_input.IsArray())
    // {
    // 	cout<<"not IsArray"<<endl<<endl;
    // 	getchar();
    // }
    for (unsigned int i = 0; i < val_input.Size(); ++i )
    {
        const rapidjson::Value &val = val_input[i];
        json_obj obj(val);
        array_val.push_back(obj);
    }
}

void json_obj::load_int(const rapidjson::Value &val)
{
    if(val.IsInt())
        i_val = val.GetInt();
    else i_val = val.GetUint();
}

void json_obj::load_llong(const rapidjson::Value &val)
{
    if(val.IsInt64())
        ll_val = val.GetInt64();
    else
        ll_val = val.GetUint64();
}

void json_obj::load_bool(const rapidjson::Value &val)
{
    b_val = val.GetBool();
}

void json_obj::load_string(const rapidjson::Value &val)
{
    s_val = val.GetString();
}

void json_obj::load_dic(const rapidjson::Document &doc)
{
    for ( rapidjson::Value::ConstMemberIterator itr = doc.MemberBegin();
          itr != doc.MemberEnd();
          ++itr )
    {
        const rapidjson::Value &key = itr->name;
        const rapidjson::Value &val = itr->value;
        json_obj key_obi(key);
        json_obj val_obj(val);
        json_obj dic_obj(key, val);
        merge(dic_obj);
    }
}

void json_obj::load_array(const rapidjson::Document &doc)
{
    // cout<<"in load_array"<<endl;
    for ( unsigned int i = 0; i < doc.Size(); ++i )
    {
        const rapidjson::Value &val = doc[i];
        // cout<<"in load_array for i = "<<i<<endl;
        json_obj obj(val);
        array_val.push_back(obj);
    }
}

void json_obj::load_int(const rapidjson::Document &doc)
{
    if(doc.IsInt())
        i_val = doc.GetInt();
    else
        i_val = doc.GetUint();
}

void json_obj::load_llong(const rapidjson::Document &doc)
{
    if(doc.IsInt64())
        ll_val = doc.GetInt64();
    else
        ll_val = doc.GetUint64();
}

void json_obj::load_bool(const rapidjson::Document &doc)
{
    b_val = doc.GetBool();
}

void json_obj::load_string(const rapidjson::Document &doc)
{
    s_val = doc.GetString();
}

void json_obj::loads(const char *s)
{
    std::string str(s);
    loads(str);
}

void json_obj::loads(std::string &str)
{
    this->clear();
    rapidjson::Document doc;
    doc.Parse(str.c_str());
    // cout<<"doc.Parse(str.c_str());"<<endl;
    if ( doc.HasParseError())
        //throw 1000;
        throw  (int)LT_DATA_ERROR_UNKNOWN;
    // cout<<"doc.HasParseError(str);"<<endl;
    update_type_by_doc(doc);
    // cout<<"update_type_by_doc"<<endl;
    switch ( _type )
    {
        case json_obj_type_string:
            load_string(doc);
            break;
        case json_obj_type_int:
            load_int(doc);
            break;
        case json_obj_type_llong:
            load_llong(doc);
            break;
        case json_obj_type_bool:
            load_bool(doc);
            break;
        case json_obj_type_dic:
            load_dic(doc);
            break;
        case json_obj_type_array:
            // cout<<"load_array"<<endl;
            load_array(doc);
            break;
        default:
            break;
    }
}

void json_obj::dump_string(rapidjson::Document &doc) const
{
    doc.SetString(s_val.c_str(), (rapidjson::SizeType) s_val.length(), doc.GetAllocator());
}

void json_obj::dump_int(rapidjson::Document &doc) const
{
    doc.SetInt(i_val);
}

void json_obj::dump_llong(rapidjson::Document &doc) const
{
    doc.SetInt64(ll_val);
}


void json_obj::dump_bool(rapidjson::Document &doc) const
{
    doc.SetBool(b_val);
}

void json_obj::dump_dic(rapidjson::Document &doc) const
{
    doc.SetObject();
    for ( json_dic_const_it it = dic_val.begin();
          it != dic_val.end();
          ++it )
    {
        rapidjson::Value v_key;
        rapidjson::Value v_value;
        const json_obj &key = it->first;
        const json_obj &val = it->second;
        key.dumps(doc, v_key);
        val.dumps(doc, v_value);
        doc.AddMember(v_key, v_value, doc.GetAllocator());
    }
}

void json_obj::dump_array(rapidjson::Document &doc) const
{
    doc.SetArray();
    for ( json_array_const_it it = array_val.begin();
          it != array_val.end();
          ++it )
    {
        rapidjson::Value v_item;
        const json_obj &item = *it;
        item.dumps(doc, v_item);

        doc.PushBack(v_item, doc.GetAllocator());
    }
}

void json_obj::dump_string(rapidjson::Document &doc, rapidjson::Value &val) const
{
    val.SetString(s_val.c_str(), (rapidjson::SizeType) s_val.length(), doc.GetAllocator());
}

void json_obj::dump_int(rapidjson::Document &doc, rapidjson::Value &val) const
{
    val.SetInt(i_val);
}

void json_obj::dump_llong(rapidjson::Document &doc, rapidjson::Value &val) const
{
    val.SetInt64(ll_val);
}

void json_obj::dump_bool(rapidjson::Document &doc, rapidjson::Value &val) const
{
    val.SetBool(b_val);
}

void json_obj::dump_dic(rapidjson::Document &doc, rapidjson::Value &val_out) const
{
    val_out.SetObject();
    for ( json_dic_const_it it = dic_val.begin();
          it != dic_val.end();
          ++it )
    {
        rapidjson::Value v_key;
        rapidjson::Value v_value;
        const json_obj &key = it->first;
        const json_obj &val = it->second;
        key.dumps(doc, v_key);
        val.dumps(doc, v_value);
        val_out.AddMember(v_key, v_value, doc.GetAllocator());
    }
}

void json_obj::dump_array(rapidjson::Document &doc, rapidjson::Value &val_out) const
{
    val_out.SetArray();
    for ( json_array_const_it it = array_val.begin();
          it != array_val.end();
          ++it )
    {
        rapidjson::Value v_item;
        const json_obj &item = *it;
        item.dumps(doc, v_item);
        val_out.PushBack(v_item, doc.GetAllocator());
    }
}

void json_obj::dumps(rapidjson::Document &doc, rapidjson::Value &val) const
{
    switch ( _type )
    {
        case json_obj_type_string:
            // cout<<"dump_string"<<endl;
            dump_string(doc, val);
            break;
        case json_obj_type_int:
            // cout<<"dump_int"<<endl;
            dump_int(doc, val);
            break;
        case json_obj_type_llong:
            // cout<<"dump_llong"<<endl;
            dump_llong(doc, val);
            break;
        case json_obj_type_bool:
            // cout<<"dump_bool"<<endl;
            dump_bool(doc, val);
            break;
        case json_obj_type_dic:
            // cout<<"dump_dic"<<endl;
            dump_dic(doc, val);
            break;
        case json_obj_type_array:
            // cout<<"dump_array"<<endl;
            dump_array(doc, val);
            break;
        default:
            break;
    }
}

void json_obj::dumps(rapidjson::Document &doc) const
{
    switch ( _type )
    {
        case json_obj_type_string:
            dump_string(doc);
            break;
        case json_obj_type_int:
            dump_int(doc);
            break;
        case json_obj_type_llong:
            dump_llong(doc);
            break;
        case json_obj_type_bool:
            dump_bool(doc);
            break;
        case json_obj_type_dic:
            dump_dic(doc);
            break;
        case json_obj_type_array:
            // cout<<"json_obj_type_array"<<endl;
            dump_array(doc);
            break;
        default:
            break;
    }
}

std::string json_obj::dumps() const
{
    if ( is_empty())
        return "";
    rapidjson::Document doc;

    dumps(doc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer <rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    return buffer.GetString();
}

void json_obj::traversal_array_const(boost::function<void( const json_obj &)> f) const
{
    assert(_type == json_obj_type_array);
    for(const json_obj &obj : array_val)
    {
        f(obj);
    }
}

void json_obj::traversal_array(boost::function<void(json_obj & )> f)
{
    assert(_type == json_obj_type_array);
    for(json_obj &obj : array_val)
    {
        f(obj);
    }
}

void json_obj::traversal_dic(boost::function<void (const json_obj &,json_obj &)> f)
{
    assert(_type == json_obj_type_dic);
    for(json_dic_it it = dic_val.begin();
        it != dic_val.end();
        ++it)
    {
        f(it->first,it->second);
    }
}

void json_obj::traversal_dic_const(boost::function<void (const json_obj &,const json_obj &)> f) const
{
    assert(_type == json_obj_type_dic);
    for(json_dic_const_it it = dic_val.begin();
    it != dic_val.end();
    ++it)
    {
        f(it->first,it->second);
    }
}

const json_obj &json_obj::operator[](const char *key) const
{
    if ( _type != json_obj_type_dic )
        //throw 700;
        throw (int)LT_DATA_ERROR_TYPE_DIC;
    json_obj obj(key);
    const json_obj& res_obj = dic_val.find(obj)->second;
    return res_obj;
}

const json_obj &json_obj::operator[](const json_obj &key) const
{
    if ( _type != json_obj_type_dic )
        //throw 700;
        throw (int)LT_DATA_ERROR_TYPE_DIC;
    json_obj obj(key);
    const json_obj& res_obj = dic_val.find(obj)->second;
    return res_obj;
}

const json_obj &json_obj::operator[](const string &key) const
{
    if ( _type != json_obj_type_dic )
        //throw 700;
        throw (int)LT_DATA_ERROR_TYPE_DIC;
    json_obj obj(key);
    const json_obj& res_obj = dic_val.find(obj)->second;
    return res_obj;
}

const json_obj &json_obj::operator[](const int &k) const
{
    if ( _type != json_obj_type_array )
        // throw 800;
        throw (int)LT_DATA_ERROR_TYPE_ARRARY;
    const json_obj& res_obj = array_val[k];
    return res_obj;
}

bool json_obj::are_types_same(json_obj_type type_a, json_obj_type type_b) const
{
    if(type_a == type_b)
        return true;
    if(type_a == json_obj_type_int && type_b == json_obj_type_llong)
        return true;
    if(type_b == json_obj_type_int && type_a == json_obj_type_llong)
        return true;
    return false;
}

long long json_obj::get_number() const
{
    if(_type == json_obj_type_llong)
        return ll_val;
    if(_type == json_obj_type_int)
        return (long long)i_val;
    return 0;
}

#define MAX_JSON_SIZE 51200

int json_obj::loads_fromfile(const std::string &filename)
{
    char *buf = new char [MAX_JSON_SIZE];
    int err = read_data_from_file(filename.c_str(), buf, MAX_JSON_SIZE);
    if(err)
    {
        delete buf;
        return err;
    }
    loads(buf);
    delete buf;
    return 0;
}

int json_obj::dumps_tofile(const std::string &filename) const
{
    return write_str2file(filename.c_str(), dumps().c_str());
}


#ifdef JSON_TEST
int main(int argc, const char *argv[])
{
    // json_obj obj("key","val");
    // json_obj obj1("key","val");
    // json_obj obj2("key","val1");

    // json_obj obj3;
    // obj3.set_array();
    // obj3.append("heee");

    // json_obj obj4;
    // obj4.set_array();
    // obj4.append("heee");
    // obj4.append("eee");

    // json_obj obj5;
    // obj5.set_array();
    // obj5.append("heee");
    // obj5.append("eee");


    // if(obj == obj1) cout<<"right"<<endl;
    // else cout<<"wrong"<<endl;
    // if(obj1 != obj2) cout<<"right"<<endl;
    // else cout<<"wrong"<<endl;
    // if(obj3 != obj2) cout<<"right"<<endl;
    // else cout<<"wrong"<<endl;
    // if(obj3 != obj4) cout<<"right"<<endl;
    // else cout<<"wrong"<<endl;
    // if(obj4 == obj5) cout<<"right"<<endl;
    // else cout<<"wrong"<<endl;

    if (argc < 2){
        cout<<"d                    ;dumps"<<endl;
        cout<<"l                    ;loads"<<endl;
        return -1;
    }
    try
    {
        if(strcmp(argv[1],"d")==0)
        {
            json_obj obj("hello");
            json_obj ar;
            ar.set_array();
            ar.append(obj);
            ar.append(2333);
            ar.append(true);

            // // cout<<obj.dumps()<<endl;
            json_obj obj_map("key","pair");
            json_obj obj_map_2("key1","pair");
            // cout<<obj_map.dumps()<<endl;
            obj_map.merge(obj_map_2);
            obj_map_2 = json_obj("key2",true);
            obj_map.merge(obj_map_2);
            obj_map_2 = json_obj("key3",22222);
            obj_map.merge(obj_map_2);
    // cout<<ar.dumps()<<endl;
            obj_map_2 = json_obj("key4",ar);
    // cout<<obj_map_2.dumps()<<endl;
            obj_map.merge(obj_map_2);
            ar.append(obj_map);
            obj_map = json_obj("test",ar);
            cout<<obj_map.dumps();
        }
        else if (strcmp(argv[1],"l") == 0)
        {
            cout<<argv[2]<<endl;
            json_obj obj;
            obj.loads(argv[2]);
            cout<<obj.dumps()<<endl;
            cout<<obj["test"].dumps()<<endl;;
            cout<<obj["test"][0].dumps()<<endl;
            cout<<obj["test"][3]["key"].dumps()<<endl;
        }
    }
    catch(int &err)
    {
        cout<<err<<endl;
    }
}

#endif
