#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <json_spirit.h>

#include "config.hpp"
#include "mapper.hpp"

using namespace std;
using namespace mcmap;

namespace fs = boost::filesystem;
namespace po = boost::program_options;

mcmap::config_t mcmap::config;

bool load_config();

int main(int argc, char **argv)
{
  po::options_description input_options("options");
  input_options.add_options()
    ("help,h", "display this help")
    ("verbose,v", "enable verbose output")
    ("version", "display version information and quit");

  po::variables_map vm;
  po::store(po::command_line_parser(argc,argv).options(input_options).run(), vm);

  if (vm.count("help"))
  {
    cout << input_options << endl;
    return 0;  
  }

  config.verbose = (vm.count("verbose") == 1);

  if (config.verbose || vm.count("version") == 1)
  {
    cout << "This is mcmap v." 
         << MCMAP_VERSION.major 
         << "." 
         << MCMAP_VERSION.minor 
         << "." 
         << MCMAP_VERSION.patch 
         << endl;

    if (vm.count("version") == 1) exit(0);
  }

  // display simple usage output if no config was supplied
  if (!load_config())
  {
    cout << "usage:\n\t " << argv[0] << " [-vh]" << endl;
    return 0;
  }

  // if we got this far, everything should be fine (beware, Murphy is probably somewhere around here.)

  mapper *m = new mapper();
  return m->work();
}

bool load_config()
{
  // load json config
  fs::path p(fs::current_path());
  p /= "mcmap.json";

  if (!fs::exists(p)) return false;

  if (config.verbose) cout << "Using config " << p << endl;

  ifstream in(p.string().c_str());
  if (in.is_open())
  {
    config.blockSize            = 16;
    config.direction            = 0; // standard north
    config.renderEnd            = false;
    config.renderNether         = false;
    config.renderOverworld      = false,
    config.renderOverworldNight = false;
    config.outputDir            = fs::path("output/");
    config.saveMapStatistics    = true;
    config.tiledOutput          = false;
    config.tileSize             = 128;
    
    config.bounds[0] = 0;
    config.bounds[1] = 0;
    config.bounds[2] = 0;
    config.bounds[3] = 0;

    config.zoomLevels[11] = 1;

    json_spirit::Value cfg;
    json_spirit::read(in, cfg);

    json_spirit::Object o = cfg.get_obj();
    for (json_spirit::Object::size_type i = 0; i != o.size(); i++)
    {
      const json_spirit::Pair& pair = o[i];

      const string& name  = pair.name_;
      const json_spirit::Value& value = pair.value_;

      if (name == "blockSize")
      {
        config.blockSize = value.get_int();
      }

      if (name == "bounds")
      {
        // TODO: read array
        json_spirit::Array bounds = value.get_array();
      }

      if (name == "direction")
      {
        config.direction = value.get_int();
      }

      if (name == "outputDir")
      {
        config.outputDir = fs::path(value.get_str());
      }

      if (name == "renderEnd")
      {
        config.renderEnd = value.get_bool();
      }

      if (name == "renderNether")
      {
        config.renderNether = value.get_bool();
      }

      if (name == "renderOverworld")
      {
        config.renderOverworld = value.get_bool();
      }

      if (name == "renderOverworldNight")
      {
        config.renderOverworldNight = value.get_bool();
      }

      if (name == "saveMapStatistics")
      {
        config.saveMapStatistics = value.get_bool();
      }

      if (name == "tiledOutput")
      {
        config.tiledOutput = value.get_bool();
      }

      if (name == "tileSize")
      {
        config.tileSize = value.get_int();
      }

      if (name == "world")
      {
        config.worldPath = fs::path(value.get_str());
      }

      if (name == "zoomLevels")
      {
        // TODO: read array
        json_spirit::Array zoomLevels = value.get_array();
      }
    }

    // default to overworld if no render mode was selected
    if (!config.renderOverworld 
    &&  !config.renderOverworldNight 
    &&  !config.renderNether 
    &&  !config.renderEnd)
    {
      config.renderOverworld = true;
    }

    return true;
  }
  
  return false;
}
