#ifndef __tile_hpp
#define __tile_hpp

#include <vector>
#include <iostream>

#include <boost/filesystem.hpp>

#include <png.h>

#include <json_spirit.h>

enum tile_face
{
  TILE_FACE_TOP,
  TILE_FACE_SIDE,
  TILE_FACE_FRONT
};

class tile
{
public:
  tile();
  ~tile();

  /* data */
private:
  int required_texture_size();

  /*
    looks up the base path (without attached facename and extension)
    of a texture for a given block id.
  */
  boost::filesystem::path texture_basepath(int block_id);

  double side;
  double diagonal;
};

class tile_cache
{
public:
  tile_cache();
  ~tile_cache();

  /* data */
};

#endif
