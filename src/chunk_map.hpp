#ifndef __chunk_map_hpp
#define __chunk_map_hpp

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

#include <nbt.h>

#include <png.h>

#include "block.hpp"

#define CHUNK_COMPRESSION_GZIP 1
#define CHUNK_COMPRESSION_ZLIB 2

#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)
#define LO_NIBBLE(b) ((b) & 0x0F)

namespace mcmap
{
  class chunk_map
  {
  public:
    chunk_map(void *chunk_data);

    void map();

  private:
    nbt_node *load(void *chunk_data);
    void render_layer(chunk_layer_t *chunk_layer);
    char calc_short(nbt_node *byte_array_node, int i);

    nbt_node *data;
    boost::filesystem::path tile_filename;

    int chunkX;
    int chunkZ;

    int width;
    int height;

    // biome is hex, just remember that; coordinates are x, z
    char biome[256];

    int num_entities;
    int num_layers;

    chunk_layer_t *layers;
  };
}

#endif
