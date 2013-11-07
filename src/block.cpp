#include "block.hpp"

namespace mcmap
{
	block::block(block_info_t block__info)
	{
		this->block_info = block_info;
		this->texture_basepath = config.assetDir += "minecraft/textures";
	}
}
