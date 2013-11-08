#ifndef __png_reader_hpp
#define __png_reader_hpp

#include <boost/filesystem.hpp>
#include <png.h>

namespace fs = boost::filesystem;

namespace mcmap
{
	class png_reader
	{
	public:
		png_reader(fs::path *path);
		png_structp get_png_data();

	private:
		fs::path *path;
	};
}

#endif
