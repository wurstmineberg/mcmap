#ifndef __texture_hpp
#define __texture_hpp

#include <string>
#include <boost/filesystem.hpp>

#include "png_image.hpp"

using namespace std;
namespace fs = boost::filesystem;

namespace mcmap
{
	typedef enum
	{
		TEXTURE_ROTATION_0 = 0,
		TEXTURE_ROTATION_90 = 1,
		TEXTURE_ROTATION_180 = 2,
		TEXTURE_ROTATION_270 = 3,
		TEXTURE_ROTATION_FLIP_0 = 4,
		TEXTURE_ROTATION_FLIP_90 = 5,
		TEXTURE_ROTATION_FLIP_180 = 6,
		TEXTURE_ROTATION_FLIP_270 = 7
	} texture_rotation_t;

	class texture
	{
	public:
		texture(string identifier, texture_rotation_t rotation);
		texture(string identifier);
		texture(fs::path, texture_rotation_t rotation = TEXTURE_ROTATION_0);

		~texture();

		boost::filesystem::path *get_path();
		png_structp get_png_data();

		string to_string();
	private:
		void init_with_path(fs::path filesystem_path, texture_rotation_t rotation);
		void init_with_identifier(string identifier, texture_rotation_t rotation);

		fs::path *path;
		texture_rotation_t rotation;
	};
}

#endif
