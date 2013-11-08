#include "item_metadata.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <boost/format.hpp>
#include <inttypes.h>

namespace fs = boost::filesystem;

using namespace std;
using namespace mcmap;

namespace mcmap
{
	texture *item_metadata::get_texture(block_face_t block_face)
	{
		if (block_face == BLOCK_FACE_TOP)
		{
			return this->metadata_info.textures.top;
		}
		if (block_face == BLOCK_FACE_NORTH)
		{
			return this->metadata_info.textures.north;
		}
		if (block_face == BLOCK_FACE_WEST)
		{
			return this->metadata_info.textures.west;
		}
		if (block_face == BLOCK_FACE_SOUTH)
		{
			return this->metadata_info.textures.south;
		}
		if (block_face == BLOCK_FACE_EAST)
		{
			return this->metadata_info.textures.east;
		}
		if (block_face == BLOCK_FACE_BOTTOM)
		{
			return this->metadata_info.textures.bottom;
		}
	}

	void item_metadata::set_textures()
	{
		texture *my_texture = NULL;

		my_texture = this->get_texture_from_dict(BLOCK_FACE_TOP);
		if (my_texture) {
			this->metadata_info.textures.top = my_texture;
		}

		my_texture = this->get_texture_from_dict(BLOCK_FACE_NORTH);
		if (my_texture) {
			this->metadata_info.textures.north = my_texture;
		}

		my_texture = this->get_texture_from_dict(BLOCK_FACE_WEST);
		if (my_texture) {
			this->metadata_info.textures.west = my_texture;
		}

		my_texture = this->get_texture_from_dict(BLOCK_FACE_SOUTH);
		if (my_texture) {
			this->metadata_info.textures.south = my_texture;
		}

		my_texture = this->get_texture_from_dict(BLOCK_FACE_EAST);
		if (my_texture) {
			this->metadata_info.textures.east = my_texture;
		}

		my_texture = this->get_texture_from_dict(BLOCK_FACE_BOTTOM);
		if (my_texture) {
			this->metadata_info.textures.bottom = my_texture;
		}
	}

	texture *item_metadata::get_texture_from_dict(block_face_t block_face)
	{
		const json_spirit::mValue value;
		json_spirit::mObject::const_iterator iterator;
		string texture_name;
		bool has_texture = false;

		iterator = this->json_data.find("texture");
	  if (iterator != this->json_data.end() && iterator->first == "texture")
	  {
	  	texture_name = string(iterator->second.get_str());
	  }

	  if (block_face == BLOCK_FACE_NORTH ||
	  		block_face == BLOCK_FACE_WEST ||
	  		block_face == BLOCK_FACE_SOUTH ||
	  		block_face == BLOCK_FACE_EAST)
	  {
	  	iterator = this->json_data.find("texture_side");
		  if (iterator != this->json_data.end() && iterator->first == "texture_side")
		  {
		  	has_texture = true;
		  	texture_name = string(iterator->second.get_str());
		  }
	  }

	  if (block_face == BLOCK_FACE_NORTH)
	  {
	    iterator = this->json_data.find("texture_north");
		  if (iterator != this->json_data.end() && iterator->first == "texture_north")
		  {
		  	has_texture = true;
		  	texture_name = string(iterator->second.get_str());
		  }
	  }

	 	if (block_face == BLOCK_FACE_WEST)
	  {
	    iterator = this->json_data.find("texture_west");
		  if (iterator != this->json_data.end() && iterator->first == "texture_west")
		  {
		  	has_texture = true;
		  	texture_name = string(iterator->second.get_str());
		  }
	  }

	  if (block_face == BLOCK_FACE_SOUTH)
	  {
	    iterator = this->json_data.find("texture_south");
		  if (iterator != this->json_data.end() && iterator->first == "texture_south")
		  {
		  	has_texture = true;
		  	texture_name = string(iterator->second.get_str());
		  }
	  }

	  if (block_face == BLOCK_FACE_EAST)
	  {
	    iterator = this->json_data.find("texture_east");
		  if (iterator != this->json_data.end() && iterator->first == "texture_east")
		  {
		  	has_texture = true;
		  	texture_name = string(iterator->second.get_str());
		  }
	  }

	  if (block_face == BLOCK_FACE_TOP)
	  {
	    iterator = this->json_data.find("texture_top");
		  if (iterator != this->json_data.end() && iterator->first == "texture_top")
		  {
		  	has_texture = true;
		  	texture_name = string(iterator->second.get_str());
		  }
	  }

	  if (block_face == BLOCK_FACE_BOTTOM)
	  {
	    iterator = this->json_data.find("texture_bottom");
		  if (iterator != this->json_data.end() && iterator->first == "texture_bottom")
		  {
		  	has_texture = true;
		  	texture_name = string(iterator->second.get_str());
		  }
	  }

	  if (has_texture)
	  {
	  	return new texture(texture_name);
	  }

	  return NULL;
	}

