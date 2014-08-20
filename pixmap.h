#ifndef _PIXMAP_H_INCLUDED
#define _PIXMAP_H_INCLUDED

//MSVC++ C4996 WARNING SUPPRESSION for strcpy to Magick c API
#define _CRT_SECURE_NO_DEPRECATE 

#include <iostream>
#include <string>
#include <sstream>
extern "C" { //extern for MingW only, GNU and MSVC++ are fine
    #include <jpeglib.h> /* IJG JPEG LIBRARAY */
    #include <jerror.h>  /* IJG JPEG LIBRARAY */
}

using namespace std;


/* scracth class for any quick utility methods for debugging purpse 
 * used only for visual debugging never used in production
 * no benefit optimizing or cleaning up code for elegence
 */

struct Pixel{
	int red;
	int green;
	int blue;
};

class Pixmap
{

public:
	Pixmap();
	Pixmap(string filename);
	~Pixmap();
	void init();
	void writeImage(string filename);
	void writeImage(string filename, int i);
	void debugImage(string filename);
	void debugImage(string filename, int i);
	int  getWidth();
	int  getHeight();
	bool isValid();
	int  maxRGB();
	bool greenPixel(int x, int y); 
	bool redPixel(int x, int y); 
	bool bluePixel(int x, int y); 
	bool whitePixel(int x, int y); // == 3 * max_rgb
	bool blackPixel(int x, int y); // == 0
	bool colorPixel(int x, int y); // !white || !black
	bool darkPixel(int x, int y);  // >= max_rgb
	bool lightPixel(int x, int y); // < max_rgb 
	int  getPixel(int x, int y);
	bool setPixel(int x, int y, int r, int g, int b);
	bool setPixel256RGB(int x, int y, int r, int g, int b);
	bool setPixel(int x, int y);
	bool setPixel(int x, int y, int width);
	bool markPixels(int x, int y, int r, int g, int b, int border);
	void markHLine(int x1, int x2, int y);
	void markHLine(int y);
	void markVLine(int y1, int y2, int x);
	void markVLine(int x);
	void markPoint(int x, int y);
	void markPoint(int x, int y, int radius);
	bool switchPixel(int x, int y, int r, int g, int b,
			int new_r, int new_g, int new_b);
	void switchColor(int r, int g, int b,
			int new_r, int new_g, int new_b);

	void setPen(int r, int g, int b);
	void savePen();
	void restorePen();
	void clearPixmap();
	void clearPixmapWhite();
	void clearPixmap(int r, int g, int b);
	void setDebug(bool flag);
	void resizePixmap(int w, int h);

private:
	long* r_buffer;
	long* g_buffer;
	long* b_buffer;
	Pixel pixel;
	static const int MAXRGB;
	string IMGTYPE;
	int width, height;
	int pen_r, pen_g, pen_b;
	int saved_pen_r, saved_pen_g, saved_pen_b;
	int serial;
	bool ok;
	bool debug;
	bool inRange(int x, int y);
	int convertRGB(int x);
	Pixel getPixelAt(int x, int y);
};


#endif /* _PIXMAP_H_INCLUDED */
