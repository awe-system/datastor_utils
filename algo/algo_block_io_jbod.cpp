#include "debug.h"
#include "algo.h"

static env jbod_super("awe_algo", "jbod_super_sec");
const unsigned long  jbod_super_byte = ((unsigned long)jbod_super.get_int())<<10;
const unsigned long  jbod_super_sec = ((unsigned long)jbod_super.get_int());

using namespace ServerSan_Algo;

void algo_block_io_jbod::clear_super_blocks()
{
    for(algo_block_vec_it it = super_blocks.begin();
        it != super_blocks.end();
        ++it)
    {
        block_io * blk_io = *it;
        algo_block_io_super *io_super = (algo_block_io_super *)blk_io;
        delete io_super;
    }
}

void algo_block_io_jbod::gen_super_blocks(algo_block_vec_t &_blocks)
{
//FIXME:释放构造super_blocks
    for(algo_block_vec_it it = _blocks.begin();
        it != _blocks.end();
        ++it)
    {
        block_io * blk_io = *it;

        algo_block_io_super *io_super = new algo_block_io_super(blk_io,jbod_super_sec);
        super_blocks.push_back((block_io *)io_super);
    }
}

algo_block_io_super* algo_block_io_jbod::add_super_block(block_io *_block)  //FIXME
{
    algo_block_io_super *io_super = new algo_block_io_super(_block, jbod_super_sec);
    super_blocks.push_back((block_io *)io_super);

    return io_super;
}

algo_block_io_jbod::~algo_block_io_jbod()
{
    delete nosuper;
    clear_super_blocks();
}

algo_block_io_jbod::algo_block_io_jbod(algo_block_vec_t &blocks)
{
    gen_super_blocks(blocks);
    nosuper = new algo_block_io_jbod_nosuper(super_blocks);
    multi_super = algo_block_multi_super(super_blocks);
    nosuper->set_io_callback((block_io_callback *) this);
}

int algo_block_io_jbod::add_block(block_io *block)
{
    algo_block_io_super *block_io_super = add_super_block(block);
    nosuper->add_block(block_io_super);
    multi_super.add_block(block_io_super);
    nosuper->set_io_callback((block_io_callback *) this);
}

unsigned long long algo_block_io_jbod::get_sector_num()const
{
    return nosuper->get_sector_num();
}

int algo_block_io_jbod::open(void)//只能被open打开
{
    return nosuper->open();
}

void algo_block_io_jbod::close(void)
{
   nosuper->close();
}

static debug bij_dbg;
void algo_block_io_jbod::do_request(request_t *request)
{

    bij_dbg.set_color(color_yellow);
    //bij_dbg.start();
    bij_dbg<<"request"<<request<<DEBUG_END;
    bij_dbg<<request->data_stack<<DEBUG_END;
    nosuper->do_request(request);
}

void algo_block_io_jbod::request_done(request_t *request, int err)
{
    bij_dbg.set_color(color_yellow);
    //bij_dbg.start();
    bij_dbg<<"request"<<request<<DEBUG_END;
    bij_dbg<<request->data_stack<<DEBUG_END;

    complete_request(request,err);
}

void algo_block_io_jbod::read_super(json_obj &obj)//一言不合就 throw
{
    multi_super.read_super(obj);
}

void algo_block_io_jbod::write_super(const json_obj &obj)//一言不合就 throw
{
    multi_super.write_super(obj);
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
    algo_block_io_jbod jbod(blocks);
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