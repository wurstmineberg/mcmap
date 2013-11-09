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
  class mapper
  {
  public:
    mapper();
    ~mapper();
    int work();
  
    /* data */
  private:
    dimension_data_t *analyze_world(dimension_t dim);
    void map(dimension_t dim);
    dimension_data_t *get_dimension(dimension_t dim);
    
    void save_map_statistics();
    
    json_spirit::Object pois();
    json_spirit::Object dimension_data(dimension_t dim);

    nbt_node* level_dat;
    boost::filesystem::path output;
    
    dimension_data_t *overworld;
    dimension_data_t *nether;
    dimension_data_t *end;
  };
}

#endif
