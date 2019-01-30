#include "algo.h"

using namespace ServerSan_Algo;


int algo_nocopy_request_mapper::map_request(request_t *request, block_request_list_t &block_request_list)
{
    algo_section_vec_t sections;
    algo_section section(request);
    ranges.map_section(section, sections);

    request_ref *nocopy_request = NULL;
    try
    {
        nocopy_request = new request_ref(request);
    } catch (...)
    {
        return ERROR_TYPE_MEMORY;
    }
    unsigned long cur_len = 0;
    int pos = ranges.rangepos_by_orgaddr(request->offset);

    for ( algo_section_vec_it it = sections.begin(); it != sections.end(); ++it, ++pos )
    {
        algo_section &sec = *it;

        //将一个连续的request查分到多个request当中（依据section）
        unsigned int this_size = (unsigned int) sec_to_size(sec.len);

        request_t *new_request = new request_t(this_size, request->buf + cur_len);
        new_request->offset = sec.start;
        //继承了原生request的属性
        new_request->type = request->type;
        //指向公共request
        new_request->private_data = nocopy_request->get();
        //填充结果
        block_request_t block_request(new_request, blocks[pos]);
        block_request_list.push_back(block_request);

        cur_len += this_size;
    }

    return ERROR_TYPE_OK;
}

int algo_nocopy_request_mapper::add_block(unsigned long new_len, block_io *block)
{
    algo_range range(len, 0, new_len);
    algo_ranges new_ranges(range);
    ranges += new_ranges;
    blocks.push_back(block);
    len += new_len;
    return 0;
}

int algo_nocopy_request_mapper::set_ranges_block(algo_ranges &_ranges, block_io *block)
{
    ranges = _ranges;
    blocks.clear();
    for ( int i = 0; i < _ranges.get_range_num(); ++i )
    {
        blocks.push_back(block);
    }
    len = ranges.length();
    return 0;
}


void algo_nocopy_request_mapper::do_request(block_request_list_t &request_list)
{
    for ( block_request_it it = request_list.begin(); it != request_list.end(); ++it )
    {
        block_request_t &block_request = *it;
        block_request.do_request();
    }
}

//当最后一个request返回时才会返回request否则返回空
request_t *algo_nocopy_request_mapper::unmap_request(request_t *request, int &err)
{
    request_ref *nocopy_request = (request_ref *) request->private_data;
    if ( err )
        nocopy_request->err = err;
    request_t *res = nocopy_request->put();
    if ( res )
    {
        delete nocopy_request;
        err = nocopy_request->err;
    }
    delete request;
    return res;
}

void algo_nocopy_request_mapper::set_block_format(block_format *format1)
{
    format = format1;
}

algo_nocopy_request_mapper::algo_nocopy_request_mapper() : format(NULL)
{

}

void algo_nocopy_request_mapper::from_json_obj(const json_obj &obj)
{
    assert(format);
    len = obj["len"].ll_val;
    ranges.from_json_obj(obj["ranges"]);
    for(const json_obj& block_obj : obj.array_val)
    {
        block_io * block = format->from_block_obj(block_obj);
        blocks.push_back(block);
    }
}

json_obj algo_nocopy_request_mapper::to_json_obj() const
{
    assert(format);
    json_obj obj("len", (long long )len);
    obj.merge(json_obj("ranges",ranges.to_json_obj()));
    json_obj blocks_obj;
    blocks_obj.set_array();
    for(const block_io* block : blocks)
    {
        blocks_obj.append(format->to_block_obj(block));

    }
    obj.merge(json_obj("blocks",blocks_obj));
    return obj;
}
