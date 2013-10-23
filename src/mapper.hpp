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
  class mapper
  {
  public:
    mapper();
    ~mapper();
    int work();
  
    /* data */
  private:
    void save_map_statistics();
    json_spirit::Object pois();

    nbt_node* level_dat;
    boost::filesystem::path output;
  };
}

#endif
