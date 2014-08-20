#ifndef _SHAPE_H_INCLUDED
#define _SHAPE_H_INCLUDED

#define THRESHOLD 8 

#define FIRST  1 
#define NUVO 2 

#include <vector>
#include <iomanip>
#include <map>
#include <algorithm>
#include <math.h>


#include "pixmap.h"
#include "common.h"

using namespace std;

/* 
* uses duplicated pixel storage (x,y) and (y,x) on two multi maps 
* for faster/easier cross-lookups of x of y and y of x 
*/
class Shape
{

public:
	Shape();
	Shape(Config *config);
	~Shape();

	void add(int x, int y, int mapcount);
	void setBounds(int min_x, int min_y, int max_x, int max_y);
	void setCenter(int cx, int cy);
	void setGrid(int w, int h);
	void setValues(vector<int> xmap, vector<int> ymap, int mapcount);
	void copyValues(int *xmap, int *ymap, int mapcount);
	void setWidthValues(int *widths_holder, int *mids, int min, int max, bool reset);
	void setHeightValues(int *heights_holder, int *mids, int min, int max, bool reset);
	void copyWidthValues(int *widths, int *mids, int min, int max);
	void copyHeightValues(int *heights, int *mids, int min, int max);
	int* getWidthValues();
	int* getHeightValues();
	int* getWMidpointValues();
	int* getHMidpointValues();
	int* getymap();
	int* getxmap();
	int  getmapcount();
	int  matchPattern();
	void setConfig(Config *config);
	//bounding box based sizes
	int  size();
	int  getWidth();
	int  getHeight();
	int  length();
	int  getcx();
	int  getcy();
	//bounding box based sizes
	//non-bounding box based corner finding
	int  getx(int y);
	int  gety(int x);
	//non-bounding box based corner finding
	int  getgridw(); //FIXME - Pass it to Pattren/Border directly
	int  getgridh(); //FIXME - Pass it to Pattren/Border directly
	int  getminx();
	int  getmaxx();
	int  getminy();
	int  getmaxy();
	bool isAnchor();
	bool isAnchorLike();
	void rotateShape(int d);
	//void rotateShape(int cx, int cy, int angle);
	void rotateShape(int cx, int cy, int dx, int dy, int angle);
	void setRotated(bool flag);
	bool isRotated();
	int  findTilt();
	bool isWithin(int x, int y, int s);
	bool isWithin(int x, int y, int sx, int sy);
	bool isWithin(int x, int y, int s, int ax, int ay);
	bool isWithin(int x, int y, int sx, int sy, int ax, int ay);

	//debug only
	void d_printValues();
	void d_printWidths();
	void d_printHeights();
	void d_markShape();
	void d_markShape(int linewidth);
	void d_markShapeValid();
	void d_markShapeLines();
	void d_markShapeBounds();
	void d_markAnchorTilt();
	void d_markAnchor();
	void d_debugAnchor();
	void d_markXLine(int y);
	void d_markYLine(int x);
	void d_marklinex(int x1, int x2, int y);
	void d_markliney(int y1, int y2, int x);
	void d_setPixmap(Pixmap *pixmap);
	//debug only

private:
	Config *config;
	int *xmap;	
	int *ymap;	
	int  mapcount;
	int *widths_at_y;
	int *heights_at_x;
	int *midpoints_at_y;
	int *midpoints_at_x;
	bool rotated; 
	int  width, height;
	int  min_x, max_x, min_y, max_y;
	int  center_x, center_y;
	int  grid_w, grid_h; 
	int  midpoint;

	void init();
	int  matchBox();
	int  matchBars();
	int  findAngle(int x1, int y1, int x2, int y2);
	int  findDirection(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	int  pointProximity(int x1, int y1, int x2, int y2);
	bool isDiagonal(int a1, int a2, int a3, int a4);
	int  maxAngle(int a1, int a2, int a3, int a4);
	int  minAngle(int a1, int a2, int a3, int a4);
	void addWidths(int *widths_holder, int *mids_holder, int x, int y);
	void addHeights(int *heights_holder, int *mids_holder, int x, int y);
	int  widthAt(int x);
	int  widthAt(int x, int delta);
	int  heightAt(int y);
	int  midxAt(int y);
	int  midyAt(int x);
	bool isEqual(int a, int b);
	bool isEqualByPixelThreshold(int a, int b, int threshold);

	//debug only
	bool debug;
	bool pixdebug;
	bool anchordebug;
	Pixmap *d_pixmap; 

	void d_debugMatchBars(int tw, int tm, int bw, int bm);
	void d_debugMatchBox(int w, int tw, int tm, int bw, int bm);
	//debug only

};

#endif /* _SHAPE_H_INCLUDED */
