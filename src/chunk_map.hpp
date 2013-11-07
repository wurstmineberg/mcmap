#ifndef __chunk_map_hpp
#define __chunk_map_hpp

#include <boost/filesystem.hpp>
#include <nbt.h>

#define CHUNK_COMPRESSION_GZIP 1
#define CHUNK_COMPRESSION_ZLIB 2

namespace mcmap
{
  class chunk_map
  {
  public:
    chunk_map(void *chunk_data);

    void map();

  private:
    nbt_node *load(void *chunk_data);

    nbt_node *data;
    boost::filesystem::path tile_filename;

    int chunkX;
    int chunkZ;

    int biome[16][16];

    int num_entities;
  };
}

#endif
