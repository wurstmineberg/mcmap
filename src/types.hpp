#ifndef __types_hpp
#define __types_hpp

namespace mcmap
{
  /* basic data types */
  typedef enum
  {
    RENDER_MODE_TOP       = 0x1,
    RENDER_MODE_ISOMETRIC = 0x2
  } render_mode_t;

  typedef enum
  {
    WORLD_ROTATION_NORTH      = 0x01,
    WORLD_ROTATION_NORTH_EAST = 0x02,
    WORLD_ROTATION_EAST       = 0x04,
    WORLD_ROTATION_SOUTH_EAST = 0x08,
    WORLD_ROTATION_SOUTH      = 0x10,
    WORLD_ROTATION_SOUTH_WEST = 0x20,
    WORLD_ROTATION_WEST       = 0x40,
    WORLD_ROTATION_NORTH_WEST = 0x80
  } world_rotation_t;

  typedef enum
  {
    DIMENSION_UNDEFINED = 0,
    DIMENSION_OVERWORLD = 1,
    DIMENSION_NETHER    = 2,
    DIMENSION_END       = 4
  } dimension_t;

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

  typedef enum {
    ITEM_TYPE_ITEM,
    ITEM_TYPE_BLOCK
  } item_type_t;

  typedef enum {
    ITEM_SUBTYPE_SLAB,
    ITEM_SUBTYPE_SLAB_TOP,
    ITEM_SUBTYPE_STAIRS
  } item_subtype_t;

  typedef enum
  {
    BLOCK_FACE_TOP,
    BLOCK_FACE_NORTH,
    BLOCK_FACE_WEST,
    BLOCK_FACE_SOUTH,
    BLOCK_FACE_EAST,
    BLOCK_FACE_BOTTOM
  } block_face_t;

  /* data storage structures */

  class texture;
  typedef struct {
    texture *top;
    texture *north;
    texture *west;
    texture *south;
    texture *east;
    texture *bottom;
  } item_metadata_texture_list_t;

  class region_map;
  typedef struct region
  {
    int regionX;
    int regionZ;
    unsigned long filesize;
    region_map *map;
  } region_t;

  typedef struct r_chunk_info
  {
    int offset;
    int size;

    int timestamp;
  } r_chunk_info_t;

  typedef struct chunk_layer
  {
    unsigned char y;

    block_info_t  blocks[4096];

    char skylight[4096];
    char blocklight[4096];
  } chunk_layer_t;

  typedef struct dimension_data
  {
    dimension_t dimension;

    std::string name;

    int num_regions;
    int max_x_extent;
    int max_z_extent;

    std::vector<region_t> regions;
  } dimension_data_t;

  /* misc */

  typedef struct 
  {
    int major;
    int minor;
    int patch;
    char hash[10];
  } version_t; 

  typedef struct 
  {
    boost::filesystem::path assetDir;
    int                     bounds[4];
    int                     blockSize;
    world_rotation_t        direction;
    boost::filesystem::path outputDir;
    
    int                     renderDimensions;
    int                     renderModes;
    int                     renderOrientations;

    dimension_t             currentDimension;
    
    bool                    statisticsOnly;
    bool                    saveMapStatistics;

    bool                    tiledOutput;
    int                     tileSize;
    bool                    verbose;
    boost::filesystem::path worldPath;
    bool                    zoomLevels[12];
  } config_t;

  /* helper functions */

  static inline std::string dimension2string(dimension_t dim) 
  { 
    switch (dim) 
    { 
      case DIMENSION_UNDEFINED: return "";

      case DIMENSION_OVERWORLD: return "Overworld";
      case DIMENSION_NETHER:    return "Nether";
      case DIMENSION_END:       return "End";
    } 
  }

  /*
   * this converts from ingame dimension integers to our dimension type
   */
  static inline dimension_t int2dimension(int dim)
  {
    switch (dim)
    {
      case -1: return DIMENSION_NETHER;
      case 0:  return DIMENSION_OVERWORLD;
      case 1:  return DIMENSION_END;
    }

    return DIMENSION_UNDEFINED;
  }

  static inline std::string dimension_storage(dimension_t dim)
  {
    switch (dim)
    {
      case DIMENSION_UNDEFINED: return "";

      case DIMENSION_OVERWORLD: return "region";
      case DIMENSION_NETHER:    return "DIM-1";
      case DIMENSION_END:       return "DIM1";
    }
  }
}

#endif
