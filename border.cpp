#include "border.h"

Border::Border(Config *_config, Shape* _shapes, Shape* _anchor)
{
	config = _config;
	shapes = _shapes;
	anchor = _anchor;


	debug       = config->DEBUG;
	pixdebug    = config->CHECK_VISUAL_DEBUG();
	anchordebug = config->ANCHOR_DEBUG;
	max_anchors = config->MAX_ANCHORS;
	max_shapes  = config->MAX_SHAPES;

	for(int i = 0; i < max_shapes; i++) shapes[i].setConfig(config); 

	if(pixdebug) pixmap = config->DBGPIXMAP;
	else         pixmap = NULL;

	edgemap = config->EDGE_MAP;
	assert(edgemap != NULL );
	width = config->GRID_WIDTH;
	height = config->GRID_HEIGHT;
	min_threshold = ( width > height ) ? width/20: height/20;
	max_threshold = ( width < height ) ? width/2 : height/2; //changed from 4 to account rotated ones

	current = new Shape(config);
	anchors = new Shape[max_anchors];
	for(int i = 0; i < max_anchors; i++) anchors[i].setConfig(config); 

	shapes_found  = 0;
	anchors_found = 0;
	widths_holder = new int[height];
	heights_holder = new int[width];
	for(int i=0; i<height; i++) widths_holder[i] = 0;
	for(int i=0; i<width; i++)  heights_holder[i] = 0;
	w_midpoints_holder = new int[height];
	h_midpoints_holder = new int[width];
	for(int i=0; i<height; i++) w_midpoints_holder[i] = 0;
	for(int i=0; i<width; i++)  h_midpoints_holder[i] = 0;

	if(pixdebug) { 
		pixmap->resizePixmap(config->GRID_WIDTH, config->GRID_HEIGHT);
		pixmap->clearPixmap(); //reset after scaling
		current->d_setPixmap(pixmap);
		anchor->d_setPixmap(pixmap);
	}
}

Border::~Border()
{
	config->freeEdgemap();
	delete current;
	delete [] anchors;
	delete [] widths_holder;
	delete [] heights_holder;
	delete [] w_midpoints_holder;
	delete [] h_midpoints_holder;
}

Shape*
Border::getShapes()
{
	return shapes;
}

Shape*
Border::getAnchor()
{
	return anchor;
}

int
Border::getShapeCount()
{
	return shapes_found;
}

int
Border::findShapes()
{
	getBorders();
	if( foundShapes() ) { 
		if(! foundAnchor()) findAnchor(); 
		if(foundAnchor()) return shapes_found;
	}
	return 0;
}

/* Sigle pass for recursive edge tracing 
* and shape length based selection filterShape()
* TODO: parsing stsrts from top-left corner
*       evaluate parsing from center point
*       could bail out bad tags early 
*       and avoid noise on edge shapes 
* TODO: bail out bad tags ideas 
* 	1. shape density 
*       2. border points density 
*/
void
Border::getBorders()
{
	int i=0, j=0, count = 0;
	if(pixdebug) pixmap->debugImage( "border", count++ );
	for(j=0; j<height; j++){
		for(i=0; i<width; i++){
			if( isEdge(i, j) ){

				BORDERCOLOR = (count++%4)+3; //FIXME: Remove after debug

				// reset globals
				min_x = i; min_y = j; max_x = i; max_y = j;
				tx = 0; ty = 0;
				startx = i; starty = j;
				xmap.clear();
				ymap.clear();
				seg_count = 0;
				resetWidthsAndHeights();

				// trace
				borderTrace(i, j);
				markBorder(i, j); 

				// verify and keep stored values
				if(filterShape()) {  
					addShape();
				}else if(pixdebug) { 
					pixmap->setPen( pixmap->maxRGB(), pixmap->maxRGB(), pixmap->maxRGB() );
					//pixmap->debugImage("skipped");
				}
			}
		}
	}
	if( pixdebug ){
		pixmap->writeImage("allshapes");
		pixmap->clearPixmap();
		pixmap->setPen(0, 0, 0);
	}
}

void
Border::resetWidthsAndHeights()
{
	for(int i=0; i<height; i++) widths_holder[i] = 0;
	for(int i=0; i<height; i++) w_midpoints_holder[i] = 0;
	for(int i=0; i<width; i++)  heights_holder[i] = 0;
	for(int i=0; i<width; i++)  h_midpoints_holder[i] = 0;
}

