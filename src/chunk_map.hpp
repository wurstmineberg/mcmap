#ifndef __chunk_map_hpp
#define __chunk_map_hpp

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

#include <nbt.h>

#include <png.h>

#include "block.hpp"

#define CHUNK_COMPRESSION_GZIP 1
#define CHUNK_COMPRESSION_ZLIB 2

namespace mcmap
{
  typedef struct chunk_layer
  {
    unsigned char y;

    block_info_t  blocks[4096];

    // TODO: implement lighting
    //unsigned char skylight[4096];
    //unsigned char blocklight[4096];
  } chunk_layer_t;

  class chunk_map
  {
  public:
    chunk_map(void *chunk_data);

    void map();

  private:
    nbt_node *load(void *chunk_data);
    void render_layer(chunk_layer_t *chunk_layer);

    nbt_node *data;
    boost::filesystem::path tile_filename;

    int chunkX;
    int chunkZ;

    int width;
    int height;

    // biome is hex, just remember that; coordinates are x, z
    char biome[16][16];

    int num_entities;
    int num_layers;

    chunk_layer_t *layers;

    unsigned char **png_rows;
  };
}

#endif
