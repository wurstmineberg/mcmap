#include "png_image.hpp"

using namespace std;

namespace fs = boost::filesystem;


namespace mcmap
{
  // custom readers and writers for fstream
  void png_fstream_read(png_structp png_ptr, png_bytep data, png_size_t length);
  void png_fstream_write(void);

  png_image::png_image(fs::path p)
	{
    this->fp.open(p.string().c_str(), fstream::in | fstream::binary);
	}

  png_image::png_image(long width, long height)
  {
    this->width  = width;
    this->height = height;
  }

  png_image::~png_image()
  {
    delete[] (png_bytep)this->rows;
    delete[] this->data;

    //And don't forget to clean up the read and info structs !
    png_destroy_read_struct(&this->png_ptr, &this->info_ptr,(png_infopp)0);

    if (this->fp.is_open()) this->fp.close();
  }

	bool png_image::load()
	{
    // validate stream and init png structures
    if (!this->validate_stream() && !this->init_png_structs()) return false;


    return true;
  }

  bool png_image::validate_stream()
  {
    char *testbuf = (char *)malloc(sizeof(char) * 8);
    bool valid = false;
    
    this->fp.read(testbuf, 8);
    int sig_valid = png_sig_cmp((png_bytep)testbuf, 0, 8);
    valid = (this->fp.good() && sig_valid == 0);
    this->fp.seekg(0, this->fp.beg);

    free(testbuf);

    return valid;
  }

  bool png_image::init_png_structs()
  {
    this->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (this->png_ptr)
    {
      this->info_ptr = png_create_info_struct(png_ptr);

      if (!this->info_ptr)
      {
        png_destroy_read_struct(&this->png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        return false;
      }
    } else return false;

    if (setjmp(png_jmpbuf(this->png_ptr))) {
      png_destroy_read_struct(&this->png_ptr, &this->info_ptr, (png_infopp)NULL);
      
      if (this->rows != NULL) delete [] this->rows;
      if (this->data != NULL) delete [] this->data;

      cerr << "ERROR: An error occured while reading the PNG file" << endl;

      return false;
    }

    png_set_read_fn(this->png_ptr, (png_voidp)&this->fp, png_fstream_read);
    this->read_png_info();

    png_read_image(this->png_ptr, this->rows);

    return true;
  }

  /**
   *  this will read png info data and apply transformations to read 24bpp rgba if necessary
   *  also, all all remaining data structures and memory allocations will be made
   */
  void png_image::read_png_info()
  {
    png_read_info(this->png_ptr, this->info_ptr);

    this->width  = png_get_image_width(this->png_ptr, this->info_ptr);
    this->height = png_get_image_height(this->png_ptr, this->info_ptr);

    //bits per CHANNEL! note: not per pixel!
    png_uint_32 bitdepth   = png_get_bit_depth(this->png_ptr, this->info_ptr);
    //Number of channels
    png_uint_32 channels   = png_get_channels(this->png_ptr, this->info_ptr);
    //Color type. (RGB, RGBA, Luminance, luminance alpha... palette... etc)
    png_uint_32 color_type = png_get_color_type(this->png_ptr, this->info_ptr);

    if (png_get_valid(this->png_ptr, this->info_ptr, PNG_INFO_tRNS)) 
    {
      channels++;
      png_set_tRNS_to_alpha(this->png_ptr);
    }

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(this->png_ptr);

    // i know, weirdest function name, but this actually scales to 8bit
    if (bitdepth == 16) 
    {
      bitdepth = 8;
      png_set_scale_16(this->png_ptr);
    }

    this->rows = new png_bytep[this->height];
    this->data = new unsigned char[this->width * this->height * bitdepth * channels / 8];

    int stride = this->width * bitdepth * channels / 8;

    // preset row pointers
    for (int i = 0; i < this->height; i++)
      this->rows[i] = (png_bytep)(this->data + i * stride);
  }

  png_bytep png_image::get_row(int i)
  {
    return this->rows[i];
  }

  png_bytep *png_image::get_rows()
  {
    return this->rows;
  }

  /* helpers */

  void png_fstream_read(png_structp png_ptr, png_bytep data, png_size_t length)
  {
    png_voidp a = png_get_io_ptr(png_ptr);
    ((fstream *)a)->read((char *)data, length);
  }

  void png_fstream_write(void)
  {

  }
}
