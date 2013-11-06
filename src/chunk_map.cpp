#include "chunk_map.hpp"

#include <iostream>
#include <cstdlib>

using namespace std;

namespace mcmap
{
  chunk_map::chunk_map(void *chunk_data)
  {
    this->data = this->load(chunk_data);
  }  

  nbt_node *chunk_map::load(void *chunk_data)
  {
    // load the nbt data

    // actual data length in bytes
    char length[4];
    memcpy(length, chunk_data, 4);
    size_t l = (size_t)ntohl(*((int *)length));
    

    if ((char)*((char *)chunk_data + 4) == CHUNK_COMPRESSION_GZIP)
    {
      // used gzip, maybe write out some information about this chunk having
      // been externally modified, thus the chunk might be corrupted
    } 

    return nbt_parse_compressed((void *)((char *)chunk_data + 5), l);
  }

  void chunk_map::map()
  {
    
  }
}
