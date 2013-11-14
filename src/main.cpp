#include <iostream>
#include <fstream>

#include <zlib.h>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <json_spirit.h>

#include <cstdlib>

#include "config.hpp"
#include "mapper.hpp"

#include "item_metadata.hpp"

using namespace std;
using namespace mcmap;
using namespace log4cxx;

namespace fs = boost::filesystem;
namespace po = boost::program_options;

namespace js = json_spirit;

namespace mcmap
{
	config_t config;
  log4cxx::LoggerPtr logger = Logger::getLogger("mcmap");

	item_metadata_store metadata_store;
}

bool load_config();
void create_asset_dir();

int main(int argc, char **argv)
{
  fs::path logger_conf = fs::path(MCMAP_BASEPATH / "misc/log4cxx.properties");
  PropertyConfigurator::configure(LOG4CXX_FILE(logger_conf.string().c_str()));

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

  if (vm.count("version") == 1)
  {
    cout << version_string() << endl;
    exit(0);
  }

  if (config.verbose)
  {
    logger->setLevel(Level::getTrace());
  }

  // display simple usage output if no config was supplied
  if (!load_config())
  {
    cout << "usage:\n\t " << argv[0] << " [-vh]" << endl;
    return 0;
  }

  metadata_store.load_metadata();

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

  LOG4CXX_INFO(logger, "Loading config " << p);

  ifstream in(p.string().c_str());
  if (in.is_open())
  {
    // set defaults

    config.blockSize            = 16;
    config.renderDimensions     = DIMENSION_OVERWORLD;
    config.renderOrientations   = WORLD_ROTATION_NORTH_EAST;
    config.renderModes          = RENDER_MODE_ISOMETRIC;
    config.outputDir            = fs::current_path() / fs::path("output/");
    config.saveMapStatistics    = true;
    config.tiledOutput          = false;
    config.tileSize             = 128;
    
    config.bounds[0] = 0;
    config.bounds[1] = 0;
    config.bounds[2] = 0;
    config.bounds[3] = 0;

    config.zoomLevels[11] = true;

    bool success = true;

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
          LOG4CXX_ERROR(logger, "Block size has to be a multiple of 16.");
          success = false;
        }
      }

      if (name == "bounds")
      {
        js::Array bounds = value.get_array();
        if (bounds.size() != 4)
        {
          LOG4CXX_WARN(logger, "Bounds has to have 4 values. Defaulting to {0, 0, 0, 0}");
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
            
            if (dimension == "Overworld") config.renderDimensions |= DIMENSION_OVERWORLD;
            if (dimension == "Nether")    config.renderDimensions |= DIMENSION_NETHER;
            if (dimension == "End")       config.renderDimensions |= DIMENSION_END;
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

          if (orientation == "N")  config.renderOrientations |= WORLD_ROTATION_NORTH;
          if (orientation == "NE") config.renderOrientations |= WORLD_ROTATION_NORTH_EAST;
          if (orientation == "E")  config.renderOrientations |= WORLD_ROTATION_EAST;
          if (orientation == "SE") config.renderOrientations |= WORLD_ROTATION_SOUTH_EAST;
          if (orientation == "S")  config.renderOrientations |= WORLD_ROTATION_SOUTH;
          if (orientation == "SW") config.renderOrientations |= WORLD_ROTATION_SOUTH_WEST;
          if (orientation == "W")  config.renderOrientations |= WORLD_ROTATION_WEST;
          if (orientation == "NW") config.renderOrientations |= WORLD_ROTATION_NORTH_WEST;
        }
      }

      if (name == "renderModes")
      {
        config.renderModes = 0;

        js::Array renderModes = value.get_array();
        for (int i = 0; i < renderModes.size(); ++i)
        {
          string renderMode = renderModes[i].get_str();

          if (renderMode == "top")       config.renderModes |= RENDER_MODE_TOP;
          if (renderMode == "isometric") config.renderModes |= RENDER_MODE_ISOMETRIC;
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
          LOG4CXX_WARN(logger, "You've given more zoom levels than are allowed, will only use first 12.")

        // reset preset default
        config.zoomLevels[11] = false;

        // set
        for (int i = 0; i < 12; ++i) config.zoomLevels[i] = true;
      }
    }

    if (config.assetDir == "") create_asset_dir();

    return success;
  }
  
  return false;
}

/**
 * Try to locate a minecraft.jar and extract the assets into MCMAP_BASEPATH/assets
 *
 * Common minecraft paths:
 *
 * Linux: ~/.minecraft
 * OS X:  ~/Library/Application Support/minecraft/
 *
 * assets are in jar!/resources/assets/
 **/
void create_asset_dir()
{
  #ifndef __UNIX__
    LOG4CXX_INFO(logger, "mcmap cannot auto-locate your minecraft assets.");
    return;
  #else
    fs::path  cwd = fs::current_path();
    chdir(MCMAP_BASEPATH.string().c_str());

    fstream timestamp;
    int current_stamp = 0;

    if (!fs::exists("assets_timestamp"))
    {
      FILE *fp = fopen("assets_timestamp", "ab+");
      fclose(fp);
    }

    timestamp.open("assets_timestamp");

    timestamp.seekg(0, timestamp.end);
    int size = timestamp.tellg();

    if (size > 0) timestamp >> current_stamp;

    fs::path mc_versions = fs::path(getenv("HOME"));
    
    #ifdef __APPLE__
      // unix + apple => os x
      mc_versions /= "Library/Application Support/minecraft";
    #else
      mc_versions /= ".minecraft";
    #endif

    mc_versions /= "versions";

    fs::path latest;
    int latest_stamp;
    
    try 
    { 
      for (fs::directory_iterator it(mc_versions); it != fs::directory_iterator(); ++it)
      {
        int check_stamp = fs::last_write_time(*it);
        if (fs::is_directory(*it) && latest_stamp < check_stamp) 
        {
          latest_stamp = check_stamp;
          latest = *it;
        }
      }
    }
    catch(boost::filesystem::filesystem_error &ex)
    {
      LOG4CXX_ERROR(logger, ex.what());
      return;
    }

    latest /= latest.filename();

    if (latest_stamp > current_stamp) 
    {
      LOG4CXX_INFO(logger, "Extracting (updated) assets from " << latest.string() + ".jar");

      string cmd = "tar -xf \"" + latest.string() + ".jar\" assets";

      system(cmd.c_str());

      timestamp << latest_stamp;
    }

    timestamp.close();

    chdir(cwd.string().c_str());

    config.assetDir = MCMAP_BASEPATH / "assets";
  #endif
}
