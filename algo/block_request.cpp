#include "algo.h"

using namespace ServerSan_Algo;

block_request_t::block_request_t(request_t *_request, block_io *_block) : request(_request), block(_block)
{
}

block_request_t::block_request_t(const block_request_t &other)
{
    request = other.request;
    block = other.block;
}

void block_request_t::do_request()
{
    block->do_request(request);
}