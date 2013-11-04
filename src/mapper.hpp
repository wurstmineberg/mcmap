#ifndef __mapper_hpp
#define __mapper_hpp

#include <iostream>
#include <vector>

#include <boost/filesystem.hpp>
#include <nbt.h>
#include <json_spirit.h>

#include "config.hpp"

namespace mcmap
{
  typedef enum mapper_dimension_t 
  {
    DIMENSION_OVERWORLD,
    DIMENSION_NETHER,
    DIMENSION_END
  };

  typedef struct region
  {
    boost::filesystem::path regionPath;
    int regionX;
    int regionY;
    long filesize;
  } region_t;

  class mapper
  {
  public:
    mapper(mapper_dimension_t dim);
    ~mapper();
    int work();
  
    /* data */
  private:
    void analyze_world();
    void save_map_statistics();
    json_spirit::Object pois();

    nbt_node* level_dat;
    boost::filesystem::path output;
    
    int num_regions;
    int max_x_dimension;
    int max_y_dimension;

    std::vector<region_t> regions;
  };
}

#endif
