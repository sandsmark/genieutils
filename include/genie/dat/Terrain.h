/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml <email>
    Copyright (C) 2011 - 2015  Mikko T P

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

#ifndef GENIE_TERRAIN_H
#define GENIE_TERRAIN_H
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

class Terrain : public ISerializable
{
public:
  Terrain();
  virtual ~Terrain();
  virtual void setGameVersion(GameVersion gv);
  static unsigned short getTerrainsSize(GameVersion gv);
  static int customTerrainAmount;

  int16_t Unknown1;
  int8_t Enabled; //must be one or the game will crash
  int8_t Random;

  unsigned short getNameSize(void);

  std::string Name;
  std::string Name2;
  int32_t SLP;
  int32_t Unknown3;
  int32_t SoundID;
  int32_t BlendPriority;//not in aoe/ror
  int32_t BlendType; //not in aoe/ror

  std::array<uint8_t, 3> Colors;
  std::pair<uint8_t, uint8_t> CliffColors;
  int8_t PassableTerrain;
  int8_t ImpassableTerrain;

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

  static const unsigned short ELEVATION_GRAPHICS_SIZE = 19;
  std::vector<FrameData> ElevationGraphics;

  int16_t TerrainToDraw;
  std::pair<int16_t, int16_t> TerrainDimensions; // rows + cols

  /// These refer to terrain borders, which are actually used only in AoE and RoR.
  std::vector<int16_t> Borders;

  static const unsigned short TERRAIN_UNITS_SIZE = 30;
  std::array<int16_t, TERRAIN_UNITS_SIZE> TerrainUnitID;
  std::array<int16_t, TERRAIN_UNITS_SIZE> TerrainUnitDensity;

  /// If two terrain units are to be placed on same spot, this selects which one will prevail others.
  /// 1 = prevails, others don't.
  std::array<int8_t, TERRAIN_UNITS_SIZE> TerrainUnitPriority;

  int16_t NumberOfTerrainUnitsUsed;

private:
  virtual void serializeObject(void);
};

}

#endif // GENIE_TERRAIN_H
