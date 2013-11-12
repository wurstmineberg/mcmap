#ifndef __mapper_hpp
#define __mapper_hpp

#include <iostream>
#include <vector>
#include <map>

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
    void map(void);
    
    void save_map_statistics();
    
    json_spirit::Object pois();
    json_spirit::Object player_data(std::string name, nbt_node *player_node);
    json_spirit::Object dimension_data(dimension_data_t *dimension);

    nbt_node* level_dat;
    boost::filesystem::path output;

    std::vector<dimension_data_t *> dimensions;
  };
}

#endif
