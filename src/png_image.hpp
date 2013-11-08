#ifndef __png_image_hpp
#define __png_image_hpp

#include <iostream>
#include <fstream>

#include <zlib.h> // since 1.3, libpng does not autoinclude zlib anymore
#include <png.h>

#include <boost/filesystem.hpp>

namespace mcmap
{
  /**
   * Wrapper class for PNG handling.
   *
   * Image data is always expected in RGBA when writing 
   * or transformed to RGBA when reading.
   **/
	class png_image
	{
	public:
		png_image(boost::filesystem::path p);
    png_image(long width, long height);
    ~png_image();

	private:
    png_structp png_ptr;
    png_infop   info_ptr;
    png_bytep  *rowPtrs;
    

    long width;
    long height;

    void save(boost::filesystem::path p);
    void load();
    
    bool validate_stream();
    bool init_png_structs();

    std::fstream fp;
	};
}

#endif
