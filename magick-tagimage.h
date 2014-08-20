#ifndef _TAGIMAGE_H_INCLUDED
#define _TAGIMAGE_H_INCLUDED

//MSVC++ C4996 WARNING SUPPRESSION for strcpy to magick c API
#define _CRT_SECURE_NO_DEPRECATE 

#include <string>
#include <magick/api.h> /* MagickCore C API */
#include "common.h"

using namespace std;

class Tagimage
{

public:
	Tagimage(Config *config);
	~Tagimage();
	int  getPixel(int x, int y);
	int  getWidth();
	int  getHeight();
	bool isValid();
	int  maxRGB();

private:
	Image *image;
	Config *config;
	ExceptionInfo exception;
	int width, height;
	bool valid;
	static const int MAXRGB = 65535;
	void resizeImage();
	void resizeImage(int width, int height);
};

#endif /* _TAGIMAGE_H_INCLUDED */
