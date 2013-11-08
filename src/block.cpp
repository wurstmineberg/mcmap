#include "block.hpp"

namespace mcmap
{
	block::block(block_info_t block_info)
	{
		this->block_info = block_info;
		this->metadata = metadata_store.get_metadata(block_info);
	}
}
