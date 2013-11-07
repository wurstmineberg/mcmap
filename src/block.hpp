#ifndef __block_hpp
#define __block_hpp

#include <vector>
#include <iostream>

#include <boost/filesystem.hpp>

#include <png.h>

#include <json_spirit.h>

typedef enum
{
  BLOCK_FACE_TOP,
  BLOCK_FACE_NORTH,
  BLOCK_FACE_WEST,
  BLOCK_FACE_SOUTH,
  BLOCK_FACE_EAST,
  BLOCK_FACE_BOTTOM
} block_face_t;

typedef enum
{
	BLOCK_ROTATION_0 = 0,
	BLOCK_ROTATION_90 = 1,
	BLOCK_ROTATION_180 = 2,
	BLOCK_ROTATION_270 = 3,
	BLOCK_ROTATION_FLIP_0 = 4,
	BLOCK_ROTATION_FLIP_90 = 5,
	BLOCK_ROTATION_FLIP_180 = 6,
	BLOCK_ROTATION_FLIP_270 = 7
} block_rotation_t;

class block
{
public:
  block(int block_id, int data);
  ~block();

  /* data */
private:
  int required_texture_size();

  /*
    looks up the base path (without attached facename and extension)
    of a texture for a given block id, data value and orientation
  */
  boost::filesystem::path texture_basepath(block_face_t block_face);
  block_rotation_t texture_rotation(block_face_t block_face);


  double side;
  double diagonal;
};

class block_cache
{
public:
  block_cache();
  ~block_cache();

  /* data */
};

#endif
