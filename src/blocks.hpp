#ifndef __blocks_hpp
#define __blocks_hpp

#include <map>
#include <string>

/**
 * map block ids to their texture basenames, whereas the basename is 
 * the name minus all face and state information.
 * 
 * for better code readability, this is done in two steps, atop of the file,
 * all blocks are given descriptive names, then those names are mapped to strings
 **/

 #define BLOCK_AIR 0
 #define BLOCK_STONE 1
 #define BLOCK_GRASS  2
 #define BLOCK_DIRT 3
 #define BLOCK_COBBLESTONE 4
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_
 #define BLOCK_

const static std::map<int, std::string> block_map =
{
  {BLOCK_STONE, "stone"}
};

#endif
