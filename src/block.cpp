#include "block.hpp"

namespace mcmap
{
	block::block(int block_id, int block_data)
	{
		this->block_id = block_id;
		this->block_data = block_data;
	}
}