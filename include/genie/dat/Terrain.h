/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml <email>
    Copyright (C) 2011 - 2013  Mikko T P

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

class Terrain : public ISerializable
{
public:
  Terrain();
  virtual ~Terrain();
  virtual void setGameVersion(GameVersion gv);

  int16_t Unknown1;
  int16_t Enabled; //must be one or the game will crash

  unsigned short getNameSize(void);

  std::string Name;
  std::string Name2;
  int32_t SLP;
  float Unknown3;
  int32_t SoundID;
  int32_t BlendPriority;//not in aoe/ror
  int32_t BlendType; //not in aoe/ror

  std::array<uint8_t, 3> Colors;
  std::array<int8_t, 5> Unknown5;
  float Unknown6;
  static const unsigned short UNKNOWN7_SIZE = 18;
  std::array<int8_t, UNKNOWN7_SIZE> Unknown7;
  int16_t FrameCount;
  int16_t AngleCount;
  int16_t TerrainID;
  static const unsigned short ELEVATION_GRAPHICS_SIZE = 54;
  std::array<int16_t, ELEVATION_GRAPHICS_SIZE> ElevationGraphics;
  int16_t TerrainReplacementID;
  static const unsigned short TERRAIN_DIMENSIONS_SIZE = 2;
  std::pair<int16_t, int16_t> TerrainDimensions;
  unsigned short getTerrainBorderSize();

  /// These refer to terrain borders, which are actually used only in AoE and RoR.
  std::vector<int16_t> TerrainBorderIDs;

  static const unsigned short TERRAIN_UNITS_SIZE = 30;
  std::array<int16_t, TERRAIN_UNITS_SIZE> TerrainUnitID;
  std::array<int16_t, TERRAIN_UNITS_SIZE> TerrainUnitDensity;

  /// If two terrain units are to be placed on same spot, this selects which one will prevail others.
  /// 1 = prevails, others don't.
  std::array<int8_t, TERRAIN_UNITS_SIZE> TerrainUnitPriority;

  int16_t NumberOfTerrainUnitsUsed;

  static const unsigned short SWGBUNKNOWN1_SIZE = 24;
  std::array<int8_t, SWGBUNKNOWN1_SIZE> SWGBUnknown1;

private:
  virtual void serializeObject(void);
};

}

#endif // GENIE_TERRAIN_H
