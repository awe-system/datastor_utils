#ifndef _SUPER_ALGO_DEVICE_H
#define _SUPER_ALGO_DEVICE_H
#include "algo.h"


namespace ServerSan_Algo
{
extern env disk_info_sec;
extern env pool_info_sec;

typedef std::vector<string> ser_list_t;
typedef std::vector<string>::const_iterator ser_list_const_it;
typedef std::vector<string>::iterator ser_list_it;

class super_algo_device : public algo_device, public block_event_callback, public block_io_callback
{
    unsigned long pool_secs;
    unsigned long disk_secs;
    algo_block_io_super block_no_disk_info;
    algo_block_io_super block;
    algo_device *device;

public:
    super_algo_device(const super_algo_device & other);
    
    super_algo_device(algo_device *_device,
            unsigned long disk_info_secs = ( unsigned long )disk_info_sec.get_int(),
            unsigned long pool_info_secs = ( unsigned long )pool_info_sec.get_int());

    unsigned long long get_sector_num()const;

    int open(void);

    void close(void);

    void do_request(request_t *request);

    void request_done(request_t *request, int err);

    string get_host_name(void) const;

    bool is_local(void) const;

    string get_device_id(void) const;

    int handle_offline(block_event *block);

    int handle_online(block_event *block);

    int write_pool_info(const json_obj & info);

    json_obj read_pool_info();

};//namespace
}
#endif
