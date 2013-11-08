#include "block.hpp"

#include <string>

using namespace std;

namespace mcmap
{
	block::block(block_info_t block_info)
	{
		this->block_info = block_info;
		this->metadata = metadata_store.get_metadata(block_info);
		if (config.verbose) cout << "New block with name: " << this->metadata->get_metadata_info()->name << endl;
		if (config.verbose) cout << "Texture: " << this->get_texture(BLOCK_FACE_TOP)->get_path().string() << endl;
	}

	block::~block()
	{

	}

	texture *block::get_texture(block_face_t block_face)
	{
		return this->metadata->get_texture(block_face);
	}
}
