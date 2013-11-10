#include "region_map.hpp"

#include <cstdlib>
#include <stdint.h>

#include <iomanip>

using namespace std;

namespace fs = boost::filesystem;

namespace mcmap
{
  region_map::region_map(fs::path region_filename, int regionX, int regionZ)
  {
    this->filename = region_filename;
    
    this->context.open(this->filename.string().c_str(), ios::binary);

    this->regionX = regionX;
    this->regionZ = regionZ;

    this->analyze();

    if (config.verbose) cout << "[region] " << regionX << "." << regionZ << endl;
  }

  region_map::~region_map()
  {
    this->context.close();

    for (std::vector<r_chunk_info_t *>::iterator it = this->chunk_infos.begin(); 
         it < this->chunk_infos.end(); ++it)
    {
      delete[] *it;
    }
  }

  void region_map::analyze()
  {
    // read location and modification data
    //int num_sectors = (fs::file_size(this->filename) / SECTOR_BYTES) - 2; // first two sectors are location and time info
    
    this->context.seekg(0, ios::beg);

    // need only a four byte buffer for all of the analyzing
    char *buffer = (char *)malloc(sizeof(char) * 4);

    // read region header
    for (int i = 0; i < SECTOR_INTS; i++)
    {
      r_chunk_info_t *chunk_info = new r_chunk_info_t;
      
      this->context.read(buffer, 4);

      int location = ntohl(*((int *)buffer));

      chunk_info->offset = location >> 8;
      chunk_info->size   = location & 0xff;

      if (chunk_info->offset > 0)
      {
        // chunks with offset == 0 are not generated yet and thus don't need to be mapped

        this->chunk_infos.push_back(chunk_info);
      }
    }

    for (int i = SECTOR_INTS; i < SECTOR_INTS * 2; i++)
    {
      int idx = i - SECTOR_INTS;
      
      if (idx < this->chunk_infos.size())
      {
        r_chunk_info_t *chunk_info = this->chunk_infos.at(i - SECTOR_INTS);

        this->context.read(buffer, 4); 
        chunk_info->timestamp = ntohl(*((int *)buffer));
      } else break;
    }

    /*
      we could read all the chunk headers now to get their exact length and
      compression scheme (which should be zlib all the time, otherwise
      someone messed with the map in a way they should not have),
      but in order to consume as little memory as possible, this is
      done on the fly while processing the chunks in the mapping stage.
    */

    free(buffer);
  }

  float region_map::saturation()
  {
    return (float)this->chunk_infos.size() / (float)CHUNKS_PER_REGION;
  }

  void region_map::map()
  {
    // create and change into directory for region
    char *a = (char *)malloc(sizeof(char) * 12);
    sprintf(a, "%d/%d", this->regionX, this->regionZ);

    char *regDirs = (char *)malloc(sizeof(char) * strlen(a));
    memcpy(regDirs, a, strlen(a));

    free(a);

    fs::path p(fs::current_path() / regDirs);
    //fs::create_directories(p);

    chdir(p.string().c_str());

    free(regDirs);

    // FIXME: this should be multi-threaded
    // FIXME: actually maybe not, since chunk rendering is multi threaded

    for (std::vector<r_chunk_info_t *>::iterator it = this->chunk_infos.begin(); it < this->chunk_infos.end(); ++it)
    {
      // allocate memory for the current chunk
      void *chunk_data = (void *)malloc((*it)->size * SECTOR_BYTES);
      
      // read chunk data into memory section
      this->context.seekg((*it)->offset * SECTOR_BYTES);
      this->context.read((char *)chunk_data, (*it)->size * SECTOR_BYTES);

      chunk_map cm(chunk_data);
      cm.map();

      free(chunk_data);
    }

    chdir("..");
  }
}
