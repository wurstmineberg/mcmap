#include "texture.hpp"
#include "config.hpp"

using namespace mcmap;
namespace fs = boost::filesystem;


namespace mcmap
{
	texture::texture(boost::filesystem::path path, texture_rotation_t rotation)
	{
		if (config.verbose) cout << "New texture: " << path << endl;
	}

	texture::texture(boost::filesystem::path path)
	{
		texture(path, TEXTURE_ROTATION_0);
	}

	texture::texture(string identifier, texture_rotation_t rotation)
	{
		fs::path texture_file = config.assetDir;
		texture_file /= "minecraft/textures/";
		texture_file /= identifier.append(".png");
		texture(texture_file, rotation);
	}

	texture::texture(string identifier)
	{
		texture(identifier, TEXTURE_ROTATION_0);
	}

	boost::filesystem::path texture::get_path()
	{
		return this->path;
	}
}
