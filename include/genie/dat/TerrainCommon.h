/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2015  Mikko T P

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GENIE_TERRAINCOMMON_H
#define GENIE_TERRAINCOMMON_H
#include "genie/file/ISerializable.h"

namespace genie
{

class FrameData : public ISerializable
{
public:
  FrameData();
  virtual ~FrameData();
  virtual void setGameVersion(GameVersion gv);

  int16_t FrameCount;
  int16_t AngleCount;
  int16_t ShapeID;

private:
  virtual void serializeObject(void);
};

class SharedTerrain : public ISerializable
{
public:
  static const unsigned short TILE_TYPE_COUNT = 19;

  int8_t Enabled; //must be one or the game will crash
  int8_t Random;

  virtual unsigned short getNameSize(void)=0;

  /// Internal long name
  std::string Name;
  /// Internal SLP name
  std::string Name2;
  int32_t SLP;
  int32_t Unknown3;
  int32_t SoundID;

  std::vector<uint8_t> Colors;

  int8_t IsAnimated;
  int16_t AnimationFrames; // # of frames to animate through
  int16_t PauseFames; // # of frames to pause animation after last frame is drawn
  float Interval; // time between frames
  float PauseBetweenLoops; // time to pause after last frame
  int16_t Frame; // the current frame (includes animation & pause frames)
  int16_t DrawFrame; // the current frame to draw
  float AnimateLast; // last time animation frame was changed
  int8_t FrameChanged; // has the DrawFrame changed since terrain was drawn?
  int8_t Drawn;
};

}

#endif // GENIE_TERRAINCOMMON_H
