#ifndef _PATTERN_H_INCLUDED
#define _PATTERN_H_INCLUDED

#include <math.h> 
#include "shape.h"
#include "matrix.h"
#include "pixmap.h"
#include "common.h"

#define TOP_LEFT 1
#define TOP_RIGHT 2
#define BOT_LEFT 3
#define BOT_RIGHT 4

#define SIDE 1
#define BELOW 2
#define ACROSS 3

#define TILT_THRESHOLD 5

using namespace std;

class Pattern
{

public:
	Pattern(Config *config, Shape *shapes, int nshapes,  Shape *anchor);
	~Pattern();

	void findCode(int* tag);
	bool findTilt();  
	bool _findTilt();  
	int  findAngle(int x1, int y1, int x2, int y2, int orientation);  
	void rotateShapes(); 

	//debug only
	void d_printPattern();
	//debug only

private:
	Config *config;
	Shape *shapes;
	Shape *anchor;
	int nshapes;
	int code_pivot_x, code_pivot_y;  //corner of the code defined by anchor
	int group_size;      //growing code group size to identofy the group blocks within
	int starting_group_size; //original code group size 
	int anchor_at;	     //defaults at top left, but can be at any corner for rotated images
	int anchor_tilt;     //for tilt correction
	int anchor_offset;
	int codeblock[12];
	int code[12];
	int center_x, center_y; 	    //center of the image ( used for rotateShapes )
	int rotate_delta_x, rotate_delta_y; //grid resize after rotate, delta used in rotateShape()

	bool idGroup(int id);
	bool idGroup(int id, int delta);
	bool idGroup(int x, int y, int gid, int delta);
	void idBlock(int minx, int miny, int maxx, int maxy, int gid, int i);
	int  locateBlock(int minx, int miny, int maxx, int maxy, int i);
	void idBlock(int x, int y, int gid, int i, int groupsize_delta);
	int  locateBlock(int x, int y, int i, int groupsize_delta);
	int  getGroupx(int i);
	int  getGroupy(int i);
	void locateAnchor();
	bool findPattern();
	bool findBlocks();
	int  matchPattern(int i);
	bool validPattern();
	void printCodeBlock();
	void resizeGroup(int delta);
	void computeRotatedGrid(int angle);
	void finalPattern(int* tag);

	//debug only
	Pixmap *pixmap;
	bool debug;
	bool pixdebug;

	void d_debugShapes();
	void d_writeShapes();
	void d_writeShapes(string filename);
	//debug only
};

#endif /* _PATTERN_H_INCLUDED */
