/*
    genie/dat - A library for reading and writing data files of genie
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

#include "genie/dat/Terrain.h"

namespace genie
{

//------------------------------------------------------------------------------
Terrain::Terrain() : Colors(), Unknown5(), Unknown7(), ElevationGraphics(),
  TerrainUnitID(), TerrainUnitDensity(), TerrainUnitPriority()
{
  Unknown1 = 0;
  Enabled = 1;
  Name = "";
  Name2 = "";
  SLP = -1;
  Unknown3 = 0;
  SoundID = -1;
  BlendPriority = -1;
  BlendType = -1;
  Terrain1 = -1;
  Terrain2 = -1;
  FrameCount = 0;
  AngleCount = 0;
  TerrainID = 0;
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

  TerrainBorderIDs.resize(getTerrainsSize(gv));
}

//------------------------------------------------------------------------------
unsigned short Terrain::getTerrainsSize(GameVersion gv)
{
  if (gv >= genie::GV_SWGB)
    return 55;
  if (gv >= genie::GV_Cysion)
    return 47; // 200 pls
  if (gv >= genie::GV_TC)
    return 42;
    return 32;
}

//------------------------------------------------------------------------------
unsigned short Terrain::getNameSize()
{
  if (getGameVersion() >= genie::GV_SWGB)
    return 17;
  else
    return 13;
}

//------------------------------------------------------------------------------
void Terrain::serializeObject(void)
{
  serialize<int16_t>(Enabled);

  serialize<std::string>(Name, getNameSize());
  serialize<std::string>(Name2, getNameSize());

  if (getGameVersion() >= genie::GV_AoEB)
    serialize<int32_t>(SLP);
  serialize<float>(Unknown3);
  serialize<int32_t>(SoundID);

  if (getGameVersion() >= genie::GV_AoKB)
  {
    serialize<int32_t>(BlendPriority);
    serialize<int32_t>(BlendType);
  }

  serialize<uint8_t, 3>(Colors);
  serialize<uint8_t, 2>(Unknown5); // 1st and 2nd are colors, 3rd and 4th are terrains
  serialize<int8_t>(Terrain1);
  serialize<int8_t>(Terrain2);
  serialize<int8_t, TERRAINS_USED_AOE>(Unknown7);
  serialize<int16_t>(FrameCount);
  serialize<int16_t>(AngleCount);
  serialize<int16_t>(TerrainID);
  serialize<int16_t, ELEVATION_GRAPHICS_SIZE>(ElevationGraphics);
  serialize<int16_t>(TerrainReplacementID);
  serializePair<int16_t>(TerrainDimensions);
  if (isOperation(OP_READ))
    serialize<int16_t>(TerrainBorderIDs, getTerrainsSize(getGameVersion()));
  else
    serialize<int16_t>(TerrainBorderIDs, TerrainBorderIDs.size());
  serialize<int16_t, TERRAIN_UNITS_SIZE>(TerrainUnitID);
  serialize<int16_t, TERRAIN_UNITS_SIZE>(TerrainUnitDensity);
  serialize<int8_t, TERRAIN_UNITS_SIZE>(TerrainUnitPriority);
  serialize<int16_t>(NumberOfTerrainUnitsUsed);

  if (getGameVersion() < genie::GV_SWGB)
    serialize<int16_t>(Unknown1);
}

}
