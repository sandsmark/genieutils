/*
    genie/dat - A library for reading and writing data files of genie
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


#include "genie/dat/Terrain.h"

namespace genie
{

//------------------------------------------------------------------------------
Terrain::Terrain(GameVersion gv) : Colors(3),
                     Unknown7(UNKNOWN7_LEN), Unknown8(UNKNOWN8_LEN),
                     Unknown9(UNKNOWN9_LEN),
                     TerrainBorderIDs(0),
                     TerrainUnitID(TERRAIN_UNITS_LEN),
                     TerrainUnitDensity(TERRAIN_UNITS_LEN),
                     TerrainUnitPriority(TERRAIN_UNITS_LEN),
                     SWGBUnknown1(SWGBUNKNOWN1_LEN)
{
  setGameVersion(gv);
  Unknown1 = 0;
  Unknown2 = 1;
  Name = "";
  Name2 = "";
  SLP = -1;
  Unknown3 = -1;
  SoundID = -1;
  BlendPriority = -1;
  BlendType = -1;
  Unknown5 = 0;
  Unknown6 = 0;
  FrameCount = 0;
  TerrainReplacementID = 0;
  NumberOfTerrainUnitsUsed = 0;
  TerrainDimensions.first = TerrainDimensions.second = -1;
}

//------------------------------------------------------------------------------
Terrain::~Terrain()
{
}

void Terrain::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
  
  TerrainBorderIDs.resize(getTerrainBorderSize());
}

//------------------------------------------------------------------------------
short Terrain::getNameSize()
{
  if (getGameVersion() >= genie::GV_SWGB)
    return 17;
  else 
    return 13;
}

//------------------------------------------------------------------------------
short Terrain::getTerrainBorderSize(void)
{
  if (getGameVersion() >= genie::GV_TC)
    return 42;
  else
    return 32;
}

//------------------------------------------------------------------------------
void Terrain::serializeObject(void )
{
  serialize<int16_t>(Unknown1);
  serialize<int16_t>(Unknown2);
  
  serialize<std::string>(Name, getNameSize());
  serialize<std::string>(Name2, getNameSize());
  
  serialize<int32_t>(SLP);
  serialize<int32_t>(Unknown3);
  serialize<int32_t>(SoundID);
  
  if (getGameVersion() >= genie::GV_AoK)
  {
    serialize<int32_t>(BlendPriority);
    serialize<int32_t>(BlendType);
  }
  
  serialize<unsigned char>(Colors, 3);
  serialize<int16_t>(Unknown5);
  serialize<int16_t>(Unknown6);
  serialize<char>(Unknown7, UNKNOWN7_LEN);
  serialize<int16_t>(FrameCount);
  serialize<int16_t>(Unknown8, UNKNOWN8_LEN);
  serialize<int16_t>(Unknown9, UNKNOWN9_LEN);
  serialize<int16_t>(TerrainReplacementID);
  serialize<int16_t>(TerrainDimensions);
  serialize<int16_t>(TerrainBorderIDs, getTerrainBorderSize());
  
  serialize<int16_t>(TerrainUnitID, TERRAIN_UNITS_LEN);
  serialize<int16_t>(TerrainUnitDensity, TERRAIN_UNITS_LEN);
  serialize<char>(TerrainUnitPriority, TERRAIN_UNITS_LEN);
  serialize<int16_t>(NumberOfTerrainUnitsUsed);
  
  if (getGameVersion() >= genie::GV_SWGB)
    serialize<char>(SWGBUnknown1, SWGBUNKNOWN1_LEN);
}

}
