#ifndef __item_metadata_hpp
#define __item_metadata_hpp

#include <json_spirit.h>

#include "texture.hpp"
#include "block_data.hpp"
#include "config.hpp"

using namespace std;

namespace mcmap
{
	typedef struct {
		std::string 									name;
		item_type_t 									type;
		item_subtype_t								subtype;
		item_metadata_texture_list_t  textures;
	} item_metadata_info_t;

	class item_metadata
	{
	public:
		item_metadata(json_spirit::mObject json_object);
		item_metadata(item_metadata *default_metadata, json_spirit::mObject itemdict);
		texture *get_texture(block_face_t block_face);
		item_metadata_info_t *get_metadata_info();

	private:
		item_metadata_info_t metadata_info;
		json_spirit::mObject json_data;
		void set_textures();
		texture *get_texture_from_dict(block_face_t block_face);
		void set_metadata_from_dict(json_spirit::mObject itemdict);
	};

	class item_metadata_store
	{
	public:
		item_metadata_store();
		bool load_metadata();
		item_metadata *get_metadata(block_info_t *block_info);
		void print_raw_data();

	private:
		item_metadata *get_metadata_for_key(string key);
		item_metadata *get_metadata_for_key(string key, item_metadata *default_metadata);
		json_spirit::mObject json_data;
	};

	extern item_metadata_store metadata_store;
}

#endif
