#ifndef _BORDER_H_INCLUDED
#define _BORDER_H_INCLUDED

#include <map>
#include "pixmap.h"
#include "shape.h"
#include "pattern.h"
#include "common.h"

#define BLACK 0
#define GREEN 1
#define RED 2
#define BLUE 3
#define YELLOW 4
#define AQUA 5
#define PINK 6
#define WHITE 7

using namespace std;

class Border
{

public:
	Border(Config *config, Shape *shapes, Shape *anchor);
	~Border();
	int findShapes();
	int getShapeCount();
	Shape* getShapes();
	Shape* getAnchor();

private:
	Config *config;
	Shape *shapes;
	Shape *anchor;  // selected anchor from current 
	Shape *anchors;
	Shape *current; // the current shape being processed
	Pixmap *pixmap;
	Pattern *pattern;
	bool *edgemap;
	int  width, height;
	int  min_threshold, max_threshold;
	int  shapes_found;
	int  anchors_found;
	int *widths_holder;
	int *heights_holder;
	int *w_midpoints_holder;
	int *h_midpoints_holder;
	int mapcount;
	int max_shapes;
	int max_anchors;

	//globals for recursion
	vector<int> xmap; //dynamic holder for shape x values
	vector<int> ymap; //dynamic holder for shape y values
	int min_x, min_y, max_x, max_y;
	int tx, ty;
	int startx, starty, seg_count;
	//globals for recursion

	void getBorders();
	void markBorder(int x, int y);
	bool isEdge(int x, int y);
	bool borderTrace(int x, int y);
	bool filterShape();
	void filterAnchor();
	void anchorCheck();
	void addShape();
	void addAnchor();
	bool foundShapes();
	bool foundAnchor();
	bool findAnchor();
	void copyAnchor(Shape *shape);
	void addWidths(int x, int y);
	void addHeights(int x, int y);
	void resetWidthsAndHeights();

	//debug only
	bool debug;
	bool pixdebug;
	bool anchordebug;
	int BORDERCOLOR; //FIXME: remove after debug

	void d_debugShapes();
	void d_debugShapes(string filename);
	void d_writeShapes();
	void d_writeShapes(string filename);
	void d_setColor(int x, int y, int r, int g, int b);
	void d_setColor256RGB(int x, int y, int r, int g, int b);
	void d_setColor(int x, int y, int color);
	//debug only

};

#endif /* _BORDER_H_INCLUDED */
