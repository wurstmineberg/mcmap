#include "mapper.hpp"

#include <boost/regex.hpp>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fstream>

using namespace std;

namespace fs = boost::filesystem;

namespace mcmap
{
  mapper::mapper()
  {
    this->output = fs::path(config.outputDir);
    
    if (fs::is_regular_file(this->output))
    {
      cerr << "Do you even documentation?" << endl;
    }

    if (!fs::exists(this->output))
    {
      fs::create_directory(this->output);
    }
  }

  mapper::~mapper()
  {

  }

  int mapper::work()
  {
    this->analyze_world();
    if (config.saveMapStatistics) this->save_map_statistics();

    return 0;
  }

  void mapper::analyze_world()
  {
    // TODO: do this dimension independent
    fs::path regions_path = fs::path(config.worldPath) / "region";

    this->num_regions = 0;

    int max_x = 1,  max_y = 1;
    int min_x = -1, min_y = -1;

    for (fs::directory_iterator it(regions_path); it != fs::directory_iterator(); ++it)
    {
      // calculate region coordinates
      this->num_regions++;
      
      string regionIdent = it->path().stem().string().substr(2);
      
      size_t dot = regionIdent.find(".");
      
      string xStr = regionIdent.substr(0, dot);
      string yStr = regionIdent.substr(++dot);

      int x = atoi(xStr.c_str());
      int y = atoi(yStr.c_str());

      if (x > max_x) max_x = x;
      if (x < min_x) min_x = x;

      if (y > max_y) max_y = y;
      if (y < min_y) min_y = y;

      // cache region path
      region_t r = {fs::path(it->path()), x, y, fs::file_size(it->path())};
      this->regions.push_back(r);
    }

    this->max_x_dimension = abs(min_x) + abs(max_x);
    this->max_y_dimension = abs(min_y) + abs(max_y);
  }

  void mapper::save_map_statistics()
  {
    fs::path ld_path = fs::path(config.worldPath) / "level.dat";
    this->level_dat = nbt_parse_path(ld_path.string().c_str());
    
    json_spirit::Object statistics;
    nbt_node *search_node = nbt_find_by_name(this->level_dat, "LevelName");
    
    statistics.push_back(json_spirit::Pair("worldName", search_node->payload.tag_string));

    search_node = nbt_find_by_name(this->level_dat, "RandomSeed");
    statistics.push_back(json_spirit::Pair("worldSeed", search_node->payload.tag_long));

    // Some notes on time in minecraft...
    //
    // - Time is given in ticks, 20 ticks = 1 s
    // - One minecraft day = 24000 ticks
    // - Sunrise is at 22800, Sunset at at 13200
    // - Noon is at 6000 and Midnight (moon at peak) is at 18000.

    search_node = nbt_find_by_name(this->level_dat, "Time");
    long age = search_node->payload.tag_long;
    int days = (int)(age / 24000);
    int current_time = (int)(age % 24000);

    statistics.push_back(json_spirit::Pair("currentTimeOfDay", current_time));
    statistics.push_back(json_spirit::Pair("numberOfDaysPlayed", days));

    search_node = nbt_find_by_name(this->level_dat, "raining");
    statistics.push_back(json_spirit::Pair("currentlyRaining", (bool)search_node->payload.tag_byte));

    search_node = nbt_find_by_name(this->level_dat, "thundering");
    statistics.push_back(json_spirit::Pair("isThunderstorm", (bool)search_node->payload.tag_byte));

    statistics.push_back(json_spirit::Pair("pois", this->pois()));



    // save json
    const char *filename = (this->output / "map_statistics.json").string().c_str();
    ofstream of(filename, ofstream::out);
    json_spirit::write(statistics, of, json_spirit::pretty_print);
  }

  json_spirit::Object mapper::pois()
  {
    json_spirit::Array spawn, player_positions;
    json_spirit::Object pois;
    
    nbt_node *search_node;

    search_node = nbt_find_by_name(this->level_dat, "SpawnX");
    spawn.push_back(search_node->payload.tag_int);

    search_node = nbt_find_by_name(this->level_dat, "SpawnY");
    spawn.push_back(search_node->payload.tag_int);

    search_node = nbt_find_by_name(this->level_dat, "SpawnZ");
    spawn.push_back(search_node->payload.tag_int);

    pois.push_back(json_spirit::Pair("spawn", spawn));

    // traverse through player files...
    fs::path players = fs::path(config.worldPath) / "players";

    for (fs::directory_iterator it(players) ; it != fs::directory_iterator(); ++it)
    {
      if (it->path().extension() == ".dat")
      {
        nbt_node *player_node = nbt_parse_path(it->path().string().c_str());
        json_spirit::Object player;
        player.push_back(json_spirit::Pair("name", it->path().stem().string()));

        /*
        // FIXME: this always segfaults and I don't get why

        json_spirit::Array playerSpawn;
        
        search_node = nbt_find_by_name(player_node, "SpawnX");
        playerSpawn.push_back(n->payload.tag_int);

        //playerSpawn.push_back(search_node->payload.tag_int);

        //search_node = nbt_find_by_name(player_node, "SpawnY");
        //playerSpawn.push_back(search_node->payload.tag_int);

        //search_node = nbt_find_by_name(player_node, "SpawnZ");
        //playerSpawn.push_back(search_node->payload.tag_int);

        player.push_back(json_spirit::Pair("spawn", playerSpawn));
        */

        // dimension, 0 = overworld, 1 = end, -1 = nether
        search_node = nbt_find_by_name(player_node, "Dimension");
        int dimension = search_node->payload.tag_int;
        string dimensionName;

        switch (dimension)
        {
          case -1: dimensionName = "Nether"; break;
          case  0: dimensionName = "Overworld"; break;
          case  1: dimensionName = "End"; break;
        }

        player.push_back(json_spirit::Pair("dimension", dimensionName));

        /*
        // TODO: current position
        json_spirit::Array pos;
        nbt_node *pos_node = nbt_find_by_name(player_node, "pos");
        struct nbt_list *pos_list = (struct nbt_list *)pos_node->payload.tag_list;
        */

        player_positions.push_back(player);
      }
    }

    pois.push_back(json_spirit::Pair("players", player_positions));

    return pois;
  }
}
