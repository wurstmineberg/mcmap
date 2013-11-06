#ifndef __chunk_map_hpp
#define __chunk_map_hpp

#define CHUNK_COMPRESSION_GZIP 1
#define CHUNK_COMPRESSION_ZLIB 2

namespace mcmap
{
  class chunk_map
  {
  public:
    chunk_map(void *chunk_data);
    ~chunk_map();

    void map();

  private:
    void *data;
  };
}

#endif
