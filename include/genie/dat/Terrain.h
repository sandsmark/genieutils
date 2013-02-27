/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2013  Armin Preiml <email>

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
  Terrain(GameVersion gv = GV_None);
  virtual ~Terrain();
  virtual void setGameVersion(GameVersion gv);
  
  int16_t Unknown1;
  int16_t Unknown2; //must be one or the game will crash
  
  short getNameSize(void);
  
  std::string Name;
  std::string Name2;
  int32_t SLP;
  int32_t Unknown3;
  int32_t SoundID;
  int32_t BlendPriority;//not in aoe/ror
  int32_t BlendType; //not in aoe/ror
  
  std::vector<unsigned char> Colors;
  int16_t Unknown5;
  int16_t Unknown6;
  static const short UNKNOWN7_LEN = 23;
  std::vector<char> Unknown7;
  int16_t FrameCount;
  static const short UNKNOWN8_LEN = 2;
  std::vector<int16_t> Unknown8;
  static const short UNKNOWN9_LEN = 54;
  std::vector<int16_t> Unknown9;
  int16_t TerrainReplacementID;
  static const short TERRAIN_DIMENSIONS_LEN = 2;
  std::pair<int16_t, int16_t> TerrainDimensions;
  short getTerrainBorderSize();
  
  /// These refer to terrain borders, which are actually used only in AoE and RoR.
  std::vector<int16_t> TerrainBorderIDs;
  
  static const short TERRAIN_UNITS_LEN = 30;
  std::vector<int16_t> TerrainUnitID;
  std::vector<int16_t> TerrainUnitDensity;
  
  /// If two terrain units are to be placed on same spot, this selects which one will prevail others.
  /// 1 = prevails, others don't.
  std::vector<char> TerrainUnitPriority;
  
  int16_t NumberOfTerrainUnitsUsed;
  
  static const short SWGBUNKNOWN1_LEN = 24;
  std::vector<char> SWGBUnknown1;
  
private:
  virtual void serializeObject(void);
};

}

#endif // GENIE_TERRAIN_H
