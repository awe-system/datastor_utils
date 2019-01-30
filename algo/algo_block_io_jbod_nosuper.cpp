#include "debug.h"
#include "algo.h"

using namespace ServerSan_Algo;

algo_block_io_jbod_nosuper::algo_block_io_jbod_nosuper(algo_block_vec_t &blocks) : _sector_count(0), _is_open(false)
{
    for ( algo_block_vec_it it = blocks.begin(); it != blocks.end(); ++it )
    {
        block_io *block = *it; //这是super_block
        unsigned long this_secs = block->get_sector_num(); //algo_device_sec_num - super_sec
        int err = _request_mapper.add_block(this_secs, block);
        if ( err )
            throw err;
        _block_vector.push_back(block);
        block->set_io_callback((block_io_callback *) this);
        _sector_count += this_secs;
    }
}

algo_block_io_jbod_nosuper::~algo_block_io_jbod_nosuper()
{}

int algo_block_io_jbod_nosuper::add_block(block_io *block)
{
//    assert(!_is_open);
    std::unique_lock<std::mutex> lck(m);
    unsigned long this_secs = block->get_sector_num();
    int err = _request_mapper.add_block(this_secs, block);
    if ( err )
        throw err;
    _block_vector.push_back(block);
    block->set_io_callback((block_io_callback *) this);
    _sector_count += this_secs;
    return err;
}

unsigned long long algo_block_io_jbod_nosuper::get_sector_num() const
{
    return _sector_count;
}

int algo_block_io_jbod_nosuper::open(void)//只能被open打开
{
    int err = ERROR_TYPE_OK;
    std::unique_lock<std::mutex> lck(m);
    algo_block_vec_it open_it;
    try
    {
        for ( open_it = _block_vector.begin(); open_it != _block_vector.end(); ++open_it )
        {
            block_io *block = *open_it;
            err = block->open();
            if ( err )
                throw err;
        }
    }
    catch (...)
    {
        for ( algo_block_vec_it close_it = _block_vector.begin(); close_it != open_it; ++close_it )
        {
            block_io *block = *close_it;
            block->close();
        }
        return err;
    }
    _is_open = true;
    return err;
}

void algo_block_io_jbod_nosuper::close(void)
{
    std::unique_lock<std::mutex> lck(m);
    for ( algo_block_vec_it close_it = _block_vector.begin();
          close_it != _block_vector.end();
          ++close_it )
    {
        block_io *block = *close_it;
        block->close();
    }
}

static debug ji_dbg(color_red);

void algo_block_io_jbod_nosuper::do_request(request_t *request)
{
    //ji_dbg.start();
    ji_dbg.set_color(color_purple);
    ji_dbg<<"do_request "<<(void *)request<<" data:"<<request->private_data<<DEBUG_END;
    ji_dbg<<request->data_stack;
    block_request_list_t requests;
    int err = _request_mapper.map_request(request, requests);
    if ( err )
    {
        complete_request(request, err);
        return;
    }

    for(block_request_list_t::iterator it = requests.begin();
            it!= requests.end();++it)
    {
        block_request_t &block_request = *it;
        ji_dbg.set_color(color_red);
        ji_dbg<<"do request"<<block_request.request<<DEBUG_END;
        ji_dbg<<block_request.request->data_stack<<DEBUG_END;
    }

    _request_mapper.do_request(requests);
}

void algo_block_io_jbod_nosuper::request_done(request_t *request, int err)
{
    ji_dbg.set_color(color_red);
    ji_dbg<<"done request"<<request<<DEBUG_END;
    ji_dbg<<"err:"<<err<<DEBUG_END;
    ji_dbg<<request->data_stack<<DEBUG_END;

    request_t *end_request = _request_mapper.unmap_request(request, err);
    if ( end_request )
    {
        ji_dbg.set_color(color_purple);
        ji_dbg<<"request_done:"<<(void *)end_request<<" data:"<<end_request->private_data<<DEBUG_END;
        ji_dbg<<end_request->data_stack<<DEBUG_END;

        complete_request(end_request, err);
    }
}

//#define TEST_ALGO_JBOD
#ifdef TEST_ALGO_JBOD

#include <iostream>

using namespace std;

#include "test_algo_device.h"

class io_callback_service : public block_io_callback
{
    void request_done(request_t *request, int err)
    {
        cout << "request_done err " << error_str(err) << endl;
    }
};

int main(int argc, char const *argv[])
{
    assert(argc >= 4);
    unsigned int super_len = 512;
    unsigned char super_buf[512];

    io_callback_service callback_service;
    string path1 = argv[1];
    string path2 = argv[2];
    string path3 = argv[3];
    string path4 = argv[4];
    unsigned long sec = atoi(argv[5]);
    assert(sec > 1);
    string ser = "ser";
    test_algo_device dev_1(ser, path1, sec);
    test_algo_device dev_2(ser, path2, sec);
    test_algo_device dev_3(ser, path3, (sec - 1) / 2 + 1);
    test_algo_device dev_4(ser, path4, (sec - 1) / 2 + 1);
    algo_block_io_super device1(&dev_1, 1);
    algo_block_io_super device2(&dev_2, 1);
    algo_block_io_super device3(&dev_3, 1);
    algo_block_io_super device4(&dev_4, 1);
    device1.set_io_callback(&callback_service);
    device2.set_io_callback(&callback_service);
    assert(0 == device1.open());
    assert(0 == device2.open());
    assert(0 == device3.open());
    assert(0 == device4.open());

    device1.read_super(super_len, super_buf);
    device2.write_super(super_len, super_buf);
    device3.write_super(super_len, super_buf);
    device4.write_super(super_len, super_buf);

    cout << "after super" << endl;
    algo_block_vec_t blocks;
    blocks.push_back((block_io *) &device3);
    blocks.push_back((block_io *) &device4);
    algo_block_io_jbod_nosuper jbod(blocks);
    jbod.set_io_callback(&callback_service);
    jbod.open();
    unsigned char *buf = new unsigned char[1024];
    request_t *request = new request_t(1024, buf);
    cout << "device1.get_sector_num():" << device1.get_sector_num() << endl;
    unsigned long i = 0;
    for ( i = 0; i < device1.get_sector_num() / 2; i++ )
    {//这个测试时同步的情况
        request->offset = i * 2;
        request->type = REQUEST_SYNC_READ;
        device1.do_request(request);
        request->type = REQUEST_SYNC_WRITE;
        jbod.do_request(request);
    }
    for ( i = 0; i < device1.get_sector_num() / 2; i++ )
    {//这个测试时同步的情况
        request->offset = i * 2;
        request->type = REQUEST_SYNC_READ;
        jbod.do_request(request);
        request->type = REQUEST_SYNC_WRITE;
        device2.do_request(request);
    }
    delete buf;
    delete request;
    jbod.close();
    device1.close();
    device2.close();
    device3.close();
    device4.close();
    return 0;
}

#endif