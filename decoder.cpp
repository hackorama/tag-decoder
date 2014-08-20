#include "decoder.h"

Decoder::Decoder(string _filename)
{
	init();
	config->TAG_IMAGE_FILE = _filename;
	config->ARGS_OK = true;
}

Decoder::Decoder(int argc,char **argv)
{
	init();
	config->checkArgs(argc, argv);
}

Decoder::Decoder(Tagimage* _tagimage)
{
	init();
	tagimage = _tagimage;
	if(tagimage != NULL) config->ARGS_OK = true;
}

Decoder::~Decoder()
{
	if(tagimage != NULL) delete tagimage;
	if(config != NULL)   delete config;
}

void
Decoder::init()
{
	tagimage = NULL;
	for(int i=0; i<12; i++) tag[i] = -1;
	config = (Config*) new Config();
}

Config*
Decoder::getConfig()
{
	return config;
}

void
Decoder::copyTag(int* _tag)
{
	for(int i=0; i<12; i++) _tag[i] = tag[i];
}

bool
Decoder::processTag()
{
	if(!config->ARGS_OK ) return false;
	if(tagimage == NULL) tagimage = new Tagimage(config);
	if(!tagimage->isValid()) { 
		delete tagimage; tagimage = NULL;
		return false;
	}
	if(config->VISUAL_DEBUG) config->setDebugPixmap(new Pixmap(config->TAG_IMAGE_FILE));
	Threshold* threshold = new Threshold(config, tagimage);
	threshold->computeEdgemap();
	delete tagimage; tagimage = NULL;
	delete threshold;
	Shape *shapes = new Shape[config->MAX_SHAPES];
	Shape *anchor = new Shape(config);
	Border* border = new Border(config, shapes, anchor);
	int nshapes = border->findShapes();
	delete border;
	if( nshapes >= 12  ){
		Pattern* pattern = new Pattern(config, shapes, nshapes, anchor);
		pattern->findCode(tag);
		delete pattern;
	}
	delete anchor;
	delete [] shapes;
	return true;
}

