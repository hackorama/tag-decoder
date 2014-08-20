#include "pixmap.h"

void libjpeg_error_exit_pixmap(j_common_ptr cinfo) {
    fprintf(stderr, "JPEG Error : " );
    (*cinfo->err->output_message) (cinfo);
    exit(1);
}

const int Pixmap::MAXRGB = 256;

Pixmap::Pixmap()
{
	r_buffer = NULL;
	g_buffer = NULL;
	b_buffer = NULL;
	width = 0;
	height = 0;
	ok = false;
	init();
}

Pixmap::Pixmap(string filename)
{
printf("foo\n");
    struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
    FILE * infile;
    JSAMPARRAY buffer;
    int row_stride;

	//strcpy(image_info->filename, filename.c_str());
    if ((infile = fopen(filename.c_str(), "rb")) == NULL) {
        fprintf(stderr, "can't open file\n");
        exit(1);
    }

	cinfo.err = jpeg_std_error(&jerr);
    jerr.error_exit = libjpeg_error_exit_pixmap;

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    (void) jpeg_read_header(&cinfo, TRUE);
    (void) jpeg_start_decompress(&cinfo);

    row_stride   = cinfo.output_width * cinfo.output_components;
    width  =  cinfo.output_width;
    height = cinfo.output_height;

    buffer = (*cinfo.mem->alloc_sarray)
        ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	int imageindex = 0;
	r_buffer = new long[width * height];
	g_buffer = new long[width * height];
	b_buffer = new long[width * height];
    while (cinfo.output_scanline < cinfo.output_height) {
//fprintf(stdout, "%d/%d ( %d x %d ) %d\n", cinfo.output_scanline, cinfo.output_height , width, height , row_stride);
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
        //(void) put_scanline(buffer[0], cinfo.output_width );
		for(int i=0; i<3*width; i+=3){ 
			r_buffer[imageindex] = (long) buffer[i];
			g_buffer[imageindex] = (long) buffer[i+1];
			b_buffer[imageindex] = (long) buffer[i+3];
			imageindex++;
		}
    }

    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
	debug = false;
	init();
}

Pixmap::~Pixmap()
{
	delete [] r_buffer;
	delete [] g_buffer;
	delete [] b_buffer;
}

void
Pixmap::init()
{
	IMGTYPE = "png";
	serial = 0 ;
	pen_r = MAXRGB;
	pen_g = 0;
	pen_b = 0;
	savePen();
	writeImage("foo.jpg");
}

void
Pixmap::resizePixmap(int _w, int _h)
{
	if(width == _w && height == _h) return;
	if(debug) cout << "pixmap resized to :" << width << ", " << height << endl;
}

void 
Pixmap::debugImage(string name, int i)
{
	writeImage(name, i);
}

void 
Pixmap::debugImage(string name)
{
	writeImage(name);
}

void 
Pixmap::writeImage(string name, int i)
{
	stringstream filename;
	string localserial = "" ;
	if( i < 10 )		    localserial = "00";
	else if( i < 100 && i > 9 ) localserial = "0";
	filename << name << "-" << localserial  << i ;
	writeImage(filename.str());
}
void 
Pixmap::writeImage(string name)
{
	serial++;

	stringstream filename;
	string prefix = "" ;
	if( serial < 10 )		      prefix = "00";
	else if( serial < 100 && serial > 9 ) prefix = "0";
	filename << prefix << serial << "-" << name << "." << IMGTYPE ;
	
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * outfile; 
	JSAMPROW row_pointer[1]; 
	int row_stride = 0; 

	cinfo.err = jpeg_std_error(&jerr);
    jerr.error_exit = libjpeg_error_exit_pixmap;
	jpeg_create_compress(&cinfo);

	if ((outfile = fopen(filename.str().c_str(), "wb")) == NULL) {
    	fprintf(stderr, "can't open %s\n", filename.str().c_str());
    	return;	
  	}

	jpeg_stdio_dest(&cinfo, outfile);

  cinfo.image_width = width;  
  cinfo.image_height = height;
  cinfo.input_components = 3;       
  cinfo.in_color_space = JCS_RGB;   
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, 100, TRUE );

  jpeg_start_compress(&cinfo, TRUE);

  row_stride = cinfo.image_width * cinfo.input_components ; 

  unsigned char  *rgb = new unsigned char[row_stride * height];
  int rgbi = 0;
  for( int i = 0; i < (width * height); i++){
	rgb[rgbi] = (unsigned char) r_buffer[i]; rgbi++;
	rgb[rgbi] = (unsigned char) g_buffer[i]; rgbi++;
	rgb[rgbi] = (unsigned char) b_buffer[i]; rgbi++;
  }
  rgbi = 0;
  for( int i = 0; i < (width * height); i++){
	if ( i < 50*width){
	rgb[rgbi] = 255; rgbi++;
	rgb[rgbi] = 0; rgbi++;
	rgb[rgbi] = 0; rgbi++;
	}else if(  i < 100*width && i >= 50*width ){
	rgb[rgbi] = 0; rgbi++;
	rgb[rgbi] = 255; rgbi++;
	rgb[rgbi] = 0; rgbi++;
	}else{
	rgb[rgbi] = 0; rgbi++;
	rgb[rgbi] = 0; rgbi++;
	rgb[rgbi] = 255; rgbi++;
	}
  }
  JSAMPLE * image_buffer = rgb;

  int next = 0;
  //while (cinfo.next_scanline < cinfo.image_height) {
  while (next < height) {
//fprintf(stdout, "%d/%dx%d [ %d x %d ] %d\n", next, cinfo.image_height , cinfo.image_width, width, height , row_stride);
    //row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
    row_pointer[0] = & image_buffer[next * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	next++;
    //(void) jpeg_write_scanlines(&cinfo, &scanbuffer, 1);
  }

  jpeg_finish_compress(&cinfo);
  fclose(outfile);
  jpeg_destroy_compress(&cinfo);

}

