#include "texture.hpp"
#include "config.hpp"

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

namespace mcmap
{
	void texture::init_with_path(fs::path filesystem_path, texture_rotation_t rotation)
	{
		this->path = new fs::path(filesystem_path.string());
		this->rotation = rotation;
		LOG4CXX_INFO(logger, "New texture: " << this->to_string());
	}

	void texture::init_with_identifier(string identifier, texture_rotation_t rotation)
	{
		if (identifier.compare("") == 0)
		{
			this->path = NULL;
		} else {
			fs::path filesystem_path = config.assetDir;
			filesystem_path /= "minecraft/textures/";
			filesystem_path /= identifier.append(".png");
			this->init_with_path(filesystem_path, rotation);
		}
	}

	texture::texture(fs::path filesystem_path, texture_rotation_t rotation)
	{
		this->init_with_path(filesystem_path, rotation);
	}

	texture::texture(string identifier, texture_rotation_t rotation)
	{
		this->init_with_identifier(identifier, rotation);
	}

	texture::texture(string identifier)
	{
		size_t last_at_sign = identifier.find_last_of("@");
		if (last_at_sign == string::npos)
		{
			this->init_with_identifier(identifier, TEXTURE_ROTATION_0);
		} else {
			string name = identifier.substr(0, last_at_sign);
			string rotation_string = identifier.substr(last_at_sign + 1, identifier.length());
			unsigned int rotation_int = boost::lexical_cast<unsigned int>(rotation_string);
			if (rotation_int <= 7)
			{
				texture_rotation_t rotation = static_cast<texture_rotation_t>(rotation_int);
				this->init_with_identifier(name, rotation);
			} else {
				LOG4CXX_WARN(logger, "Texture with name: \"" << name << "\" has invalid rotation value: " << rotation_string << ". Defaulting to no rotation. Please fix items.json. ");
				this->init_with_identifier(name, TEXTURE_ROTATION_0);
			}
		}
	}

	texture::~texture()
	{
		delete this->path;
	}

	boost::filesystem::path *texture::get_path()
	{
		return this->path;
	}

	string texture::to_string()
	{
		stringstream stream;
		stream << "<Texture: \"" << *this->path << "\" Rotation: " << boost::format("%i") % (int)this->rotation << ">";
		return stream.str();
	}

	png_image *texture::get_image()
	{
		png_image *image = new png_image(*this->path);
		return image;
	}
}
