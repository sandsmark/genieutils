/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2014  Mikko T P

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

#include "genie/dat/TerrainBlock.h"

namespace genie
{

//------------------------------------------------------------------------------
TerrainBlock::TerrainBlock()
{
}

//------------------------------------------------------------------------------
TerrainBlock::~TerrainBlock()
{
}

//------------------------------------------------------------------------------
void TerrainBlock::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  updateGameVersion(Terrains);
  updateGameVersion(TerrainBorders);

  ZeroSpace.resize(getZeroSpaceSize());
  Rendering.resize(getRenderingSize());
  Something.resize(getSomethingSize());
}

//------------------------------------------------------------------------------
unsigned short TerrainBlock::getZeroSpaceSize(void)
{
  switch (getGameVersion())
  {
    case genie::GV_AoE:
    case genie::GV_RoR:  return 2;
    case genie::GV_AoKA: return 10;
    case genie::GV_AoK:
    case genie::GV_TC:
    case genie::GV_SWGB:
    case genie::GV_CC:   return 14;
    default: return 0;
  }
}

//------------------------------------------------------------------------------
unsigned short TerrainBlock::getRenderingSize(void)
{
  switch (getGameVersion())
  {
    case genie::GV_AoE:
    case genie::GV_RoR:  return 21;
    case genie::GV_AoKA:
    case genie::GV_AoK:
    case genie::GV_TC:   return 29;
    case genie::GV_SWGB:
    case genie::GV_CC:   return 31;
    default: return 0;
  }
}

//------------------------------------------------------------------------------
unsigned short TerrainBlock::getSomethingSize(void)
{
  switch (getGameVersion())
  {
    case genie::GV_AoE:
    case genie::GV_RoR:  return 5;
    case genie::GV_AoKA: return 6;
    case genie::GV_AoK:
    case genie::GV_TC:
    case genie::GV_SWGB:
    case genie::GV_CC:   return 157;
    default: return 0;
  }
}

//------------------------------------------------------------------------------
/// Size in bytes
/// AoE Trial Beta	36264
/// AoE & RoR		36304
/// AoK A & B		36340
/// AoK				37208
/// TC				42208
/// SWGB & CC		49640
void TerrainBlock::serializeObject(void)
{
  //TODO: AoE/RoR: maybe:
  // struct { short unknown[3]; } terrainheader[terrain_count_]
  serialize<int16_t, TERRAIN_HEADER_SIZE>(GraphicsRendering);

  serializeSub<Terrain>(Terrains, Terrain::getTerrainsSize(getGameVersion()));

  // TerrainBorders seem to be unused (are empty) in GV > AoK Alpha
  serializeSub<TerrainBorder>(TerrainBorders, 16); //TODO: fixed size?

  // Empty space.
  serialize<int16_t>(ZeroSpace, getZeroSpaceSize());

  serialize<uint16_t>(NumberOfTerrainsUsed2);

  serialize<int16_t>(Rendering, getRenderingSize());

  // Few pointers and small numbers.
  serialize<int32_t>(Something, getSomethingSize());
}

}
