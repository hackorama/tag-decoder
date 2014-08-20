set -x
#/c/MingW/bin/c++.exe -g -O3 -Wall -I./pthreads/include -I./jpeg/include -L./jpeg/lib/win32:./pthreads/lib main.cpp decoder.cpp tagimage.cpp pixmap.cpp  config.cpp threshold.cpp border.cpp pattern.cpp matrix.cpp shape.cpp  -ljpeg -lpthreadGCE2 -o decode-mingw.exe
/c/MingW/bin/g++.exe -g -O3 -Wall -I./jpeg/include -L./jpeg/lib/win32 main.cpp decoder.cpp tagimage.cpp pixmap.cpp  config.cpp threshold.cpp border.cpp pattern.cpp matrix.cpp shape.cpp  -ljpeg -o decode-mingw.exe

