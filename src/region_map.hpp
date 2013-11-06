#ifndef __region_map_hpp
#define __region_map_hpp

#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>

#include <nbt.h>

#include "chunk_map.hpp"

#define CHUNKS_PER_REGION 1024

#define SECTOR_BYTES 4096
#define SECTOR_INTS  SECTOR_BYTES / 4

namespace mcmap
{
  typedef struct r_chunk_info
  {
    int offset;
    int size;
    
    int timestamp;
  } r_chunk_info_t;

  class region_map
  {
  public:
    region_map(boost::filesystem::path region_filename);
    ~region_map();

    float saturation();

  private:
    void analyze();

    boost::filesystem::path filename;
    std::vector<r_chunk_info_t *> chunk_infos;
    std::vector<chunk_map>        chunk_maps;

    std::ifstream context;
  };
}

#endif
