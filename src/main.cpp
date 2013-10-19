#include <iostream>

#include <boost/program_options.hpp>
#include <json_spirit.h>


#include "config.hpp"

using namespace std;
using namespace mcmap;

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
}

bool load_config()
{
  // load json config
  
  return false;
}