void
Border::addWidths(int x, int y)
{
	if(widths_holder[y] == 0){
		widths_holder[y]      = x*-1;
		w_midpoints_holder[y] = x;
	}else if(widths_holder[y] < 0){
		int diff = abs(widths_holder[y]+x);
		if(diff > 8)  {
			//w_midpoints_holder[y] = abs(widths_holder[y])+(diff/2);
			int lastx = abs(widths_holder[y]);
			w_midpoints_holder[y] = x > lastx ? lastx+(diff/2) : x+(diff/2);
			widths_holder[y]      = diff; 
		}
	}
}

void
Border::addHeights(int x, int y)
{
	if(heights_holder[x] == 0){
		heights_holder[x]     = y*-1;
		h_midpoints_holder[x] = y;
	}else if(heights_holder[x] < 0){
		int diff = abs(heights_holder[x]+y);
		if(diff > 8) {
			//h_midpoints_holder[x] = abs(heights_holder[x])+(diff/2);
			int lasty = abs(heights_holder[x]);
			h_midpoints_holder[x] = y > lasty ? lasty+(diff/2) : y+(diff/2);
			heights_holder[x]     = diff;
		}
	}
}

bool
Border::borderTrace(int x, int y)
{
	if (x < min_x) min_x = x;
	if (x > max_x) max_x = x;
	if (y < min_y) min_y = y;
	if (y > max_y) max_y = y;
	if( seg_count > 0){
		if (x == startx && y == starty) return true; // closed border 
		else                            markBorder( x, y ); 
	}
	xmap.push_back(x);
	ymap.push_back(y);
	addWidths(x,y);
	addHeights(x,y);
	tx+=x;
	ty+=y;
	seg_count++;

	int i = 1;
	if(isEdge(  x, y+i)) borderTrace(  x, y+i);
	if(isEdge(  x, y-i)) borderTrace(  x, y-i); 

	if(isEdge(x+i, y-i)) borderTrace(x+i, y-i);
	if(isEdge(x+i, y  )) borderTrace(x+i, y  );
	if(isEdge(x+i, y+i)) borderTrace(x+i, y+i);

	if(isEdge(x-i, y+i)) borderTrace(x-i, y+i);
	if(isEdge(x-i, y  )) borderTrace(x-i, y);
	if(isEdge(x-i, y-i)) borderTrace(x-i, y-i);
	return false; // open border
}

bool
Border::foundShapes()
{
	if( shapes_found < 12 )        return false;
	return true;
}

bool
Border::foundAnchor()
{
	if( anchor->size() <= 0 ) return false;
	return true;
}

/* 
* Check if current shape matches anchor features
* Replace selected anchor if this one is bigger 
* TODO: size checked by bounding box area Shape::size()
*       should we use the Shape::length() instead 
*/
void
Border::anchorCheck()
{
	current->setValues(xmap, ymap, seg_count);
	current->setBounds(min_x, min_y, max_x, max_y);
	current->setCenter( min_x + (max_x - min_x)/2, min_y + (max_y - min_y)/2 );
	current->setWidthValues(widths_holder, w_midpoints_holder, min_y, max_y, false);
	current->setHeightValues(heights_holder, h_midpoints_holder, min_x, max_x, false);
	if(anchordebug) cout << "[ anchorCheck " ;

	if( current->isAnchor() ){ //level=0 strictest select as final anchor
		if(anchordebug) cout << current->size() << "-" << anchor->size() << endl;
		if( current->size() > anchor->size() ){ //larger block over rides last found anchor 
			anchor->setValues(xmap, ymap, seg_count);
			anchor->setBounds(min_x, min_y, max_x, max_y);
			anchor->setCenter( min_x + (max_x - min_x)/2, min_y + (max_y - min_y)/2 );
			anchor->copyWidthValues(current->getWidthValues(), current->getWMidpointValues(),  min_y, max_y);
			anchor->copyHeightValues(current->getHeightValues(), current->getHMidpointValues(), min_x, max_x);
			if(anchordebug) cout << " Found : " << anchor->size();
		}
	}else if( current->isAnchorLike() ){ //level=3 loosest, add to possible list of anchors
		addAnchor();
	}
	if(anchordebug) cout << "anchorCheck ]"  << endl;
}

