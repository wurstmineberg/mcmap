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
  typedef enum mapper_dimension 
  {
    DIMENSION_OVERWORLD,
    DIMENSION_NETHER,
    DIMENSION_END
  } mapper_dimension_t;

  typedef struct region
  {
    boost::filesystem::path regionPath;
    int regionX;
    int regionY;
    long filesize;
  } region_t;

  typedef struct dimension
  {
    int num_regions;
    int max_x_extent;
    int max_y_extent;

    std::vector<region_t> regions;    
  } dimension_t;

  class mapper
  {
  public:
    mapper();
    ~mapper();
    int work();
  
    /* data */
  private:
    dimension_t *analyze_world(mapper_dimension_t dim);
    void save_map_statistics();
    json_spirit::Object pois();

    nbt_node* level_dat;
    boost::filesystem::path output;
    
    dimension_t *overworld;
    dimension_t *nether;
    dimension_t *end;
  };
}

#endif
