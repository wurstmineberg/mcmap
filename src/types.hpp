#ifndef __types_hpp
#define __types_hpp

namespace mcmap
{
  /* basic data types */
  typedef struct 
  {
    int major;
    int minor;
    int patch;
    char hash[10];
  } version_t; 

  typedef enum
  {
    WORLD_ROTATION_SOUTH_EAST = 0,
    WORLD_ROTATION_NORTH_EAST = 1,
    WORLD_ROTATION_NORTH_WEST = 2,
    WORLD_ROTATION_SOUTH_WEST = 3
  } world_rotation_t;

  typedef enum
  {
    RENDER_MODE_TOP,
    RENDER_MODE_ISOMETRIC
  } render_mode_t;

  typedef enum
  {
    DIMENSION_OVERWORLD = 0,
    DIMENSION_NETHER    = -1,
    DIMENSION_END       = 1
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

  typedef struct 
  {
    boost::filesystem::path assetDir;
    int                     bounds[4];
    int                     blockSize;
    world_rotation_t        direction;
    boost::filesystem::path outputDir;
    int                     renderDimension;
    render_mode_t           renderMode;
    bool                    renderEnd;
    bool                    renderNether;
    bool                    renderOverworld;
    bool                    renderOverworldNight;
    bool                    saveMapStatistics;
    bool                    tiledOutput;
    int                     tileSize;
    bool                    verbose;
    boost::filesystem::path worldPath;
    bool                    zoomLevels[12];
  } config_t;

  static inline std::string dimension2string(dimension_t dim) 
  { 
    switch (dim) 
    { 
      case DIMENSION_OVERWORLD: return "Overworld";
      case DIMENSION_NETHER:    return "Nether";
      case DIMENSION_END:       return "End";
    } 
  }

  static inline std::string dimension_storage(dimension_t dim)
  {
    switch (dim)
    {
      case DIMENSION_OVERWORLD: return "region";
      case DIMENSION_NETHER:    return "DIM-1";
      case DIMENSION_END:       return "DIM1";
    }
  }
}

#endif
