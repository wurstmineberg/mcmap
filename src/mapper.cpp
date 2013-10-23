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
    this->load();
    return 0;
  }

  void mapper::load()
  {
    fs::path level_dat = fs::path(config.worldPath) / "level.dat";
    
    nbt_node* test = nbt_parse_path(level_dat.string().c_str());
    cout << nbt_dump_ascii(test) << endl;
  }
}
