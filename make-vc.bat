cl /O2 /I "jpeg\include"  /EHsc /Fo"tmp\\" /MT /nologo /TP main.cpp threshold.cpp decoder.cpp config.cpp tagimage.cpp shape.cpp pixmap.cpp pattern.cpp matrix.cpp border.cpp /link /OUT:"decode-win32-release.exe" /NOLOGO /LIBPATH:"jpeg\lib\win32" libjpeg.a kernel32.lib 