void
Pixmap::setDebug(bool flag)
{
	debug = flag;
}

int
Pixmap::getWidth()
{
	if( width < 0 ) width = 0;
	return width;
}

int
Pixmap::getHeight()
{
	if( height < 0 ) height = 0;
	return height;
}

bool
Pixmap::isValid()
{
	if( r_buffer == NULL || g_buffer == NULL  || b_buffer == NULL ) ok = false;
	return ok;
}

int
Pixmap::maxRGB()
{
	return MAXRGB;
}

bool
Pixmap::inRange(int x, int y)
{
	if( ! isValid() )  return false;
	if( ( x >= 0 &&  x < width ) && ( y >= 0 &&  y < height ) ) {
		return true;
	}
	return false;
}

void
Pixmap::switchColor( int r, int g, int b, int new_r, int new_g, int new_b )
{
	for(int i=0; i<width; i++){
		for(int j=0; j<height; j++){
			switchPixel( i, j, r, g, b, new_r, new_g, new_b);
		}
	}
}

Pixel
Pixmap::getPixelAt(int x, int y)
{
	int index = (y * width) + x ;
	pixel.red   = (int)r_buffer[index]; 
	pixel.green = (int)g_buffer[index];
	pixel.blue  = (int)b_buffer[index];
	return pixel;
}

bool
Pixmap::switchPixel( int x, int y, int r, int g, int b, 
					int new_r, int new_g, int new_b )
{
	if( ! inRange( x, y ) ) return true;
	Pixel pixel = getPixelAt(x, y);
	if( pixel.red == r &&  pixel.green == g &&  pixel.blue == b ){
		return setPixel( x, y, new_r, new_g, new_b );
	}
	return false;
}

int
Pixmap::getPixel( int x, int y ) 
{
	//if( ! inRange( x, y ) ) return 0;
	Pixel pixel = getPixelAt(x, y);
	return ( pixel.red + pixel.green + pixel.blue );
}

void
Pixmap::setPen( int r, int g, int b ) 
{
	pen_r = convertRGB(r);
	pen_g = convertRGB(g);
	pen_b = convertRGB(b);
}

void
Pixmap::savePen() 
{
	saved_pen_r = pen_r;
	saved_pen_g = pen_g;
	saved_pen_b = pen_b;
}

void
Pixmap::restorePen() 
{
	pen_r = saved_pen_r;
	pen_g = saved_pen_g;
	pen_b = saved_pen_g;
}

int
Pixmap::convertRGB( int x ) 
{
	if( x <= 0 || x > 256  ) return x;
	int factor = MAXRGB / 256;
	return x*factor;
}

bool
Pixmap::colorPixel( int x, int y ) // not black
{
	if( ! inRange( x, y ) ) return true;
	Pixel pixel = getPixelAt(x, y);
	int p = pixel.red + pixel.green + pixel.blue ;
	if( p > 0 && p < (3 * MAXRGB) ) return true;
	return false;
}

bool
Pixmap::darkPixel( int x, int y ) // not black
{
	if( ! inRange( x, y ) ) return true;
	Pixel pixel = getPixelAt(x, y);
	int p = pixel.red + pixel.green + pixel.blue ;
	if( p >= MAXRGB ) return true;
	return false;
}

bool
Pixmap::lightPixel( int x, int y ) // not black
{
	if( ! inRange( x, y ) ) return true;
	Pixel pixel = getPixelAt(x, y);
	int p = pixel.red + pixel.green + pixel.blue ;
	if( p < MAXRGB ) return true;
	return false;
}

bool
Pixmap::redPixel( int x, int y ) 
{
	if( ! inRange( x, y ) ) return false;
	Pixel pixel = getPixelAt(x, y);
	int p = pixel.red + pixel.green + pixel.blue ;
	if( pixel.red == MAXRGB && p == MAXRGB ) return true;
	return false;
}

