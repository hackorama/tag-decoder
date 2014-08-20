#ifndef _DECODER_H_INCLUDED
#define _DECODER_H_INCLUDED

#include <string>

#include "threshold.h"
#include "tagimage.h"
#include "pixmap.h"
#include "border.h"
#include "pattern.h"
#include "common.h"


using namespace std;

class Decoder 						//I am the tag decoder engine
{

public:
	Decoder(string filename); 		//Give me the image file name
	Decoder(int argc, char **argv); //Or give me the image file and other command line options
	Decoder(Tagimage* tagimage);	//Or give the image object you have created already 
	~Decoder();						//  **BE WARNED** To save memory I am told to delete the image 
									//  as soon I finish processing, so pass me a copy of you want to keep it. 
									//  *DONT DELETE** the image agian yourself afterwards
									//  I feel bad to do this, but memory is tight on cellphones 
	
	Config* getConfig();			//Get my configuration control, and customize my behaviour
	bool    processTag();			//Ask me to proces it for you (I assign all my work to others here)
	void    copyTag(int *tag);		//Copy (not a reference) the result back to you

private:							//These are my internal stuff, not of interest to outside world
	void init();

	Config*   config;		//Where I store all my options (ask the Config class for details)
	Tagimage* tagimage;		//The image I am working on(either a reference or one I created)
	int tag[12];			//I store the result here
};

#endif /* _DECODER_H_INCLUDED */

