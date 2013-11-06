#ifndef __mapper_hpp
#define __mapper_hpp

#include <iostream>
#include <vector>

#include <boost/filesystem.hpp>
#include <nbt.h>
#include <json_spirit.h>

#include "config.hpp"

#include "region_map.hpp"

namespace mcmap
{
  typedef enum mapper_dimension 
  {
    DIMENSION_OVERWORLD = 0,
    DIMENSION_NETHER    = -1,
    DIMENSION_END       = 1
  } mapper_dimension_t;

  typedef struct region
  {
    int regionX;
    int regionY;
    long filesize;
    region_map *map;
  } region_t;

  typedef struct dimension
  {
    std::string name;

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
    void map(mapper_dimension_t dim);
    dimension_t *get_dimension(mapper_dimension_t dim);
    
    void save_map_statistics();
    
    json_spirit::Object pois();
    json_spirit::Object dimension_data(mapper_dimension_t dim);

    nbt_node* level_dat;
    boost::filesystem::path output;
    
    dimension_t *overworld;
    dimension_t *nether;
    dimension_t *end;
  };
}

#endif
