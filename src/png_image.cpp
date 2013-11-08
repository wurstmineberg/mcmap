#include "png_reader.hpp"

using namespace std;

namespace fs = boost::filesystem;

namespace mcmap
{
	png_reader::png_reader(fs::path *path)
	{
    this->path = path;
    this->fp.open(path->string().c_str(), ios::binary);
	}

  png_reader::~png_reader()
  {
    this->fp.close();
  }

	png_structp png_reader::get_png_data()
	{
    if (!this->validate())
    {
      return NULL;
    }


  }

  bool png_reader::validate()
  {

  }
}
