#include "png_image.hpp"

using namespace std;

namespace fs = boost::filesystem;


namespace mcmap
{
  // custom readers and writers for fstream
  void png_fstream_read(png_structp pngPtr, png_bytep data, png_size_t length);
  void png_fstream_write(void);

  png_image::png_image(fs::path p)
	{
    this->fp.open(path->string().c_str(), ios::read | ios::binary);
	}

  png_image(long width, long height)
  {

  }

  png_image::~png_image()
  {
    if (this->fp.is_open()) this->fp.close();
  }

	bool png_image::load()
	{
    // validate stream and init png structures
    if (!this->validate() && !this->init_png_structs()) return false;


    return true;
  }

  bool png_image::validate_stream()
  {
    char *testbuf = (char *)malloc(sizeof(char) * 8);
    bool valid = false;
    
    this->fp.read(testbuf, 8);
    valid = (this->fp.good() && png_sig_cmp(&testbuf, 0, 8));
    this->fp.seekg(0, is.beg);

    free(testbuf);

    return valid;
  }

  bool png_image::init_png_structs()
  {
    this->png_ptr = png_create_read_struct(PNG_LIBPNG_VERSION_STRING, NULL, NULL, NULL);

    if (this->png_ptr)
    {
      this->info_ptr = png_create_info_struct(png_ptr);

      if (!this->info_ptr)
      {
        png_destroy_read_struct(&this->png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        return false;
      }
    } else return false;

    if (setjmp(png_jmpbuf(this->pngPtr))) {
      png_destroy_read_struct(&this->pngPtr, &this->infoPtr, (png_infopp)NULL);
      
      if (this->rowPtrs != NULL) delete [] this->rowPtrs;

      cerr << "ERROR: An error occured while reading the PNG file" << endl;

      return false;
    }

    return true;
  }

  void png_fstream_read(png_structp pngPtr, png_bytep data, png_size_t length)
  {

  }

  void png_fstream_write(void)
  {

  }
}
