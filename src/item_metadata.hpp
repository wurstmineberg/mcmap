#ifndef __item_metadata_hpp
#define __item_metadata_hpp

#include <json_spirit.h>

#include "block.hpp"
#include "texture.hpp"

using namespace std;

namespace mcmap
{
	typedef enum {
		ITEM_TYPE_ITEM,
		ITEM_TYPE_BLOCK
	} item_type_t;

	typedef struct {
		std::string name;
		item_type_t type;

	} item_metadata_info_t;

	class item_metadata
	{
	public:
		item_metadata(json_spirit::mObject json_object);
		texture get_texture(block_face_t block_face);
		item_metadata_info_t *get_metadata_info();

	private:
		item_metadata_info_t metadata_info;
		json_spirit::mObject json_data;
		bool get_value_for_key(string key, const json_spirit::mValue *value);
	};

	class item_metadata_store
	{
	public:
		item_metadata_store();
		item_metadata *get_metadata(block_info_t block_info);
		void print_raw_data();

	private:
		bool load_config();
		item_metadata *get_metadata_for_key(string key);
		json_spirit::mObject json_data;
	};
}

#endif
