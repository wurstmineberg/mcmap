#ifndef __block_data_hpp
#define __block_data_hpp

namespace mcmap
{
  typedef struct block_info
  {
    uint32_t id;
    uint8_t data;
  } block_info_t;

  typedef struct
	{
		int id;
		int data;
	} item_info;

	typedef enum
	{
	  BLOCK_FACE_TOP,
	  BLOCK_FACE_NORTH,
	  BLOCK_FACE_WEST,
	  BLOCK_FACE_SOUTH,
	  BLOCK_FACE_EAST,
	  BLOCK_FACE_BOTTOM
	} block_face_t;

	typedef enum {
		ITEM_TYPE_ITEM,
		ITEM_TYPE_BLOCK
	} item_type_t;

	typedef enum {
		ITEM_SUBTYPE_SLAB,
		ITEM_SUBTYPE_SLAB_TOP,
		ITEM_SUBTYPE_STAIRS
	} item_subtype_t;

	typedef struct {
		texture *top;
		texture *north;
		texture *west;
		texture *south;
		texture *east;
		texture *bottom;
	} item_metadata_texture_list_t;
}

#endif
