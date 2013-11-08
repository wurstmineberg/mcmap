#include "item_metadata.hpp"

#include <iostream>
#include <fstream>
#include <string>

namespace fs = boost::filesystem;

using namespace std;
using namespace mcmap;

namespace mcmap
{
	texture *item_metadata::get_texture(block_face_t block_face)
	{
		const json_spirit::mValue value;
		json_spirit::mObject::const_iterator iterator;
		string *default_texture = NULL;

		iterator = this->json_data.find("texture");
	  if (iterator != this->json_data.end() && iterator->first == "texture")
	  {
	  	default_texture = new string(iterator->second.get_str());
	  }

	  return new texture(*default_texture);
	}

	item_metadata_info_t *item_metadata::get_metadata_info()
	{
		return &this->metadata_info;
	}

	item_metadata::item_metadata(json_spirit::mObject itemdict)
	{
	  this->json_data = itemdict;
		const json_spirit::mValue value;
		json_spirit::mObject::const_iterator iterator;

		iterator = this->json_data.find("name");
	  if (iterator != this->json_data.end() && iterator->first == "name")
	  {
	  	this->metadata_info.name = iterator->second.get_str();
	  }

	  iterator = this->json_data.find("type");
	  if (iterator != this->json_data.end() && iterator->first == "type")
	  {
	  	string name = iterator->second.get_str();
	  	if (name.compare("block") == 0)
	  	{
	  		this->metadata_info.type = ITEM_TYPE_BLOCK;
	  	}
	  }
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

	item_metadata *item_metadata_store::get_metadata(block_info_t block_info)
	{
		string baseKey = std::to_string(block_info.block_id);
		item_metadata *metadata = this->get_metadata_for_key(baseKey);
		return metadata;
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

	    block_info_t block_info;
	    block_info.block_id = 13;
	    block_info.data_value = 0;

	    this->get_metadata(block_info);

	    return true;
	  }

	  return false;
	}
}
