//
// Created by root on 19-2-12.
//
#include "algo_interface.h"
#include "test_block_io_set.h"
#include "test_algo_device.h"

ServerSan_Algo::block_io *
ServerSan_Algo::test_block_io_set::get_block(const json_obj& key)
{
    auto it = obj_map.find(key);
    if (it == obj_map.end())
        return NULL;
    return it->second;
}

void ServerSan_Algo::test_block_io_set::put_block(ServerSan_Algo::block_io *io)
{//do nothing
}

#define MAX_TEST_FILE 10

ServerSan_Algo::test_block_io_set::test_block_io_set(std::string _file_dir,
                                                     unsigned long _size_secs) :
        file_dir(_file_dir)
{
    for (long long i = 0; i < MAX_TEST_FILE; ++i )
    {
        test_algo_device *device = new test_algo_device(to_string(i),
                                                        _file_dir +
                                                        to_string(i),_size_secs);
        json_obj obj( i );
        obj_map.insert(make_pair(obj,device));
    }
}

void ServerSan_Algo::test_block_io_set::set_io_cb(
        ServerSan_Algo::block_io_callback *cb)
{
    for (auto it : obj_map)
    {
        it.second->set_io_callback(cb);
    }
}

void ServerSan_Algo::test_block_io_set::open_all()
{
    for (auto it : obj_map)
    {
        it.second->open();
    }
}

void ServerSan_Algo::test_block_io_set::close_all()
{
    for (auto it : obj_map)
    {
        it.second->close();
    }
}

int ServerSan_Algo::test_block_io_set::get_block_async(const json_obj &key,
                                                       ServerSan_Algo::block_io_set_cb_t *cb,
                                                       void *ctx)
{
    auto it = obj_map.find(key);
    if (it == obj_map.end())
        return -1;
    cb->block_got(it->second, ctx, 0);
    return 0;
}
