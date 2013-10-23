#include "mapper.hpp"

#include <iostream>

#include <boost/filesystem.hpp>

using namespace std;

namespace fs = boost::filesystem;

namespace mcmap
{
  mapper::mapper()
  {
    
  }

  mapper::~mapper()
  {

  }

  int mapper::work()
  {
    if (config.saveMapStatistics) this->save_map_statistics();

    return 0;
  }

  void mapper::save_map_statistics()
  {
    // load level.dat
    fs::path ld_path = fs::path(config.worldPath) / "level.dat";
    this->level_dat = nbt_parse_path(ld_path.string().c_str());
    
  }
}
