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
    ("verbose,v", "enable verbose output");

  po::options_description invisible_options("invisbles");
  invisible_options.add_options()
    ("world", po::value<string>(&config.worldPath), "The Minecraft world save you want to map.");

  po::options_description all("all");
  all.add(input_options).add(invisible_options);

  po::positional_options_description positionals;
  positionals.add("world", 1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc,argv).options(all).positional(positionals).run(), vm);

  if (vm.count("help"))
  {
    cout << input_options << endl;
    return 0;  
  }

  // display simple usage output if no world save was supplied
  if (!load_config() || config.worldPath.length() == 0)
  {
    cout << "usage:\n\t " << argv[0] << " [-vh] worldsave" << endl;
    return 0;
  }

  config.verbose = (vm.count("verbose") == 1);

  // if we got this far, everything should be fine (beware, Murphy is probably somewhere around here.)

  if (config.verbose)
  {
    cout << "This is mcmap v." 
         << MCMAP_VERSION.major 
         << "." 
         << MCMAP_VERSION.minor 
         << "." 
         << MCMAP_VERSION.patch 
         << endl;
  }

  mapper *m = new mapper();
}

bool load_config()
{
  // load json config
  fs::path p(fs::current_path());
  p /= "mcmap.json";

  if (!fs::exists(p)) return false;

  ifstream in(p.string().c_str());
  if (in.is_open())
  {
    json_spirit::Value cfg;
    json_spirit::read(in, cfg);

    json_spirit::Object o = cfg.get_obj();
    for (json_spirit::Object::size_type i = 0; i != o.size(); i++)
    {
      const json_spirit::Pair& pair = o[i];

      const string& name  = pair.name_;
      const json_spirit::Value& value = pair.value_;

      // TODO: parse full config

      if (name == "world")
      {
        config.worldPath = value.get_str();
      }

      /*if (name == "bounds")
      {
        config.worldPath = value.get_vec();
      }*/

      if (name == "output")
      {
        // TODO: sanity check directory
        config.outputDir = value.get_str();
      }
    }

    return true;
  }
  
  return false;
}
