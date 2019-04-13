#ifndef _ALGO_BLOCK_MULTI_SUPER_H
#define _ALGO_BLOCK_MULTI_SUPER_H

namespace ServerSan_Algo
{
class algo_block_multi_super
{
private:
	algo_block_vec_t super_blocks;
public:
	algo_block_multi_super();
	algo_block_multi_super(algo_block_vec_t &blocks);
	algo_block_multi_super(const algo_block_multi_super & other);

	void add_block(block_io *block)
	{
		super_blocks.push_back(block);
	}

	void read_super(json_obj &obj);//一言不合就 throw
	void write_super(const json_obj &obj);//一言不合就 throw
	
	virtual ~algo_block_multi_super(){};
};
};

#endif