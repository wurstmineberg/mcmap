#ifndef __mapper_hpp
#define __mapper_hpp

#include <nbt.h>

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
    void load();
  };
}

#endif
