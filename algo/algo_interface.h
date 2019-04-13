#ifndef ALGO_INTERFACE_H
#define ALGO_INTERFACE_H

#include <assert.h>
#include <map>
#include <string>
#include <vector>
#include <list>
#include "lt_data/json_obj.h"
#include "lt_data/ref_obj.h"


using namespace std;
namespace ServerSan_Algo
{
class block_io;

class block_io_callback;

class block_event;

class block_event_callback;

class algo_device;

class algo_metadata_callback;

class algo_vol_operation;

class algo_pool_operation;

class algo_cluster_operation;

class algo_vol_quiesce;

typedef vector<algo_device *>              algo_device_list_t;
typedef algo_device_list_t::iterator       algo_device_it_t;
typedef vector<block_io *>                 algo_block_vec_t;
typedef vector<block_io *>::iterator       algo_block_vec_it;
typedef vector<block_io *>::const_iterator algo_block_vec_const_it;

typedef map<string, algo_device *>           algo_device_map;
typedef map<string, algo_device *>::iterator algo_device_map_iter;
typedef enum
{
    REQUEST_SYNC_READ = 0,
    REQUEST_SYNC_WRITE,
    REQUEST_ASYNC_READ,
    REQUEST_ASYNC_WRITE,
    REQUEST_UNKNOWN,
}                                            request_type;

static inline unsigned long sec_to_size(unsigned long sec)
{
    return sec << 9;
}

static inline unsigned long size_to_sec(unsigned long size)
{
    return size >> 9;
}

static inline unsigned long k64_to_size(unsigned int sec)
{
    return sec << 16;
}

static inline unsigned long size_to_k64(unsigned long size)
{
    return size << 16;
}

static inline unsigned long k64_to_sec(unsigned long sec)
{
    return sec << 7;
}

static inline unsigned long sec_to_k64(unsigned long k64)
{
    return k64 >> 7;
}

class algo_obj
{
public:
    virtual void from_json_obj(const json_obj &obj) = 0;
    
    virtual json_obj to_json_obj() const = 0;
};

class request_t;

class block_io_callback
{
public:
    virtual void request_done(request_t *request, int err) = 0;
};

class request_t : public algo_obj
{
public:
    request_type        type;
    unsigned long       offset;
    unsigned char       *buf;
    unsigned int        len;
    void                *private_data;
    std::vector<void *> data_stack;
public:
    void from_json_obj(const json_obj &obj) override;
    
    json_obj to_json_obj() const override;
    
    bool is_read() const;
    
    request_t();
    
    request_t(unsigned int _len, unsigned char *_buf);
    
    request_t(const request_t &other);
    
    void push_private(void *new_private);
    
    void *pop_private();
    
    void push_callback(block_io_callback *cb);
    
    void request_done(int error);
};


class block_io
{
public:
    block_io_callback *io_callback;
    
    block_io();
    
    block_io(const block_io &other);

public:
    virtual unsigned long long get_sector_num() const = 0;
    
    virtual int open(void) = 0;
    
    virtual void close(void) = 0;
    
    virtual void do_request(request_t *request) = 0;
    
    virtual void do_request_withcb(request_t *request, block_io_callback *cb);
    
    virtual int set_io_callback(block_io_callback *_io_callback);
    
    virtual void complete_request(request_t *request, int error);
    
    virtual block_io_callback *get_io_callback();
    
    virtual ~block_io(){};
};

class block_event_callback
{
public:
    virtual int handle_offline(block_event *block);
    
    virtual int handle_online(block_event *block);
};

class block_io_set_cb_t
{
public:
    virtual void block_got(block_io * ,void* ctx,int err) = 0;
};

class block_io_set
{
public:
    virtual block_io *get_block(const json_obj & key);
    
    virtual int get_block_async(const json_obj & key,block_io_set_cb_t *, void * ctx);
  
    virtual void put_block(block_io *) = 0;
};
//
class block_event
{
    block_event_callback *event;
public:
    block_event(const block_event &other);
    
    block_event();
    
    int offline();
    
    int online();
    
    void set_event_callback(block_event_callback *event_callback);
};

class algo_metadata_callback
        : public block_io_callback, public block_event_callback
{
};

typedef enum
{
    POOL_TYPE_MAIN_COPY,//类型
    POOL_TYPE_MAIN_RAID,
    POOL_TYPE_MAIN_DEDUP,
    POOL_TYPE_EXTRA_RAID,
    POOL_TYPE_EXTRA_COPY,
    POOL_TYPE_EXTRA_DISK,
    POOL_TYPE_JBOD,
    POOL_TYPE_UNKNOWN,
}                                            pool_type;

class algo_pool_info
{
public:
    algo_device_list_t algo_device_list;
    
