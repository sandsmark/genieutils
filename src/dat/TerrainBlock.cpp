/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2014 - 2016  Mikko "Tapsa" P

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
  MapPointer = 0;
  Unknown1 = 0;
  MapWidth = 0;
  MapHeight = 0;
  WorldWidth = 0;
  WorldHeight = 0;
  Unknown2 = 0;
  UnknownPointer1 = 0;
  MapMinX = 0;
  MapMinY = 0;
  MapMaxX = 0;
  MapMaxY = 0;
  MapMaxXplus1 = 0;
  MapMaxYplus1 = 0;
  TerrainsUsed2 = 0;
  RemovedBlocksUsed = 0;
  BordersUsed = 0;
  MaxTerrain = 0;
  TileWidth = 0;
  TileHeight = 0;
  TileHalfHeight = 0;
  TileHalfWidth = 0;
  ElevHeight = 0;
  CurRow = 0;
  CurCol = 0;
  BlockBegRow = 0;
  BlockEndRow = 0;
  BlockBegCol = 0;
  BlockEndCol = 0;
  UnknownPointer2 = 0;
  UnknownPointer3 = 0;
  AnyFrameChange = 0;
  MapVisibleFlag = 0;
  FogFlag = 1;
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

  TileSizes.resize(SharedTerrain::TILE_TYPE_COUNT);
  SomeBytes.resize(getBytesSize());
  SomeInt32.resize(getSomethingSize());
}

//------------------------------------------------------------------------------
unsigned short TerrainBlock::getBytesSize(void)
{
  if (getGameVersion() >= genie::GV_SWGB)
    return 25;
  if (getGameVersion() >= genie::GV_AoKA)
    return 21;
  if (getGameVersion() >= genie::GV_AoEB)
    return 5;
  return 2;
}

//------------------------------------------------------------------------------
unsigned short TerrainBlock::getSomethingSize(void)
{
  if (getGameVersion() >= genie::GV_AoK)
    return 157;
  if (getGameVersion() >= genie::GV_AoKB)
    return 84;
  if (getGameVersion() >= genie::GV_AoKA)
    return 6;
  if (getGameVersion() >= genie::GV_AoEB)
    return 5;
  return 2625;
}

//------------------------------------------------------------------------------
/// Size in bytes
/// AoE Trial Beta  36264
/// AoE & RoR       36304
/// AoK A & B       36340
/// AoK             37208
/// TC              42208
/// SWGB & CC       49640
void TerrainBlock::serializeObject(void)
{
  serialize<int32_t>(MapPointer);
  serialize<int32_t>(Unknown1); // <-- this could be here or just before tile sizes
  serialize<int32_t>(MapWidth);
  serialize<int32_t>(MapHeight);
  serialize<int32_t>(WorldWidth);
  serialize<int32_t>(WorldHeight);

  serializeSub<TileSize>(TileSizes, SharedTerrain::TILE_TYPE_COUNT);
  if (getGameVersion() >= genie::GV_AoE)
    serialize<int16_t>(Unknown2);

  if (isOperation(OP_READ))
    serializeSub<Terrain>(Terrains, Terrain::getTerrainsSize(getGameVersion()));
  else
    serializeSub<Terrain>(Terrains, Terrains.size());

  if (getGameVersion() < genie::GV_AoEB)
    serialize<int16_t>(AoEAlphaUnknown, (16 * 1888) / 2);
  // TerrainBorders seem to be unused (are empty) in GV > AoK Alpha
  serializeSub<TerrainBorder>(TerrainBorders, 16); //TODO: fixed size?

  // Probably filled after loading map in game.
  serialize<int32_t>(UnknownPointer1);
  if (getGameVersion() >= genie::GV_AoKA)
  {
    serialize<float>(MapMinX);
    serialize<float>(MapMinY);
    serialize<float>(MapMaxX);
    serialize<float>(MapMaxY);
    if (getGameVersion() >= genie::GV_AoK)
    {
      serialize<float>(MapMaxXplus1);
      serialize<float>(MapMaxYplus1);
    }
  }

  serialize<uint16_t>(TerrainsUsed2);
  if (getGameVersion() < genie::GV_AoEB)
    serialize<uint16_t>(RemovedBlocksUsed);
  serialize<uint16_t>(BordersUsed);
  serialize<int16_t>(MaxTerrain);
  serialize<int16_t>(TileWidth);
  serialize<int16_t>(TileHeight);
  serialize<int16_t>(TileHalfHeight);
  serialize<int16_t>(TileHalfWidth);
  serialize<int16_t>(ElevHeight);
  serialize<int16_t>(CurRow);
  serialize<int16_t>(CurCol);
  serialize<int16_t>(BlockBegRow);
  serialize<int16_t>(BlockEndRow);
  serialize<int16_t>(BlockBegCol);
  serialize<int16_t>(BlockEndCol);

  if (getGameVersion() >= genie::GV_AoEB) // Maybe?
  {
    serialize<int32_t>(UnknownPointer2);
	serialize<int32_t>(UnknownPointer3);
    serialize<int8_t>(AnyFrameChange);
    serialize<int8_t>(MapVisibleFlag);
    serialize<int8_t>(FogFlag);
  }

  serialize<int8_t>(SomeBytes, getBytesSize());

  // Few pointers and small numbers.
  serialize<int32_t>(SomeInt32, getSomethingSize());
}

//------------------------------------------------------------------------------
TileSize::TileSize()
{
  Width = 0;
  Height = 0;
  DeltaY = 0;
}

//------------------------------------------------------------------------------
TileSize::~TileSize()
{
}

//------------------------------------------------------------------------------
void TileSize::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

//------------------------------------------------------------------------------
void TileSize::serializeObject(void)
{
  serialize<int16_t>(Width);
  serialize<int16_t>(Height);
  if (getGameVersion() >= genie::GV_AoE)
    serialize<int16_t>(DeltaY);
}

}
