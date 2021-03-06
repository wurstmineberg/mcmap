#include "chunk_map.hpp"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <stdint.h>

#include "block_layer.hpp"

#define IN_LAYER(y, layer_y) (y > (layerY * 16) && y < ((layerY + 16) * 16))

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
    
    // biome and height map data
    nbt_node *biomes = nbt_find_by_name(this->data, "Biomes");
    nbt_node *height = nbt_find_by_name(this->data, "HeightMap");

    for (int i = 0; i < 255; i++)
    {
      int z = i % 16;
      int x = (i + 1) % 16;

      this->biome[x][z]  = biomes->payload.tag_byte_array.data[i];
      this->height[x][z] = height->payload.tag_int_array.data[i];
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

    vector<boost::thread *> threads;
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
      for (int i = 0; i < 4096; i++)
      {
        /*
        // NOTE: the coordinates are not really needed here but maybe we can verwurst them for some statistics
        int x,y,z;

        y = (i - (i % 3)) % 16;
        z = ((i - (i % 3)) + 1) % 16;
        x = ((i - (i % 3)) + 2) % 16;
        */

        nbt_node *block_search_node = nbt_find_by_name(layer_node, "Blocks");

        chunk_layer->blocks[i].id = (int)(block_search_node->payload.tag_byte_array.data[i]);

        block_search_node = nbt_find_by_name(layer_node, "Add");
        if (block_search_node)
        {
          char add = block_search_node->payload.tag_byte_array.data[i];
          chunk_layer->blocks[i].id += (i % 2 == 0) ? LO_NIBBLE(add << 8) : HI_NIBBLE(add << 8); 
        }

        block_search_node = nbt_find_by_name(layer_node, "Data");
        chunk_layer->blocks[i].data = this->calc_short(block_search_node, i);

        block_search_node = nbt_find_by_name(layer_node, "BlockLight");
        chunk_layer->blocklight[i] = this->calc_short(block_search_node, i);

        block_search_node = nbt_find_by_name(layer_node, "SkyLight");
        chunk_layer->skylight[i] = this->calc_short(block_search_node, i);
      }

      boost::thread th = boost::thread(&chunk_map::render_layer, this, chunk_layer);
      threads.push_back(&th);
    }

    // wait on layer processing to finish
    for (int i = 0; i < this->num_layers; i++) threads.at(i)->join();

    // TODO: assemble chunk
  }

  int chunk_map::entities_count()
  {
    return this->num_entities;
  }

  void chunk_map::render_layer(chunk_layer_t *chunk_layer)
  {
    if (config.renderModes & RENDER_MODE_TOP && chunk_layer->has_top_blocks)
    {
      // do north top render, apply requested orientations on render
    }

    if (config.renderModes & RENDER_MODE_ISOMETRIC)
    {
      // do isometric render for each orientation
    }
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
