#include "chunk_map.hpp"

#include <cstdlib>

namespace mcmap
{
  chunk_map::chunk_map(void *chunk_data)
  {
    this->data = chunk_data;
  }  

  chunk_map::~chunk_map()
  {
    free(data);
  }

  void chunk_map::map()
  {
    
  }
}
