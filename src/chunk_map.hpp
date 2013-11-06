#ifndef __chunk_map_hpp
#define __chunk_map_hpp

namespace mcmap
{
  class chunk_map
  {
  public:
    chunk_map(chunk_info_t *info);

  private:
    chunk_info_t *info;
  };
}

#endif
