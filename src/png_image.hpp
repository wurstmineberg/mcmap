#ifndef __png_reader_hpp
#define __png_reader_hpp

#include <iostream>
#include <fstream>

#include <zlib.h> // since 1.3, libpng does not autoinclude zlib anymore
#include <png.h>

#include <boost/filesystem.hpp>

namespace mcmap
{
	class png_reader
	{
	public:
		png_reader(boost::filesystem::path *path);
		png_structp get_png_data();

	private:
		boost::filesystem::path *path;
    std::fstream fp;
    
    bool validate();
	};
}

#endif
