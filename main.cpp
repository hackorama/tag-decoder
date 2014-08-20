/* 
* 1.0R Jul 2005
* 2.0R Sep 2007 
* 3.0R Nov 2007 
* 4.0R Dec 2007 

* 5.0D Apr 2008 
*/


#include <time.h>
#include "decoder.h"

int main(int argc,char **argv) {
	int tag[12];

	Decoder* decoder = new Decoder(argc, argv);
	if(decoder->processTag()) { 
		decoder->copyTag(tag);
		for(int i=0; i<12; i++) cout << tag[i];
		cout << endl;
	}
	delete decoder;

	//perf
	if(argc > 3){ 
		if(strcmp(argv[3], "t") == 0) cout << (float)clock()/(float)CLOCKS_PER_SEC << " Secs" << endl;
	}
	//perf

	return 0;
}