/*
* if the first threshold based pass did not find an anchor
* this could be a tilted or abnormal sized one 
* this pass selects the biggest block from the 
* threshold selection as an anchor if it passes the square check
* 
* Instaed of Shape::isSquare() uses a more loose check Shape::isAnchorLevel2()
*
* NOTE: Returns true only on finding a new one 
* Return False, If there is an existing one or we could not find a new one
*
*/
//Border::findAnchor(int level)
bool
Border::findAnchor()
{
	int maxsize = 0;
	int shapeindex = -1, anchorindex = -1;
	if( anchor->size() > 0 ) return false; //already have an anchor
	if(anchordebug) cout << "[findAnchor " << endl;
	for (int i = 0; i < shapes_found; i++){
		if( shapes[i].size() > maxsize ) { 
			if( shapes[i].isAnchor() ) {
				if(anchordebug) cout << "shapes[" << i << "] size=" 
					<< shapes[i].size() << " max=" << maxsize << endl;
				maxsize = shapes[i].size();
				shapeindex = i;
			}
		}
	}
	for (int i = 0; i < anchors_found; i++){
		if( anchors[i].size() > maxsize ) { 
			if( anchors[i].isAnchor() ) {
				if(anchordebug) cout << "anchors[" << i << "] size=" 
					<< anchors[i].size() << "max=" << maxsize << endl;
				maxsize = anchors[i].size();
				anchorindex = i;
			}
		}
	}
	if(anchordebug)          cout << "findAnchor ]" << endl;
	if(anchorindex >= 0)     copyAnchor(&anchors[anchorindex]);
	else if(shapeindex >= 0) copyAnchor(&shapes[shapeindex]);
	else   			 return false;
	if(pixdebug){
		anchor->d_debugAnchor();
		anchor->d_markAnchor();
		ostringstream	tmp;
		tmp << "anchor-level" ;
		pixmap->writeImage( tmp.str() );
	}
	if(anchordebug) { 
		cout << "Found Anchor " ;
		if(anchorindex >= 0)  cout << " index=" << anchorindex << " in anchors" << endl;
		else  		      cout << " index=" << shapeindex << " in shapes" << endl;
	}
	return true; //found a new one, with relaxed check
}

void
Border::copyAnchor(Shape *shape)
{
	int mnx =0, mny = 0, mxx = 0, mxy = 0;
	mnx = shape->getminx();
	mny = shape->getminy();
	mxx = shape->getmaxx();
	mxy = shape->getmaxy();
	anchor->copyValues(shape->getxmap(), shape->getymap(), shape->getmapcount());
	anchor->setBounds(mnx, mny, mxx, mxy);
	anchor->setCenter( mnx + (mxx - mnx)/2, mny + (mxy - mny)/2 );
	//anchor->setGrid( pixmap->getWidth(),  pixmap->getHeight() );
	anchor->copyWidthValues(shape->getWidthValues(), shape->getWMidpointValues(), mny, mxy);
	anchor->copyHeightValues(shape->getHeightValues(), shape->getHMidpointValues(), mnx, mxx);
}

void
Border::addAnchor()
{
	if( anchors_found >= max_anchors ){
		if(debug) cout << "ERROR: MAX ANCHOR SHAPES " << endl;
		return;
	}
	anchors[anchors_found].setValues(xmap, ymap, seg_count);
	anchors[anchors_found].setWidthValues(widths_holder, w_midpoints_holder, min_y, max_y, true);
	anchors[anchors_found].setHeightValues(heights_holder, h_midpoints_holder, min_x, max_x, true);
	anchors[anchors_found].setBounds(min_x, min_y, max_x, max_y);
	anchors[anchors_found].setCenter(min_x + (max_x - min_x)/2, min_y + (max_y - min_y)/2);
	if(anchordebug) cout << "addAnchor :" << anchors[anchors_found].size() << " (" << anchors_found << ")" << endl;
	if(debug){
		anchors[anchors_found].d_printWidths();
		anchors[anchors_found].d_printHeights();
	}
	anchors_found++;
}

