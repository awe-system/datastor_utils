//
// Created by root on 18-3-15.
//
#include "algo.h"
#include "debug.h"
#include "algo_interface.h"
#include <set>

static env checksum_on("awe_log", "checksum_on");

namespace ServerSan_Algo
{
extern env pool_vols_sec;

block_event::block_event(const block_event &other)
{
    event = other.event;
}

block_event::block_event()
{
    event = NULL;
}

int block_event::offline()
{
    if ( !event )
    {
        return 0;
    }
    return event->handle_offline(this);
}

int block_event::online()
{
    if ( !event )
    {
        return 0;
    }
    return event->handle_online(this);
}

void block_event::set_event_callback(block_event_callback *event_callback)
{
    event = event_callback;
}

int block_event_callback::handle_offline(block_event *block)
{
    return 0;
}

int block_event_callback::handle_online(block_event *block)
{
    return 0;
}


request_t::request_t() : type(REQUEST_UNKNOWN), offset(0), buf(NULL), len(0),
                         private_data(NULL), trace_info("")
{
}

request_t::request_t(unsigned int _len, unsigned char *_buf) :
        type(REQUEST_UNKNOWN), offset(0), buf(_buf), len(_len),
        private_data(NULL), trace_info("")
{
}

request_t::request_t(const request_t &other):trace_info(other.get_trace())
{
    type         = other.type;
    offset       = other.offset;
    buf          = other.buf;
    len          = other.len;
    private_data = other.private_data;
    
    for ( std::vector<void *>::const_iterator      it = other.data_stack
            .begin(); it != other.data_stack.end(); ++it )
    {
        data_stack.push_back(*it);
    }
    for ( std::vector<std::string>::const_iterator it = other.string_stack
            .begin(); it != other.string_stack.end(); ++it )
    {
        string_stack.push_back(*it);
    }
}

void request_t::push_private(void *new_private)
{
    data_stack.push_back(private_data);
    private_data = new_private;
}

void *request_t::pop_private()
{
    void *cur_private = private_data;
    if ( data_stack.size() != 0 )
    {
        private_data = data_stack.back();
        data_stack.pop_back();
    }
    return cur_private;
}

void request_t::push_callback(block_io_callback *cb)
{
    push_private(cb);
}

void request_t::request_done(int error)
{
    block_io_callback *cb = (block_io_callback *) pop_private();
    cb->request_done(this, error);
}

bool request_t::is_read() const
{
    switch ( type )
    {
        case REQUEST_SYNC_READ:
        case REQUEST_ASYNC_READ:
            return true;
            break;
        case REQUEST_SYNC_WRITE:
        case REQUEST_ASYNC_WRITE:
            return false;
            break;
    }
    abort();
}

unsigned long request_t::checksum() const
{
    unsigned      result                     = 0xcc;
    unsigned char res[sizeof(unsigned long)] = {0};
    for ( uint    i                          = 0; i < sizeof(unsigned long);
          ++i )
    {
        res[i] = i * i - 1;
    }
    for ( uint    i                          = 0; i < len; ++i )
    {
        res[i % sizeof(unsigned long)] ^= buf[i];
    }
    memcpy(&result, res, sizeof(unsigned long));
    return result;
}

void request_t::from_json_obj(const json_obj &obj)
{
    abort();
}

static string dump_void(void *p)
{
    char tmp_buf[100] = {0};
    sprintf(tmp_buf, "%p", p);
    return string(tmp_buf);
}

json_obj request_t::to_json_obj() const
{
    json_obj obj("type", (int) type);
    obj.merge(json_obj("offset", (long long) offset));
    lt_data_t data(len, buf);
    obj.merge(json_obj("data_buf", data.to_less_string()));
    obj.merge(json_obj("private_data", (long long) private_data));
    obj.merge(json_obj("private_data_str", dump_void(private_data)));
    if ( checksum_on.get_string() == string("yes") )
    {
        long long checksum_val = checksum();
        obj.merge(json_obj("checksum", (long long) checksum_val));
    }
    json_obj  statck_obj;
    statck_obj.set_array();
    json_obj   statck_str_obj;
    json_obj   string_stack_obj;
    for ( void *data : data_stack )
    {
        statck_obj.append(json_obj((long long) data));
        statck_str_obj.append(json_obj(dump_void(data)));
    }
    for ( auto str : string_stack )
    {
        string_stack_obj.append(json_obj(str));
    }
    obj.merge(json_obj("stack", statck_obj));
    obj.merge(json_obj("stack_str", statck_str_obj));
    obj.merge(json_obj("string_stack", string_stack_obj));
    return obj;
}

void request_t::push_string(const std::string &str)
{
    string_stack.push_back(str);
}

std::string request_t::pop_string()
{
    auto str = string_stack.front();
    string_stack.pop_back();
    return str;
}

void request_t::set_trace(const std::string &trace)
{
    trace_info = trace;
}

std::string request_t::get_trace() const
{
    return  trace_info;
}


block_io::block_io()
{
    io_callback = NULL;
}

block_io::block_io(const block_io &other)
{
    io_callback = other.io_callback;
}

int block_io::set_io_callback(block_io_callback *_io_callback)
{
    io_callback = _io_callback;
}

block_io_callback *block_io::get_io_callback()
{
    return io_callback;
}

void block_io::complete_request(request_t *request, int error)
{
    if ( io_callback )
    {
//        dbg << "io_callback " << (long) io_callback << color_blue << request->to_json_obj().dumps() << end_dbgl;
        io_callback->request_done(request, error);
    }
    else
    {
//        dbg << "stack " << color_blue << request->to_json_obj().dumps() << end_dbgl;
        request->request_done(error);
    }
}

void
block_io::do_request_withcb(request_t *request, block_io_callback *cb)
{
    assert(nullptr == io_callback);
    request->push_private(cb);
    do_request(request);
}

int block_io_set::get_block_async(const json_obj &key, block_io_set_cb_t *,
                                  void *ctx)
{
    return -ERROR_TYPE_NOTSUPPORT;
}

block_io *block_io_set::get_block(const json_obj &key)
{
    abort();
}


class safe_voidp_set
{
    std::mutex       m;
    std::set<void *> s;
public:
    int alloc(void *p)
    {
        std::unique_lock<std::mutex> lck(m);
        auto                         it = s.find(p);
        if ( it == s.end() )
        {
            s.insert(p);
            return ERROR_TYPE_OK;
        }
        return -ERROR_TYPE_MEMORY;
    }
    