    // virtual algo_pool_info(string &attr);
    virtual pool_type get_type() = 0;
    // virtual string get_pool_name() = 0;
};

class algo_cluster_operation
{
public:
    virtual algo_pool_operation *get_pool(const string &pool_name) = 0;
    
    virtual int regroup_pool(algo_device_list_t &algo_device_list,
                             block_io_callback *io_callback) = 0;
    
    virtual int
    create_pool(const string &poolname, algo_pool_info *pool_info) = 0;
    
    virtual int remove_pool(const string &pool_name) = 0;
    
    virtual int
    extend_pool(const string &main_pool, const string &extra_pool) = 0;
    
    virtual int
    migrate_volume(const string &org_pool_name, const string &org_vol_name,
                   const string &dest_pool_name,
                   const string &dest_vol_name) = 0;

protected:
    std::map<string, algo_pool_operation *> pool_map;
public:
    std::list<algo_pool_operation *> list_pool();
    
    void get_pool_list(std::list<std::string> &pool_list);

public:
    algo_pool_operation *ref_pool(const string &pool_name);
    
    void unref_pool(algo_pool_operation *pool);
    
    
    bool is_pool_ref(const string &pool_name);
};


class algo_pool_operation
        : public block_event_callback, public ref_obj, public algo_obj
{
private:
    block_io *interval_super;
protected:
    void create_interval_super();

protected:
    std::map<string, algo_vol_operation *> _vol_map;
private:
    
    virtual void
    save(const json_obj &storage_info) = 0;//池内信息的保存实际使用时直接伴组合to_json_obj使用
    
    virtual void load(json_obj &storage_info) = 0;//从设备中读取信息
    
    virtual void clear_vol_map() = 0;

public:
    
    virtual ~algo_pool_operation();
    
    virtual algo_vol_operation *get_vol(const string &vol_name) = 0;
    
    virtual string get_name() const = 0;
    
    virtual algo_pool_info *get_pool_info() = 0;
    
    //卷和池相关的
    virtual int create_vol(const string &vol_name, unsigned long long size_sec,
                           block_io_callback *io_callback) = 0;//包含精简配置
    
    virtual int remove_vol(const string &vol_name) = 0;
    
    virtual int snap_vol(const string &org_vol, const string &new_vol) = 0;
    
    virtual int revert_vol(const string &org_vol, const string &snap_vol) = 0;
    
    virtual int clone_vol(const string &org_vol, const string &new_vol) = 0;
    
    virtual int expand(algo_device_list_t &algo_device_list) = 0;
    
    virtual int rebalance(void) = 0;
    
    virtual unsigned long long get_pool_size() = 0;
    
    virtual unsigned long long get_free_size() = 0;

public:
    algo_pool_operation();
    
    algo_pool_operation(block_io *interval_super);
    
    std::list<algo_vol_operation *> list_vol();
    
    void get_vol_list(std::list<std::string> &vol_list);
    
    algo_vol_operation *ref_vol(const string &vol_name);
    
    void unref_vol(algo_vol_operation *vol);
    
    bool is_vol_ref(const string &vol_name);
    
    void read_super(json_obj &obj);//为上层pool提供的读取其supper的内容
    
    void write_super(const json_obj &obj);//为上层pool提供写入super的内容
    
    void split_super(const json_obj &whole_info, json_obj &memory_info,
                     json_obj &storage_info);
    
    void save_super();//将自身的super保存在介质中
    
    void load_super(const json_obj &memory_info);//将自身的super保存在介质中
};

class algo_device : public block_io, public block_event
{
public:
    virtual string get_host_name(void) const = 0;
    
    virtual bool is_local(void) const = 0;
    
    virtual string get_device_id(void) const = 0;
};


class algo_vol_operation : public algo_device, public ref_obj
{
public:
    virtual int expand(unsigned long new_size) = 0;
    
    virtual string get_name() const = 0;
    
    virtual algo_pool_operation *get_pool() = 0;
};

typedef std::map<string, algo_pool_operation *>                pool_map_t;
typedef std::map<string, algo_pool_operation *>::iterator      pool_map_it;
typedef std::pair<string, algo_pool_operation *>               pool_pair;
typedef std::map<string, algo_pool_operation *>::iterator      pool_it;
typedef std::map<string, algo_vol_operation *>                 vol_map_t;
typedef std::map<string, algo_vol_operation *>::iterator       vol_map_it;
typedef std::pair<string, algo_vol_operation *>                vol_pair;
typedef std::map<string, algo_vol_operation *>::iterator       vol_it;
typedef std::map<string, algo_vol_operation *>::const_iterator const_vol_it;
typedef std::vector<algo_vol_operation *>                      vol_list_t;
typedef std::vector<algo_vol_operation *>::iterator            vol_list_it;
};
#endif