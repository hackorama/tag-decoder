#ifndef _TRANSFORM_H_INCLUDED
#define _TRANSFORM_H_INCLUDED

// #define PTHREAD

#include "tagimage.h"
#include "pixmap.h"
#include "common.h"

#ifdef PTHREAD
#include <pthread.h>
#endif

#include <vector>
#include <algorithm>

using namespace std;

class Threshold
{
public:
	Threshold(Config *config, Tagimage *pixin);
	~Threshold();
	void setPixmap(Pixmap *pixmap);
	void computeEdgemapOpt(int size, int offset);
	void computeEdgemap(int size, int offset, int y1, int y2);
	void computeEdgemap();
	bool *getEdgeMap();
	void scheduleWork(int id);

private:
	Config   *config;
	Tagimage *tagimage;
	bool  *edgemap;
	bool  *ta;
	int   width, height;
	float scale;
	int   span;
	int   max_rgb;
	bool  multi_threaded;

	int  getPixel(int i, int j);
	void resolveScaling();
	void fillEdgemap();

	//debug only 
	bool pixdebug;
	Pixmap *dbgpixmap;

	void d_setPixelMarked(int i, int j);
	void d_setPixelBlank(int i, int j);
	void d_setPixelFilled(int i, int j);
	//debug only 
};

#endif /* _TRANSFORM_H_INCLUDED */
