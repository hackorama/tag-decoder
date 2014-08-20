#ifndef _TAGIMAGE_H_INCLUDED
#define _TAGIMAGE_H_INCLUDED

// MSVC++ C4996 WARNING SUPPRESSION for strcpy to Magick c API
#define _CRT_SECURE_NO_DEPRECATE 

#include <string>
#include "common.h"
extern "C" { //extern for MingW only, GNU and MSVC++ are fine
	#include <jpeglib.h> /* IJG JPEG LIBRARAY */
	#include <jerror.h>  /* IJG JPEG LIBRARAY */
}

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
	int  COLORS;

private:
	unsigned char* buffer;
	Config *config;
	int  width, height;
	bool valid;
	static const int MAXRGB;
	void processScanLine(unsigned char buffer[], int width);
};

#endif /* _TAGIMAGE_H_INCLUDED */
