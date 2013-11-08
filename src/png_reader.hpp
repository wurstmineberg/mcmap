#ifndef __png_image_hpp
#define __png_image_hpp

#include <iostream>
#include <fstream>

#include <zlib.h> // since 1.3, libpng does not autoinclude zlib anymore
#include <png.h>

#include <boost/filesystem.hpp>

namespace mcmap
{
	class png_image
	{
	public:
		png_image(boost::filesystem::path *path);
    ~png_image();
		png_structp get_png_data();

	private:
		boost::filesystem::path *path;
    std::fstream fp;
    
    bool validate();
	};
}

#endif
