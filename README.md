# genieutils #

Genieutils is a library to work with data and asset files of Genie engine games
(everything from AoE1 Alpha versions, via SWGB:CC to the Definitive
Editions).

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

For a lot of stuff it is also possible to convert between different versions of
the formats. The only version conversion that has a tool (i. e. you don't need
to write code) is the .dat files, though. For e. g. graphics you will have to
hook together the SLP and ICM classes yourself with a couple of lines of C++.

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

## TODO ##

File formats used in Genie that are not supported (yet):

 - MP3 files, used for in-game music.
    - I recommend using something like [miniaudio's implementation](https://github.com/mackron/miniaudio/blob/master/extras/dr_mp3.h)
      if you need it, it's used in freeaoe and works fine with the genie games' music.
 - MIDI files, also used for in-game music.
    - https://github.com/schellingb/TinySoundFont is a good alternative if you need it.
    - [This sound font](https://github.com/sandsmark/freeaoe/tree/martin/midi/data/general808)
      should cover everything, but doesn't have the best quality.
 - AVI videos, used for intro videos etc.
    - All the games (pre-DE at least) seem to use plain avi files with indeo41,
      with the same bit depth etc. and the scummvm implementation should work.
      So it's just a matter of porting it over.
    - According to Folkert van Verseveld (the author of the
      [aoe](https://github.com/FolkertVanVerseveld/aoe) project, ffmpeg's
      indeo41 implementation introduces jitter, so just using that is probably out of the window.
    - Started on in the martin/video branch in freeaoe, I'll probably throw it
      in genieutils when it gets finished and I manage to implement write
      support (don't like read-only stuff in genieutils).
 - AI scripts.
    - There is a fairly complete bison/flex based parser (as well as the script
      to generate the source files) here:
      https://github.com/sandsmark/freeaoe/tree/master/src/ai
 - PNG Files, used by HD and later for e. g. terrains.
    - Bring your own, everything ships with PNG support these days.
 - RMS (random map) files.

In progress (i. e. started on importing code from elsewhere and got bored):
 - DDS files, used by DE games for e. g. terrain textures, started in the `martin/dds` branch.
 - wwise wav files, used by the DE games, started in the martin/wwise branch.
 - Recorded games, started in the martin/mgx branch.
 - LZ4, which the DE games have started using, lives in src/file/LZ4.cpp but not used.
 - CAB support, to be able to e. g. load the data directly from the installer
   for the trial versions without manual unpacking. Lives in
   src/file/CabFile.cpp, but got bored of fixing LZX support.