	item_metadata_info_t *item_metadata::get_metadata_info()
	{
		return &this->metadata_info;
	}

	item_metadata::item_metadata(item_metadata *default_metadata, json_spirit::mObject itemdict)
	{
		memset(&this->metadata_info, 0, sizeof(this->metadata_info));
		this->set_metadata_from_dict(default_metadata->json_data);
		this->json_data = itemdict;
		this->set_metadata_from_dict(itemdict);
	}

	item_metadata::item_metadata(json_spirit::mObject itemdict)
	{
		memset(&this->metadata_info, 0, sizeof(this->metadata_info));
		this->json_data = itemdict;
		this->set_metadata_from_dict(itemdict);
	}

	void item_metadata::set_metadata_from_dict(json_spirit::mObject itemdict)
	{
		const json_spirit::mValue value;
		json_spirit::mObject::const_iterator iterator;

		iterator = itemdict.find("name");
	  if (iterator != itemdict.end() && iterator->first == "name")
	  {
	  	this->metadata_info.name = iterator->second.get_str();
	  }

	  iterator = itemdict.find("type");
	  if (iterator != itemdict.end() && iterator->first == "type")
	  {
	  	string name = iterator->second.get_str();
	  	if (name.compare("block") == 0)
	  	{
	  		this->metadata_info.type = ITEM_TYPE_BLOCK;
	  	}
	  }

	  this->set_textures();
	}

	item_metadata_store::item_metadata_store()
	{
		this->load_metadata();
	}

	item_metadata *item_metadata_store::get_metadata_for_key(string key)
	{
    json_spirit::mObject::const_iterator iterator = this->json_data.find(key);

    if (iterator != this->json_data.end() && iterator->first == key)
    {
	    json_spirit::mValue itemdict = iterator->second;

	    return new item_metadata(itemdict.get_obj());
    }

    return NULL;
	}

	item_metadata *item_metadata_store::get_metadata_for_key(string key, item_metadata *default_metadata)
	{
    json_spirit::mObject::const_iterator iterator = this->json_data.find(key);

    if (iterator != this->json_data.end() && iterator->first == key)
    {
	    json_spirit::mValue itemdict = iterator->second;

	    return new item_metadata(default_metadata, itemdict.get_obj());
    }

    return NULL;
	}

	item_metadata *item_metadata_store::get_metadata(block_info_t *block_info)
	{
		stringstream baseStream;
		baseStream << boost::format("%02i") % block_info->id;
		item_metadata *metadata = this->get_metadata_for_key(baseStream.str());

		stringstream dataStream;
		dataStream << boost::format("%02i:%01i") % block_info->id % (int)(block_info->data);

		item_metadata *enhanced_metadata = NULL;

		if (metadata) {
			enhanced_metadata = this->get_metadata_for_key(dataStream.str(), metadata);
		} else {
			enhanced_metadata = this->get_metadata_for_key(dataStream.str());
		}

		if (enhanced_metadata) {
			return enhanced_metadata;
		} else {
			return metadata;
		}
	}

	bool item_metadata_store::load_metadata()
	{
	  // load items.json
	  fs::path p(fs::current_path());
	  p /= "misc/items.json";

	  if (!fs::exists(p)) return false;
	  if (config.verbose) cout << "Using items.json " << p << endl;

	  ifstream in(p.string().c_str());
	  if (in.is_open())
	  {
	    json_spirit::mValue data;
	    json_spirit::read(in, data);

	    this->json_data = data.get_obj();

	    return true;
	  }

	  return false;
	}
}
