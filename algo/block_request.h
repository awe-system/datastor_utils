#ifndef _BLOCK_REQUEST_H
#define _BLOCK_REQUEST_H

#include <vector>

using namespace std;
namespace ServerSan_Algo
{

class block_request_t
{
public:
    request_t *request;
    block_io *block;//block_io
public:
    block_request_t(request_t *_request, block_io *_block);

    block_request_t(const block_request_t &other);

    void do_request();
};

typedef vector<block_request_t> block_request_list_t;
typedef vector<block_request_t>::iterator block_request_it;

} //namespace
#endif