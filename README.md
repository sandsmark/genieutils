# master but with dummy commit to force lgtm to build #



# genieutils #

Genieutils consists of a library to work with certain data and resource/asset
files of genie engine games.

Notice that this library is in developement, that means the API will change.
Please also backup your files before editing to avoid file corruption
because of possible bugs.

This repo is originally a fork of https://github.com/Tapsa/genieutils, but
because a lot of stuff doesn't like github forks, it is now a separate repo.


## Features ###

 *   reading/writing of empires\*.dat and genie\*.dat files
 *   reading (and some writing) of drs, slp, blendomatic, and pal files
 *   reading of scn, scx, cpx, bln files
 *   reading/writing of language\*.dll files


## Dependencies ##

 - A modern C++ Compiler (i. e. supports C++17)

This assumes that you do a recursive clone of the repo or remember to update
the submodules, otherwise you need to install zstr, and winiconv when building
for Windows.

You can update submodules after cloning the repo or pulling new commits by doing:

```
git submodule update --init --recursive
```


## Building ##

The build uses CMake.
```
mkdir build && cd build
cmake ..
cmake --build .
```

This creates a `genieutils.dll` or `libgenieutils.so` file.

To create a static library instead, use the `GENIEUTILS_STATIC_BUILD` flag:
```
cmake .. -DGENIEUTILS_STATIC_BUILD=YES
cmake --build .
```

Optionally, enable link-time optimization:
```
cmake .. -DCMAKE_BUILD_TYPE=Release -DENABLE_IPO=YES
cmake --build .
```
