#ifndef __mapper_hpp
#define __mapper_hpp

#include <nbt.h>

#include "config.hpp"

namespace mcmap
{
  /* save some statistics off of level.dat, might be of use. */
  struct map_statistics
  {
    bool weatherRain;
    bool weatherThunderstorm;
    bool hasStructures;

    int  spawnCoordinates;
    long seed;
    long timeOfDay;
    long numberOfDaysPlayed;

  };

  class mapper
  {
  public:
    mapper();
    ~mapper();
    int work();
  
    /* data */
  private:
    void save_map_statistics();

    nbt_node* level_dat;
    int num_regions;
  };
}

#endif
