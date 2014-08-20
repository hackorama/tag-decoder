#ifndef _CONFIG_H_INCLUDED
#define _CONFIG_H_INCLUDED

#include <string>

#include "common.h"
#include "pixmap.h"

class Config
{

public:
	Config();
	~Config();

	unsigned char *PIXBUF;
	bool *EDGE_MAP; 		//border map created in Threshold parsed in Border

	int THRESHOLD_WINDOW_SIZE; 	//Adapative thresholdng window size(lower the faster)
	int THRESHOLD_OFFSET;		//threshold offset adjustment
	int THRESHOLD_RGB_FACTOR;	//RGB range multiplication factor 

	int  PIXMAP_SCALE_SIZE;         //fix pixmap to this bounding box size 
	int  PIXMAP_MINIMUM_SCALE_SIZE; //minimum valid value for PIXMAP_SCALE_FACTOR
	bool PIXMAP_FAST_SCALE;         //scale by skipping(FAST) or by averaging(SLOW)
	bool PIXMAP_NATIVE_SCALE;  //scale using platform specific external libraray
	bool JPG_SCALE;			   //scale by 2/4/8 on IJG JPEG lib decompress 
	//NATIVE_SCALE requires no further scaling, JPG_SCALE may need further scaling

	int ANCHOR_BOX_FLEX_PERCENT;    //allowed flexibility for box width and height 
	int SHAPE_BOX_FLEX_PERCENT;	//allowed flexibility for box width and height

	int GRID_WIDTH;			//image width
	int GRID_HEIGHT;		//image height

	bool PESSIMISTIC_ROTATION;	//resizing the grid for rotated shapes

	string TAG_IMAGE_FILE; 		//image filename 
	Pixmap* DBGPIXMAP;

	int THREADS;

	bool CHECK_VISUAL_DEBUG();
	void setDebugPixmap(Pixmap* pixmap);
	bool checkArgs(int argc, char **argv);
	void freeEdgemap();
	void freePixbuf();

	bool DEBUG;
	bool VISUAL_DEBUG;
	bool ANCHOR_DEBUG;

	bool ARGS_OK;

	static const int MAX_ANCHORS;
	static const int MAX_SHAPES;
	static const bool PLATFORM_CPP;
	static const bool PLATFORM_CPP_MAGICK;
	static const bool PLATFORM_CPP_SYMBIAN;
	static const bool PLATFORM_CPP_SYMBIAN_S60;

	/*
	static final boolean PLATFORM_JAVA = true;
	static final boolean PLATFORM_JAVA_ME = false;
	static final boolean PLATFORM_JAVA_SE = true;
	static final boolean PLATFORM_JAVA_ANDROID = false;
	*/

};

#endif /* _CONFIG_H_INCLUDED */
