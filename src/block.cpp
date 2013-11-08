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
	}

	block::~block()
	{

	}
}
