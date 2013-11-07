#include "chunk_map.hpp"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <stdint.h>

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
    nbt_node *search_node;

    // chunk x,z
    search_node = nbt_find_by_name(this->data, "xPos");
    this->chunkX = search_node->payload.tag_int;

    search_node = nbt_find_by_name(this->data, "zPos");
    this->chunkZ = search_node->payload.tag_int;
    
    // biome data
    search_node = nbt_find_by_name(this->data, "Biomes");
    for (int i = 0; i < search_node->payload.tag_byte_array.length; i++)
    {
      int x = i % 16, z = (i / 16);
      this->biome[x][z] = (int)(search_node->payload.tag_byte_array.data)[i];
    }

    // number of entities
    /*
      This might be a useful statistic to have in terms of figuring out lag spikes and stuffs.
    */
    search_node = nbt_find_by_name(this->data, "Entities");
    this->num_entities = list_length(&search_node->payload.tag_list->entry);
  }
}