bool
Pixmap::greenPixel( int x, int y ) 
{
	if( ! inRange( x, y ) ) return false;
	Pixel pixel = getPixelAt(x, y);
	int p = pixel.red + pixel.green + pixel.blue ;
	if( pixel.green == MAXRGB && p == MAXRGB ) return true;
	return false;
}

bool
Pixmap::bluePixel( int x, int y ) 
{
	if( ! inRange( x, y ) ) return false;
	Pixel pixel = getPixelAt(x, y);
	int p = pixel.red + pixel.green + pixel.blue ;
	if( pixel.blue == MAXRGB && p == MAXRGB ) return true;
	return false;
}

bool
Pixmap::whitePixel( int x, int y ) // not black
{
	if( ! inRange( x, y ) ) return true;
	Pixel pixel = getPixelAt(x, y);
	int p = pixel.red + pixel.green + pixel.blue ;
	if( p == (3 * MAXRGB) ) return true;
	return false;
}

bool
Pixmap::blackPixel( int x, int y ) // black 
{
	if( ! inRange( x, y ) ) return false;
	Pixel pixel = getPixelAt(x, y);
	int p = pixel.red + pixel.green + pixel.blue ;
	if( p ==  0 ) return true;
	return false;
}

bool
Pixmap::markPixels(int x, int y, int r, int g, int b, int border)
{

	setPixel(x-border, y-border, r, g, b);
	setPixel(x,   y-border, r, g, b);
	setPixel(x+border, y-border, r, g, b);

	setPixel(x-border, y, r, g, b);
	setPixel(x,   y, r, g, b);
	setPixel(x+border, y, r, g, b);

	setPixel(x-border, y+border, r, g, b);
	setPixel(x,   y+border, r, g, b);
	setPixel(x+border, y+border, r, g, b);

	return true;
}

bool
Pixmap::setPixel(int x, int y)
{
	if( ! inRange( x, y ) ) return false;
	return setPixel(x, y, pen_r, pen_g, pen_b);
}

bool
Pixmap::setPixel(int x, int y, int width)
{
	if( ! inRange( x, y ) ) return false;
	bool result = true;
	for(int i = 0; i < width*2; i++){
		result &= setPixel((x-width)+i, y);
	}
	for(int i = 0; i < width*2; i++){
		result &= setPixel(x, (y-width)+i);
	}
	return result;
}

bool
Pixmap::setPixel256RGB(int x, int y, int r, int g, int b)
{
	int factor = MAXRGB / 256 ;
	return setPixel(x, y, r*factor, g*factor, b*factor);
}

bool
Pixmap::setPixel(int x, int y, int r, int g, int b)
{
	if( ! inRange( x, y ) ) return false;
	int index = (y * width) + x ;
	r_buffer[index] = r;
	g_buffer[index] = g;
	b_buffer[index] = b;
	return true;
}

void
Pixmap::clearPixmap(int r, int g, int b)
{
	for(int i=0; i<width; i++){
		for(int j=0; j<height; j++){
			setPixel( i, j, r, g, b);
		}
	}
}

void
Pixmap::clearPixmap()
{
	clearPixmap(maxRGB(), maxRGB(), maxRGB());
}

void
Pixmap::clearPixmapWhite()
{
	clearPixmap(0, 0, 0);
}

void
Pixmap::markPoint(int x, int y, int radius)
{
	for( int i = 0; i < (2*radius); i++)
		for( int j = 0; j < (2*radius); j++)
			setPixel( (x-radius) + i, (y-radius) + j );
}

void
Pixmap::markPoint(int x, int y)
{
	setPixel(x,y);

	setPixel(x++,y);
	setPixel(x--,y);
	setPixel(x,y++);
	setPixel(x,y--);
	setPixel(x++,y++);
	setPixel(x++,y--);
	setPixel(x--,y++);
	setPixel(x--,y--);
}

void
Pixmap::markVLine(int x)
{
	markVLine(1, height-1, x);
}

void
Pixmap::markHLine(int y)
{
	markHLine(1, width-1, y);
}

void
Pixmap::markHLine(int _x1, int _x2, int y)
{
	int x1 = 0,  x2 = 0;
	if( _x1 < _x2 ) { 
		x1 = _x1 ; x2 = _x2 ;
	}else{
		x1 = _x2 ; x2 = _x1 ;
	}
	for(int x = x1; x <= x2; x++){
		setPixel(x, y);
	}
}

void
Pixmap::markVLine(int _y1, int _y2, int x)
{
	int y1 = 0,  y2 = 0;
	if( _y1 < _y2 ) { 
		y1 = _y1 ; y2 = _y2 ;
	}else{
		y1 = _y2 ; y2 = _y1 ;
	}
	for(int y = y1; y <= y2; y++){
		setPixel(x, y);
	}
}

