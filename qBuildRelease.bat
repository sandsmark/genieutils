CD ../genieutils-build/release/
SET zlibIF="H:\Qt\Tools\mingw492_32\i686-w64-mingw32\include"
SET zlibLL="H:\Qt\Tools\mingw492_32\i686-w64-mingw32\lib\libz.a"
SET iconvIF="H:\Qt\Tools\mingw492_32\i686-w64-mingw32\include"
SET iconvLL="H:\Qt\Tools\mingw492_32\i686-w64-mingw32\lib\libiconv.a"
cmake -j4 -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DGUTILS_TOOLS=0 -DGUTILS_TEST=0 -DZLIB_INCLUDE_DIR="%zlibIF%" -DZLIB_LIBRARY="%zlibLL%" -DICONV_INCLUDE_DIR="%iconvIF%" -DICONV_LIBRARIES="%iconvLL%" ../../genieutils/
mingw32-make
CD ../../genieutils/
PAUSE
