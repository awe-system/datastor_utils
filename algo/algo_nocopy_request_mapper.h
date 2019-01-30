#ifndef _ALGO_NOCOPY_REQUEST_MAPPER
#define _ALGO_NOCOPY_REQUEST_MAPPER

namespace ServerSan_Algo
{

class block_format
{
public:
    virtual json_obj to_block_obj(const block_io *block) const =0;
    virtual block_io *from_block_obj(const json_obj &) = 0;
};

//只考虑组合的方式 也就是说一个range只有一个block_io
class algo_nocopy_request_mapper: public algo_obj
{
    unsigned long len;
    algo_block_vec_t blocks;
    algo_ranges ranges;
    block_format * format;
public:
    void from_json_obj(const json_obj &obj) override;

    json_obj to_json_obj() const override;

    algo_nocopy_request_mapper();

    void set_block_format(block_format * format1);
    int map_request(request_t *request, block_request_list_t &block_request_list);

    int add_block(unsigned long new_len, block_io *block);

    int set_ranges_block(algo_ranges &_ranges, block_io *block);

    void do_request(block_request_list_t &request_list);

    //当最后一个request返回时才会返回request否则不返回
    request_t *unmap_request(request_t *request, int &err);
};

};
#endif