# use the installed headers and library version
# g++ -g -O3 -Wall  main.cpp decoder.cpp tagimage.cpp pixmap.cpp  config.cpp threshold.cpp border.cpp pattern.cpp matrix.cpp shape.cpp  -ljpeg -o decode

set -x

g++ -g -O3 -Wall -I./jpeg/include -L./jpeg/lib/cygwin main.cpp decoder.cpp tagimage.cpp pixmap.cpp  config.cpp threshold.cpp border.cpp pattern.cpp matrix.cpp shape.cpp  -ljpeg -lpthread  -o decode