    int free(void *p)
    {
        std::unique_lock<std::mutex> lck(m);
        auto                         it = s.find(p);
        if ( it == s.end() )
        {
            
            return -ERROR_TYPE_MEMORY;
        }
        s.erase(p);
        return ERROR_TYPE_OK;
    }
    
    bool is_alive(void *p)
    {
        std::unique_lock<std::mutex> lck(m);
        auto                         it = s.find(p);
        if ( it == s.end() )
        {
            return false;
        }
        return true;
    }
};

static safe_voidp_set            p_sets;

algo_obj::algo_obj()
{
    memset(magic_stack,0xcc,MAGIC_STACK_SIZE);
    p_sets.alloc(this);
}

static bool is_stack_legal(unsigned char empty_stack[])
{
    unsigned char com_stack[MAGIC_STACK_SIZE];
    memset(com_stack, 0xcc, MAGIC_STACK_SIZE);
    return 0 == memcmp(com_stack, empty_stack, MAGIC_STACK_SIZE);
}

algo_obj::~algo_obj()
{
    p_sets.free(this);
}

void algo_obj::assert_legal()
{
    assert(p_sets.is_alive(this));
    
    assert(is_stack_legal(magic_stack));
}

}

ServerSan_Algo::algo_pool_operation *
ServerSan_Algo::algo_cluster_operation::ref_pool(const string &pool_name)
{
    //NOTE:不使用锁 由于业务保证 在remove时 必须不能继续有从0到1的ref增加
    algo_pool_operation *pool = get_pool(pool_name);
    if ( pool )
    { pool->get(); }
    return pool;
}

void ServerSan_Algo::algo_cluster_operation::unref_pool(
        ServerSan_Algo::algo_pool_operation *pool)
{
    //NOTE:不使用锁 由于业务保证 在remove时 必须不能继续有从0到1的ref增加
    pool->put();
}

bool
ServerSan_Algo::algo_cluster_operation::is_pool_ref(const string &pool_name)
{
    //NOTE:不使用锁 由于业务保证 在remove时 必须不能继续有从0到1的ref增加
    algo_pool_operation *pool = get_pool(pool_name);
    if ( !pool )
    {
        return false;
    }
    return pool->is_ref();
}

std::list<ServerSan_Algo::algo_pool_operation *>
ServerSan_Algo::algo_cluster_operation::list_pool()
{
    list<ServerSan_Algo::algo_pool_operation *> pool_list;
    for ( pool_map_it                           it = pool_map.begin();
          it != pool_map.end(); ++it )
    {
        pool_list.push_back(it->second);
    }
    return pool_list;
}

void ServerSan_Algo::algo_cluster_operation::get_pool_list(
        std::list<std::string> &pool_list)
{
    for ( pool_map_it it = pool_map.begin(); it != pool_map.end(); ++it )
    {
        pool_list.push_back(it->first);
    }
}

