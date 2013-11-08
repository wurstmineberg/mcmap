#include "png_image.hpp"

using namespace std;

namespace fs = boost::filesystem;

namespace mcmap
{
	png_image::png_image(fs::path *path)
	{
    this->path = path;
    this->fp.open(path->string().c_str(), ios::binary);
	}

  png_image::~png_image()
  {
    this->fp.close();
  }

	png_structp png_image::get_png_data()
	{
    if (!this->validate())
    {
      return NULL;
    }


  }

  bool png_image::validate()
  {

  }
}
