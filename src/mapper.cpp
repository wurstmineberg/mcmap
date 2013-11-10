#include "mapper.hpp"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fstream>

using namespace std;

namespace fs = boost::filesystem;
namespace js = json_spirit;

namespace mcmap
{
  mapper::mapper()
  {
    this->output = config.outputDir;
    
    if (fs::is_regular_file(this->output))
    {
      cerr << "Do you even documentation?" << endl;
      exit(1);
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
    // create and change into temp directory for map tiles
    fs::path p(this->output / "tiles_temp");
    fs::create_directory(p);
    chdir(p.string().c_str());

    if (config.renderDimensions & DIMENSION_OVERWORLD)
      this->dimensions.push_back(this->analyze_world(DIMENSION_OVERWORLD));

    if (config.renderDimensions & DIMENSION_NETHER)
      this->dimensions.push_back(this->analyze_world(DIMENSION_NETHER));

    if (config.renderDimensions & DIMENSION_END)
      this->dimensions.push_back(this->analyze_world(DIMENSION_END));

    if (!config.statisticsOnly)   this->map();
    if (config.saveMapStatistics) this->save_map_statistics();

    return 0;
  }

  dimension_data_t *mapper::analyze_world(dimension_t dim)
  {
    dimension_data_t *dimension_data = new dimension_data_t;

    fs::path regions_path = fs::path(config.worldPath) / dimension_storage(dim);

    dimension_data->name = dimension2string(dim);
    dimension_data->dimension   = dim;
    dimension_data->num_regions = 0;

    int max_x = 1,  max_z = 1;
    int min_x = -1, min_z = -1;

    for (fs::directory_iterator it(regions_path); it != fs::directory_iterator(); ++it)
    {
      // calculate region coordinates
      dimension_data->num_regions++;
      
      string regionIdent = it->path().stem().string().substr(2);
      
      size_t dot = regionIdent.find(".");
      
      string xStr = regionIdent.substr(0, dot);
      string zStr = regionIdent.substr(++dot);

      int x = atoi(xStr.c_str());
      int z = atoi(zStr.c_str());

      // check if the region is in the requested chunk boundary...
      bool in_chunk_boundary = false;

      if ((config.bounds[0] == 0) || (config.bounds[1] == 0)
      ||  (config.bounds[2] == 0) || (config.bounds[3] == 0)) 
      {
        // if any of the bounds is zero, all are assumed zero..
        in_chunk_boundary = true;
      } else
      {
        // else calculate the required regions
        int min_requiredX = config.bounds[3] >> 5; // SE
        int max_requiredX = config.bounds[0] >> 5; // NE

        int min_requiredZ = config.bounds[2] >> 5; // SW
        int max_requiredZ = config.bounds[1] >> 5; // NW

        if ((x < min_requiredX) || (x > max_requiredX)) in_chunk_boundary = false;
        if ((z < min_requiredZ) || (z > max_requiredZ)) in_chunk_boundary = false;

        in_chunk_boundary = true;
      }

      // if so, do stuffs with it
      if (in_chunk_boundary)
      {
        if (x > max_x) max_x = x;
        if (x < min_x) min_x = x;

        if (z > max_z) max_z = z;
        if (z < min_z) min_z = z;

        // cache region path and meta information
        region_t r = 
        { 
          x, 
          z, 
          fs::file_size(it->path()), 
          new region_map(fs::path(it->path()), x, z)
        };

        dimension_data->regions.push_back(r);
      }
    }

    dimension_data->max_x_extent = abs(min_x) + abs(max_x);
    dimension_data->max_z_extent = abs(min_z) + abs(max_z);

    return dimension_data;
  }

  void mapper::map()
  {
    for (std::vector<dimension_data_t *>::iterator it = this->dimensions.begin();
         it < this->dimensions.end(); ++it)
    {
      dimension_data_t *dim_data = *it;

      config.currentDimension = dim_data->dimension;
      
      // create and change into temp directory for dimension tiles
      fs::path p(dim_data->name);
      fs::create_directory(p);
      chdir(p.string().c_str());

      if (config.verbose) cout << "Mapping dimension " << dim_data->name << endl;

      for (std::vector<region_t>::iterator reg = dim_data->regions.begin(); 
           reg < dim_data->regions.end(); 
           ++reg)
      {
        region_t *current = &(*reg);
        current->map->map();
      }

      chdir("..");
    }

  }

  void mapper::save_map_statistics()
  {
    fs::path ld_path = fs::path(config.worldPath) / "level.dat";
    this->level_dat = nbt_parse_path(ld_path.string().c_str());
    
    js::Object statistics;
    nbt_node *search_node = nbt_find_by_name(this->level_dat, "LevelName");
    
    statistics.push_back(js::Pair("worldName", search_node->payload.tag_string));

    search_node = nbt_find_by_name(this->level_dat, "RandomSeed");
    statistics.push_back(js::Pair("worldSeed", search_node->payload.tag_long));

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

    statistics.push_back(js::Pair("currentTimeOfDay", current_time));
    statistics.push_back(js::Pair("numberOfDaysPlayed", days));

    search_node = nbt_find_by_name(this->level_dat, "raining");
    statistics.push_back(js::Pair("currentlyRaining", (bool)search_node->payload.tag_byte));

    search_node = nbt_find_by_name(this->level_dat, "thundering");
    statistics.push_back(js::Pair("isThunderstorm", (bool)search_node->payload.tag_byte));

    statistics.push_back(js::Pair("pois", this->pois()));


    // save json for main stats
    const char *filename = (this->output / "map_statistics.json").string().c_str();
    ofstream of(filename, ofstream::out);
    js::write(statistics, of, js::pretty_print);
    of.close();

    js::Object region_statistics;

    for (std::vector<dimension_data_t *>::iterator it = this->dimensions.begin();
         it < this->dimensions.end(); ++it)
    {
      js::Array dim;
      dim.push_back(this->dimension_data(*it));
      region_statistics.push_back(js::Pair("dimensions", dim));
    }
    

    const char *region_filename = (this->output / "region_statistics.json").string().c_str();
    ofstream of2(region_filename, ofstream::out);
    js::write(region_statistics, of2, js::pretty_print);
    of2.close();


    if (config.verbose)
      cout << "Saved statistics to " << this->output << endl;
  }

  js::Object mapper::pois()
  {
    js::Array spawn, player_positions;
    js::Object pois;
    
    nbt_node *search_node;

    search_node = nbt_find_by_name(this->level_dat, "SpawnX");
    spawn.push_back(search_node->payload.tag_int);

    search_node = nbt_find_by_name(this->level_dat, "SpawnY");
    spawn.push_back(search_node->payload.tag_int);

    search_node = nbt_find_by_name(this->level_dat, "SpawnZ");
    spawn.push_back(search_node->payload.tag_int);

    pois.push_back(js::Pair("spawn", spawn));

    // traverse through player files...
    fs::path players = fs::path(config.worldPath) / "players";

    for (fs::directory_iterator it(players) ; it != fs::directory_iterator(); ++it)
    {
      if (it->path().extension() == ".dat")
      {
        nbt_node *player_node = nbt_parse_path(it->path().string().c_str());
        js::Object player;
        player.push_back(js::Pair("name", it->path().stem().string()));

        /*
        // FIXME: this always segfaults and I don't get why

        js::Array playerSpawn;
        
        search_node = nbt_find_by_name(player_node, "SpawnX");
        playerSpawn.push_back(n->payload.tag_int);

        //playerSpawn.push_back(search_node->payload.tag_int);

        //search_node = nbt_find_by_name(player_node, "SpawnY");
        //playerSpawn.push_back(search_node->payload.tag_int);

        //search_node = nbt_find_by_name(player_node, "SpawnZ");
        //playerSpawn.push_back(search_node->payload.tag_int);

        player.push_back(js::Pair("spawn", playerSpawn));
        */

        // dimension, 0 = overworld, 1 = end, -1 = nether
        search_node = nbt_find_by_name(player_node, "Dimension");
        int dimension = search_node->payload.tag_int;

        player.push_back(js::Pair("dimension", dimension2string(int2dimension(dimension))));

        /*
        // TODO: current position
        js::Array pos;
        nbt_node *pos_node = nbt_find_by_name(player_node, "pos");
        struct nbt_list *pos_list = (struct nbt_list *)pos_node->payload.tag_list;
        */

        player_positions.push_back(player);
      }
    }

    pois.push_back(js::Pair("players", player_positions));

    return pois;
  }

  js::Object mapper::dimension_data(dimension_data_t *dimension)
  {
    js::Object obj;

    obj.push_back(js::Pair("name", dimension->name));
    
    obj.push_back(js::Pair("num_regions", dimension->num_regions));

    obj.push_back(js::Pair("max_x_extent", dimension->max_x_extent));
    obj.push_back(js::Pair("max_y_extent", dimension->max_z_extent));

    js::Array regions;
    float overall_saturation = 0;

    for (std::vector<region_t>::iterator it = dimension->regions.begin(); it < dimension->regions.end(); ++it)
    {
      region_t *current = &(*it);
      js::Object region;

      region.push_back(js::Pair("regionX", current->regionX));
      region.push_back(js::Pair("regionZ", current->regionZ));

      region.push_back(js::Pair("saturation", current->map->saturation()));

      overall_saturation += current->map->saturation();

      regions.push_back(region);
    }

    obj.push_back(js::Pair("regions", regions));

    obj.push_back(js::Pair("overallSaturation", overall_saturation / (float)dimension->num_regions));

    return obj;
  }
}
