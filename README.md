# genieutils #

Genieutils consists of a library to work with certain data and resource/asset
files of genie engine games.

Notice that this library is in developement, that means the API will change.
Please also backup your files before editing to avoid file corruption
because of possible bugs.


## Features ###

 *   reading/writing of empires*.dat and genie*.dat files
 *   reading (and some writing) of drs, slp, blendomatic, and pal files
 *   reading of scn, scx, cpx, bln files
 *   reading/writing of lagnuage*.dll files


## Dependencies ##

 - A modern C++ Compiler (i. e. supports C++17)

This assumes that you do a recursive clone of the repo or remember to update
the submodules, otherwise you need to install zstr, and winiconv when building
for Windows.
