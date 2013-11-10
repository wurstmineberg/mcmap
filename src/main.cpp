#include <iostream>
#include <fstream>

#include <zlib.h>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <json_spirit.h>

#include "config.hpp"
#include "mapper.hpp"

#include "item_metadata.hpp"

using namespace std;
using namespace mcmap;

namespace fs = boost::filesystem;
namespace po = boost::program_options;

namespace js = json_spirit;

namespace mcmap
{
	config_t config;
	item_metadata_store metadata_store;
}

bool load_config();
void discover_asset_dir();

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
         << "-"
         << MCMAP_VERSION.hash
         << endl;

    if (vm.count("version") == 1) exit(0);
  }

  // display simple usage output if no config was supplied
  if (!load_config())
  {
    cout << "usage:\n\t " << argv[0] << " [-vh]" << endl;
    return 0;
  }

  metadata_store = item_metadata_store();

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
    // set defaults

    config.blockSize            = 16;
    config.renderDimensions     = DIMENSION_OVERWORLD;
    config.renderOrientations   = WORLD_ROTATION_NORTH_EAST;
    config.renderModes          = RENDER_MODE_ISOMETRIC;
    config.outputDir            = fs::path("output/");
    config.saveMapStatistics    = true;
    config.tiledOutput          = false;
    config.tileSize             = 128;
    
    config.bounds[0] = 0;
    config.bounds[1] = 0;
    config.bounds[2] = 0;
    config.bounds[3] = 0;

    config.zoomLevels[11] = true;

    js::Value cfg;
    js::read(in, cfg);

    js::Object o = cfg.get_obj();
    for (js::Object::size_type i = 0; i != o.size(); i++)
    {
      const js::Pair& pair = o[i];

      const string& name  = pair.name_;
      const js::Value& value = pair.value_;

      if (name == "assetDir")
      {
        config.assetDir = fs::path(value.get_str());
      }

      if (name == "blockSize")
      {
        int blockSize = value.get_int();

        if (blockSize % 16 == 0)
        {
          config.blockSize = blockSize;  
        } else
        {
          cerr << "Block size has to be a multiple of 16." << endl;
        }
      }

      if (name == "bounds")
      {
        js::Array bounds = value.get_array();
        if (bounds.size() != 4)
        {
          cerr << "Bounds has to have 4 values. Defaulting to {0, 0, 0, 0}" << endl;
        } else
        {
          for (int i = 0; i < 4; ++i) config.bounds[i] = bounds[i].get_int();
        }
      }

      if (name == "renderDimensions")
      {
        // reset
        config.renderDimensions = 0;

        js::Array renderDimensions = value.get_array();
        for (int i = 0; i < renderDimensions.size(); ++i)
        {
          try
          {
            string dimension = renderDimensions[i].get_str();
            
            if (dimension == "Overworld") config.renderDimensions &= DIMENSION_OVERWORLD;
            if (dimension == "Nether")    config.renderDimensions &= DIMENSION_NETHER;
            if (dimension == "End")       config.renderDimensions &= DIMENSION_END;
          } catch(exception& e)
          {
            // this is horribly bad error management but the only other allowed value would be int
            int d = renderDimensions[i].get_int();
            if (d > 0 && d < 5) config.renderDimensions &= d;
          }
        }
      }

      if (name == "renderOrientations")
      {
        config.renderOrientations = 0;

        js::Array renderOrientations = value.get_array();
        for (int i = 0; i < renderOrientations.size(); ++i)
        {
          string orientation = renderOrientations[i].get_str();

          if (orientation == "N")  config.renderOrientations &= WORLD_ROTATION_NORTH;
          if (orientation == "NE") config.renderOrientations &= WORLD_ROTATION_NORTH_EAST;
          if (orientation == "E")  config.renderOrientations &= WORLD_ROTATION_EAST;
          if (orientation == "SE") config.renderOrientations &= WORLD_ROTATION_SOUTH_EAST;
          if (orientation == "S")  config.renderOrientations &= WORLD_ROTATION_SOUTH;
          if (orientation == "SW") config.renderOrientations &= WORLD_ROTATION_SOUTH_WEST;
          if (orientation == "W")  config.renderOrientations &= WORLD_ROTATION_WEST;
          if (orientation == "NW") config.renderOrientations &= WORLD_ROTATION_NORTH_WEST;
        }
      }

      if (name == "renderModes")
      {
        config.renderModes = 0;

        js::Array renderModes = value.get_array();
        for (int i = 0; i < renderModes.size(); ++i)
        {
          string renderMode = renderModes[i].get_str();

          if (renderMode == "top")       config.renderModes &= RENDER_MODE_TOP;
          if (renderMode == "isometric") config.renderModes &= RENDER_MODE_ISOMETRIC;
        }
      }

      if (name == "outputDir")
      {
        config.outputDir = fs::path(value.get_str());
      }

      if (name == "saveMapStatistics")
      {
        config.saveMapStatistics = value.get_bool();
      }

      if (name == "statisticsOnly")
      {
        config.statisticsOnly = value.get_bool();
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
        js::Array zoomLevels = value.get_array();
        
        if (zoomLevels.size() > 12 && config.verbose)
        {
          cerr << "You've given more zoom levels than are allowed, will only use first 12." << endl;
        }

        // reset preset default
        config.zoomLevels[11] = false;

        // set
        for (int i = 0; i < 12; ++i) config.zoomLevels[i] = true;
      }
    }

    if (!fs::is_directory(config.assetDir)) discover_asset_dir();

    return true;
  }
  
  return false;
}


void discover_asset_dir()
{

}
