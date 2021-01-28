//
// Created by root on 2021/1/27.
//

#ifndef DATASTOR_UTILS_PSYNC_DEVICE_H
#define DATASTOR_UTILS_PSYNC_DEVICE_H


class psync_device : public ServerSan_Algo::block_io, public ServerSan_Algo::algo_obj
{
public:
    psync_device(const string &_path, unsigned long _size_secs);
    virtual ~psync_device();
    void from_json_obj(const json_obj &obj) override;
    json_obj to_json_obj() const override;
    unsigned long long int get_sector_num() const override;
    int open(void) override;
    void close(void) override;
    void do_request(ServerSan_Algo::request_t *request) override;
    void do_request_sync(ServerSan_Algo::request_t *request);
private:
    void do_request_write(ServerSan_Algo::request_t *request);
    void do_request_read(ServerSan_Algo::request_t *request);
private:
    void request_mem_align_read(ServerSan_Algo::request_t *request);
    void request_mem_recovery_read(ServerSan_Algo::request_t *request);
    void request_mem_align_write(ServerSan_Algo::request_t *request);
    void request_mem_recovery_write(ServerSan_Algo::request_t *request);
private:
    string path;
    unsigned long long sector_num;
    int fd;
};


#endif //DATASTOR_UTILS_PSYNC_DEVICE_H
