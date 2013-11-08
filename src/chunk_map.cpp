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

    // calculate width and height based on config
    // FIXME: these values are probably wrong.
    this->width  = 15 * config.blockSize;
    this->height = 255 * (config.blockSize / 2) + config.blockSize;
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
      // FIXME: these coordinates are very possibly very wrong
      int z = i % 16, x = (i / 16);
      this->biome[z][x] = search_node->payload.tag_byte_array.data[i];
    }

    // number of entities
    /*
      This might be a useful statistic to have in terms of figuring out lag spikes and stuffs.
    */
    search_node = nbt_find_by_name(this->data, "Entities");
    this->num_entities = list_length(&search_node->payload.tag_list->entry);

    // actual block data
    search_node = nbt_find_by_name(this->data, "Sections");
    this->num_layers = list_length(&search_node->payload.tag_list->entry);

    boost::thread *threads = (boost::thread *)malloc(sizeof(boost::thread) * this->num_layers);
    this->layers = (chunk_layer_t *)malloc(sizeof(chunk_layer_t) * this->num_layers);

    for (int i = 0; i< this->num_layers; i++)
    {
      nbt_node *layer_node = nbt_list_item(search_node, i);
      nbt_node *layer_search_node;

      // read the layer data into some nicer structure
      chunk_layer_t *chunk_layer = &this->layers[i];

      // layer Y
      layer_search_node = nbt_find_by_name(layer_node, "Y");
      chunk_layer->y = (unsigned char)layer_search_node->payload.tag_byte;

      // block data arrays

      /**
       * block data is stored in yzx 
       * => (y * 16 + z) * 16 + x
       **/
      int x,y,z;
      for (int i = 0; i < 4096; i++)
      {
        // NOTE: the coordinates are not really needed here but maybe we can verwurs the
        y = (i - (i % 3)) % 16;
        z = ((i - (i % 3)) + 1) % 16;
        x = ((i - (i % 3)) + 2) % 16;

        nbt_node *block_search_node = nbt_find_by_name(layer_node, "Blocks");

        // TODO: implement support for full range of block_ids
        chunk_layer->blocks[i].block_id = (int)(block_search_node->payload.tag_byte_array.data[i]);

        block_search_node = nbt_find_by_name(layer_node, "Data");
        chunk_layer->blocks[i].data_value = this->calc_short(block_search_node, i);

        block_search_node = nbt_find_by_name(layer_node, "BlockLight");
        chunk_layer->blocklight[i] = this->calc_short(block_search_node, i);

        block_search_node = nbt_find_by_name(layer_node, "SkyLight");
        chunk_layer->skylight[i] = this->calc_short(block_search_node, i);
      }

      threads[i] = boost::thread(&chunk_map::render_layer, this, chunk_layer);
    }

    // wait on layer processing to finish
    for (int i = 0; i < this->num_layers; i++) threads[i].join();

    // TODO: assemble chunk

    #ifdef DEBUG
    exit(1);
    #endif
  }

  void chunk_map::render_layer(chunk_layer_t *chunk_layer)
  {
    // TODO: implement
  }

  char chunk_map::calc_short(nbt_node *byte_array_node, int i)
  {
    char s;
    
    if (i % 2 == 0)
    {
      // data is low nibble on current index
      s = LO_NIBBLE(byte_array_node->payload.tag_byte_array.data[i]);
    } else
    {
      // data is high nibble on previous index
      s = HI_NIBBLE(byte_array_node->payload.tag_byte_array.data[(i != 4095) ? i-1 : i]);
    }

    return s;
  }
}
