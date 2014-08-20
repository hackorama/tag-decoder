CC=g++
set -x
${CC} -O3 -I./jpeg/include -c main.cpp 
${CC} -O3 -I./jpeg/include -c decoder.cpp 
${CC} -O3 -I./jpeg/include -c tagimage.cpp 
${CC} -O3 -I./jpeg/include -c pixmap.cpp  
${CC} -O3 -I./jpeg/include -c config.cpp 
${CC} -O3 -I./jpeg/include -c threshold.cpp 
${CC} -O3 -I./jpeg/include -c border.cpp 
${CC} -O3 -I./jpeg/include -c pattern.cpp 
${CC} -O3 -I./jpeg/include -c matrix.cpp 
${CC} -O3 -I./jpeg/include -c shape.cpp 
${CC} -L./jpeg/lib/linux  main.o decoder.o tagimage.o pixmap.o  config.o threshold.o border.o pattern.o matrix.o shape.o  -ljpeg -o decode

