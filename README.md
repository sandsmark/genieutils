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
 *   reading/writing of drs, slp, smp, smx, blendomatic, and pal files
 *   reading/writing of scn, scx, cpx, bln, sin, wav files
 *   reading/writing of language\*.dll files

Also includes several extra tools.

### Command line tools ###

 * `cpxtool`: command line tool to work with campaign and scenario files (view details and extract).
 * `genie-datextract`: convert .dat files between game versions, or just extract to raw.
 * `drstool`: list and/or extract contents from .drs files (graphics, sound, etc.)
 * `pcriotool`: list or show a single string from the language DLLs.
 * `genie-bindiff`: very simple binary differ (mostly for debugging, better to use dhex if you need something serious).

### Graphical tools (requires Qt) ###

 * `picviewer`: display SLP/SMP/SMX files.
 * `drsexplorer`: browse and show SLP/SMP/SMX graphics in a DRS archive.

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
