#ifndef __block_hpp
#define __block_hpp

#include <vector>
#include <iostream>

#include <boost/filesystem.hpp>

#include <png.h>

#include <json_spirit.h>

#include "config.hpp"
#include "texture.hpp"
#include "block_data.hpp"
#include "item_metadata.hpp"

namespace mcmap
{
	class block
	{
	public:
	  block(block_info_t block_info);
	  ~block();

	  png_structp get_rendered_texture();

	private:
	  int required_texture_size();

	  /*
	    looks up the base path (without attached facename and extension)
	    of a texture for a given block id, data value and orientation
	  */
	  texture *get_texture(block_face_t block_face);
	  texture_rotation_t get_texture_rotation(block_face_t block_face);

	  block_info_t block_info;
	  item_metadata *metadata;
	  world_rotation_t world_rotation;
	  boost::filesystem::path texture_basepath;
	};
}

#endif