ServerSan_Algo::algo_vol_operation *
ServerSan_Algo::algo_pool_operation::ref_vol(const string &vol_name)
{
    //NOTE:不使用锁 由于业务保证 在remove时 必须不能继续有从0到1的ref增加
    algo_vol_operation *vol = get_vol(vol_name);
    if ( vol )
    { vol->get(); }
    return vol;
}

void ServerSan_Algo::algo_pool_operation::unref_vol(
        ServerSan_Algo::algo_vol_operation *vol)
{
    //NOTE:不使用锁 由于业务保证 在remove时 必须不能继续有从0到1的ref增加
    vol->put();
    return;
}

bool ServerSan_Algo::algo_pool_operation::is_vol_ref(const string &vol_name)
{
    //NOTE:不使用锁 由于业务保证 在remove时 必须不能继续有从0到1的ref增加
    algo_vol_operation *vol = get_vol(vol_name);
    if ( !vol )
    {
        return false;
    }
    return vol->is_ref();
}


void ServerSan_Algo::algo_pool_operation::read_super(json_obj &obj)
{//FIXME:此处暂时无并发处理
    algo_block_io_super *super = NULL;
    if ( !interval_super )
    {
        create_interval_super();
    }
    super = (algo_block_io_super *) interval_super;
    
    super->read_super(obj);
}

void ServerSan_Algo::algo_pool_operation::write_super(const json_obj &obj)
{//FIXME:此处暂时无并发处理
    algo_block_io_super *super = NULL;
    if ( !interval_super )
    {
        create_interval_super();
    }
    super = (algo_block_io_super *) interval_super;
    super->write_super(obj);
}

ServerSan_Algo::algo_pool_operation::algo_pool_operation() : interval_super(
        NULL)
{
}

void ServerSan_Algo::algo_pool_operation::create_interval_super()
{
    unsigned long long vol_sec       =
                               (unsigned long long) ServerSan_Algo::pool_vols_sec
                                       .get_int();
    string             interval_name = "*";
    algo_vol_operation *vol          = get_vol(interval_name);
    if ( !vol )
    {
        int err = create_vol(interval_name, vol_sec, NULL);
        if ( err )
        { throw err; }
    }
    vol = get_vol(interval_name);
    if ( !vol )
    { throw (int) -ERROR_TYPE_NOVOL; }
    
    try
    {
        algo_block_io_super *super = new algo_block_io_super(vol, vol_sec);
        interval_super = (block_io *) super;
    }
    catch (...)
    {
        throw (int) -ServerSan_Algo::ERROR_TYPE_MEMORY;
    }
    
}

std::list<ServerSan_Algo::algo_vol_operation *>
ServerSan_Algo::algo_pool_operation::list_vol()
{
    list<ServerSan_Algo::algo_vol_operation *> vol_list;
    for ( vol_map_it                           it = _vol_map.begin();
          it != _vol_map.end(); ++it )
    {
        if ( it->first != "*" )
        {
            vol_list.push_back(it->second);
        }
    }
    return vol_list;
}

void ServerSan_Algo::algo_pool_operation::get_vol_list(
        std::list<std::string> &vol_list)
{
    for ( vol_map_it it = _vol_map.begin(); it != _vol_map.end(); ++it )
    {
        if ( it->first != "*" )
        {
            vol_list.push_back(it->first);
        }
    }
}

void
ServerSan_Algo::algo_pool_operation::split_super(const json_obj &whole_info,
                                                 json_obj &memory_info,
                                                 json_obj &storage_info)
{
    try
    {
        storage_info = whole_info["storage"];
        memory_info  = whole_info["memory"];
    }
    catch (...)
    {
        throw (int) -ERROR_TYPE_SUPER_MISMATCH;
    }
}

void ServerSan_Algo::algo_pool_operation::save_super()
{
    json_obj storage = to_json_obj();
    //LOG_INFO("\n\n[in write super] save_super: %s\n", storage.dumps().c_str());
    save(storage);
//    json_obj read_storage;
}

void
ServerSan_Algo::algo_pool_operation::load_super(const json_obj &memory_info)
{
    json_obj whole("memory", memory_info);
    json_obj storage;
    load(storage);
    //LOG_INFO("\n[in load_super] load_super: %s\n", storage.dumps().c_str());
    whole.merge(json_obj("storage", storage));
    from_json_obj(whole);
}

ServerSan_Algo::algo_pool_operation::~algo_pool_operation()
{
    algo_block_io_super *super = (algo_block_io_super *) interval_super;
    delete super;
}
