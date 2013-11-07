#ifndef __chunk_map_hpp
#define __chunk_map_hpp

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

#include <nbt.h>

#include <png.h>

#define CHUNK_COMPRESSION_GZIP 1
#define CHUNK_COMPRESSION_ZLIB 2

namespace mcmap
{
  typedef struct block
  {
    char data_value;
    int  block_id;
  } block_t;

  typedef struct chunk_layer
  {
    unsigned char y;

    // since we don't have to worry about compression at this point,
    // these are in "normal" x,y,z order

    block_t       blocks[16][16][16];
    unsigned char skylight[16][16][16];
    unsigned char blocklight[16][16][16];
  } chunk_layer_t;

  class chunk_map
  {
  public:
    chunk_map(void *chunk_data);

    void map();

  private:
    nbt_node *load(void *chunk_data);
    void renderLayer(chunk_layer_t *chunk_layer);

    nbt_node *data;
    boost::filesystem::path tile_filename;

    int chunkX;
    int chunkZ;

    // biome is hex, just remember that
    char biome[16][16];

    //png_byte_p layers[16]; // might not all be used

    int num_entities;
    int num_layers;

    chunk_layer_t *layers;
  };
}

#endif
