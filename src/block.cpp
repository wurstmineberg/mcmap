#include "block.hpp"

#include <string>

using namespace std;

namespace mcmap
{
	block::block(block_info_t block_info)
	{
		this->block_info = block_info;
		this->metadata = metadata_store.get_metadata(&block_info);
		if (this->metadata->get_metadata_info()->type != ITEM_TYPE_BLOCK)
		{
			if (config.verbose) cout << "Warning: specified item is not a block. Failing silently: " << this->metadata->get_metadata_info()->name << endl;
		}

		if (config.verbose) cout << "New block with name: " << this->metadata->get_metadata_info()->name << endl;
		texture *mytexture = this->get_texture(BLOCK_FACE_EAST);

		if (config.verbose) cout << "Texture: " << mytexture->to_string() << endl;
	}

	block::~block()
	{
		delete this->metadata;
	}

	texture *block::get_texture(block_face_t block_face)
	{
		return this->metadata->get_texture(block_face);
	}
}
