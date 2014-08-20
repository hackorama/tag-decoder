#include "tagimage.h"

void libjpeg_error_exit(j_common_ptr cinfo) {
    fprintf(stderr, "JPEG Error : " );
    (*cinfo->err->output_message) (cinfo);
    exit(1);
}

const int Tagimage::MAXRGB = 256;

Tagimage::Tagimage(Config *_config)
{
    valid = false;
    COLORS = 1;
    config = _config;

	struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE * infile;
    JSAMPARRAY buffer;
    int row_stride;

    //if ((infile = fopen(config->TAG_IMAGE_FILE.c_str(), "rb")) == NULL) {
    infile = fopen(config->TAG_IMAGE_FILE.c_str(), "rb");
	if( infile == NULL ){
        fprintf(stderr, "can't open file\n");
        exit(1);
    }

    cinfo.err = jpeg_std_error(&jerr);
    jerr.error_exit = libjpeg_error_exit;

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);

    (void) jpeg_read_header(&cinfo, TRUE);

    cinfo.dct_method = JDCT_FASTEST;
    cinfo.do_fancy_upsampling = false;
    cinfo.output_components = 1;
    cinfo.out_color_space = JCS_GRAYSCALE; 
 	/*
	cinfo.output_components = 3;
	cinfo.out_color_space = JCS_RGB; 
	*/

    width  = cinfo.image_width;
    height = cinfo.image_height;

    if(config->JPG_SCALE){ //TODO: Make default remove check after JPEG is stable
		int boxsize = config->PIXMAP_SCALE_SIZE;
		if( boxsize > config->THRESHOLD_WINDOW_SIZE ) {
			if(  width > boxsize || height > boxsize ){
				float scale = width > height ? (float)width/(float)boxsize  :  (float)height/(float)boxsize ;
				int n_width = (int)((float)width/scale);
				int n_height = (int)((float)height/scale);
				if( (width / 8) >= n_width && (height / 8) >= n_height ){
					cinfo.scale_num = 1;
					cinfo.scale_denom = 8;
				}else if((width / 4) >= n_width && (height / 4 >= n_height ) ){
					cinfo.scale_num = 1;
					cinfo.scale_denom = 4;
				}else if((width / 2) >= n_width && (height / 2 >= n_height ) ){
					cinfo.scale_num = 1;
					cinfo.scale_denom = 2;
				}
		  	}
		} 
    }

    jpeg_calc_output_dimensions(&cinfo);

    width  = cinfo.output_width;
    height = cinfo.output_height;

    (void) jpeg_start_decompress(&cinfo);

    assert(cinfo.output_components == cinfo.out_color_components); //dont support colormapped jpeg

    row_stride   = cinfo.output_width * cinfo.output_components;
    width  =  cinfo.output_width;
    height = cinfo.output_height;

    buffer = (*cinfo.mem->alloc_sarray)
    ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    config->PIXBUF = new unsigned char[width * height];

    while (cinfo.output_scanline < cinfo.output_height) {
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
        (void) processScanLine(buffer[0], cinfo.output_width );
    }

    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    valid = true;
}

Tagimage::~Tagimage()
{
	config->freePixbuf();
}

static int imageindex = 0;

void
Tagimage::processScanLine(unsigned char buffer[], int width)
{
		for(int i=0; i<width; i++){
       	     config->PIXBUF[imageindex] = (unsigned char) buffer[i];
       	     imageindex++;
		}
	/*
		for(int i=0; i<3*width; i+=3){
       	     //fprintf(stdout, " |%3d|%3d|%3d| ", buffer[i], buffer[i+1], buffer[i+2] );
       	     r_buffer[imageindex] = (unsigned char) buffer[i];
       	     g_buffer[imageindex] = (int) buffer[i+1];
       	     b_buffer[imageindex] = (int) buffer[i+2];
       	     imageindex++;
    	}
	*/
}

int
Tagimage::getPixel( int x, int y ) 
{
	return config->PIXBUF[(y * width) + x];
}

int
Tagimage::getWidth()
{
	if( width < 0 ) width = 0;
	return width;
}

int
Tagimage::getHeight()
{
	if( height < 0 ) height = 0;
	return height;
}

bool
Tagimage::isValid()
{
	if(config->PIXBUF == NULL) valid = false;
	return valid;
}

int
Tagimage::maxRGB()
{
	return MAXRGB;
}