void
Border::addShape()
{
	if( shapes_found >= max_shapes ){
		if(debug) cout << "ERROR: MAX SHAPES " << endl;
		return;
	}
	shapes[shapes_found].setValues(xmap, ymap, seg_count);
	shapes[shapes_found].setWidthValues(widths_holder, w_midpoints_holder, min_y, max_y, true);
	shapes[shapes_found].setHeightValues(heights_holder, h_midpoints_holder, min_x, max_x, true);
	shapes[shapes_found].setBounds(min_x, min_y, max_x, max_y);
	shapes[shapes_found].setCenter( min_x + (max_x - min_x)/2, min_y + (max_y - min_y)/2 );
	shapes[shapes_found].d_setPixmap(pixmap);
	if(debug){
		shapes[shapes_found].d_printWidths();
		shapes[shapes_found].d_printHeights();
	}

	shapes_found++;
	if(pixdebug){
		ostringstream tmp;
		tmp << "shape-" << shapes_found-1 << "-" << xmap.size() << "-" 
			<<  min_x + (max_x - min_x)/2 << "-" << min_y + (max_y - min_y)/2  ;
		pixmap->debugImage(tmp.str());
	}
}

void
Border::filterAnchor()
{
	if(!foundAnchor()) anchorCheck();
}

/*
* bounding box size based threshold 
* TODO : Add total length Shape:Llength() based threshold 
*        to weed out boudning box fitting non blocks
*
*/
bool
Border::filterShape()
{
	int length = xmap.size();
	if(debug){ 
		cout << length << " "  << min_x << ", " << min_y  << "  " << max_x << ", " << max_y ;
		cout << " [" << min_threshold << "-" <<  max_threshold << "] "  ;
		cout <<  max_x - min_x << " ";
		cout <<  max_y - min_y << " " ;
	}
	if(((max_x - min_x) < min_threshold || (max_y - min_y) < min_threshold)
		&& length < (2*min_threshold) ) { //extra check for rotated 6 and 7 shapes 
			if(debug) cout << " \t: - "  << endl;
	} else if( (max_x - min_x) > max_threshold ||  (max_y - min_y) > max_threshold ) {
		if(debug) cout << " \t: + "  << endl;
		filterAnchor();
	}else{
		if(debug) cout << " \t: *  " << endl;
		return true;
	}
	return false;
}

void
Border::markBorder(int x, int y)
{
	edgemap[(y*width)+x] = false;
	if(pixdebug) d_setColor(x, y, BORDERCOLOR);
}

bool
Border::isEdge(int x, int y)
{
	return edgemap[(y*width)+x];
}

void
Border::d_setColor(int x, int y, int color)
{
	if( color == RED )    d_setColor256RGB(x, y, 255, 0, 0);
	if( color == GREEN )  d_setColor256RGB(x, y, 0, 255, 0);
	if( color == BLUE )   d_setColor256RGB(x, y, 0, 0, 255);
	if( color == YELLOW ) d_setColor256RGB(x, y, 255, 215, 0);
	if( color == PINK )   d_setColor256RGB(x, y, 255, 0, 255);
	if( color == AQUA )   d_setColor256RGB(x, y, 0, 128, 255);
}

void
Border::d_setColor256RGB(int x, int y, int r, int g, int b)
{
	if(pixdebug) pixmap->setPixel256RGB(x, y, r, g, b);
}

void
Border::d_setColor(int x, int y, int r, int g, int b)
{
	if(pixdebug) pixmap->setPixel(x, y, r, g, b);
}

void
Border::d_debugShapes(string filename)
{
	if(pixdebug) d_writeShapes(filename);
}

void
Border::d_debugShapes()
{
	if(pixdebug) d_writeShapes();
}

void
Border::d_writeShapes(string filename)
{
	if(!pixdebug) return;

	d_writeShapes();
	pixmap->writeImage(filename);
}

void
Border::d_writeShapes()
{
	if(!pixdebug) return;

	pixmap->clearPixmap();
	for (int i = 0; i < shapes_found; i++) shapes[i].d_markShape(); 
	if( debug ) { 
		cout << "Code Blocks = " << shapes_found ;
		anchor->size() <= 0 ?  cout << ", no Anchor"<< endl :  cout << endl ;
	}
}
