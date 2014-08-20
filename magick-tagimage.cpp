#include "tagimage.h"

Tagimage::Tagimage(Config *_config)
{
	config = _config;
	ImageInfo *image_info = CloneImageInfo((ImageInfo *) NULL);
	GetExceptionInfo(&exception);
	strcpy(image_info->filename, config->TAG_IMAGE_FILE.c_str());
	image = ReadImage(image_info,&exception);
	DestroyImageInfo(image_info);
	if (image == (Image *) NULL){
		width = 0;
		height = 0;
		valid = false;
		cerr << "Failed loading image file: " << config->TAG_IMAGE_FILE  << endl;
	}else{
		width  = image->columns;
		height =  image->rows;
		valid  = true;
		if(config->DEBUG) cout  << config->TAG_IMAGE_FILE 
			<< " (" << width << "x" << height << ")" << endl;
	}
	if(config->PIXMAP_NATIVE_SCALE) resizeImage();
}

Tagimage::~Tagimage()
{
	DestroyImage(image);
	DestroyExceptionInfo(&exception);
}

int
Tagimage::getPixel( int x, int y ) 
{
	PixelPacket pixel = GetOnePixel( image, x, y);
	return ( pixel.red + pixel.green + pixel.blue );
}

int
Tagimage::getWidth()
{
	if( width < 0 ) width = 0;
	return width;
}

int
Tagimage::getHeight()
{
	if( height < 0 ) height = 0;
	return height;
}

bool
Tagimage::isValid()
{
	if( image == NULL ) valid = false;
	return valid;
}

int
Tagimage::maxRGB()
{
	return MAXRGB;
}

void
Tagimage::resizeImage()
{
	float boxsize = (float)config->PIXMAP_SCALE_SIZE;
	if( boxsize <= config->THRESHOLD_WINDOW_SIZE ) return;
	if( width <= boxsize && height <= boxsize ) return ;
	float scale = width > height ? (float)width/boxsize  :  (float)height/boxsize ;
	resizeImage((int)((float)width/scale), (int)((float)height/scale));
}

void
Tagimage::resizeImage(int _width, int _height)
{
	/*
	Image *resized_image = ResizeImage(image, (int)((float)width/scale),
	(int)((float)height/scale), LanczosFilter, 1.0, &exception);
	*/
	Image *resized_image = ScaleImage(image, _width, _height, &exception);
	DestroyImage(image);
	image = resized_image;
	width = image->columns;
	height =  image->rows;
	config->GRID_WIDTH  = width;
	config->GRID_HEIGHT = height;
	if(config->DEBUG) cout << "tag resized to :" << width << ", " << height << endl;
}
