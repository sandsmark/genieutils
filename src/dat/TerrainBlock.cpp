/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2014 - 2017  Mikko "Tapsa" P

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

  Terrains.resize(Terrain::getTerrainCount(gv));
  updateGameVersion(Terrains);
  updateGameVersion(TerrainBorders);

  TileSizes.resize(SharedTerrain::TILE_TYPE_COUNT);
  SomeBytes.resize(getBytesSize());
  SomeInt32.resize(getSomethingSize());
}

//------------------------------------------------------------------------------
unsigned short TerrainBlock::getBytesSize(void)
{
  GameVersion gv = getGameVersion();
  if (gv >= GV_SWGB)
    return 25;
  if (gv >= GV_AoKA)
    return 21;
  if (gv >= GV_AoKE3)
    return 17;
  if (gv >= GV_AoEB)
    return 2;// Padding
  return 0;
}

//------------------------------------------------------------------------------
unsigned short TerrainBlock::getSomethingSize(void)
{
  GameVersion gv = getGameVersion();
  if (gv >= GV_AoK)
    return 157;
  if (gv >= GV_AoKB)
    return 84;
  if (gv >= GV_AoKE3)
    return 6;
  if (gv >= GV_AoEB)
    return 5;// 5 pointers
  return 65;
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
  GameVersion gv = getGameVersion();

  serialize<int32_t>(VirtualFunctionPtr);
  serialize<int32_t>(MapPointer);
  serialize<int32_t>(MapWidth);
  serialize<int32_t>(MapHeight);
  serialize<int32_t>(WorldWidth);
  serialize<int32_t>(WorldHeight);

  serializeSub<TileSize>(TileSizes, SharedTerrain::TILE_TYPE_COUNT);
  if (gv >= GV_AoE)
    serialize<int16_t>(PaddingTS);// Padding for TileSizes (32-bit aligned)

  if (isOperation(OP_READ))
    serializeSub<Terrain>(Terrains, Terrain::getTerrainCount(gv));
  else
    serializeSub<Terrain>(Terrains, Terrains.size());

  std::cout << "Terrains: " << Terrains.size() << std::endl;

  if (gv < GV_AoEB)
    serialize<int16_t>(AoEAlphaUnknown, (16 * 1888) / 2);
  // TerrainBorders seem to be unused (are empty) in GV > AoK Alpha
  serializeSub<TerrainBorder>(TerrainBorders, 16); //TODO: fixed size?

  // Probably filled after loading map in game.
  serialize<int32_t>(MapRowOffset);
  if (gv >= GV_AoKA)
  {
    serialize<float>(MapMinX);
    serialize<float>(MapMinY);
    serialize<float>(MapMaxX);
    serialize<float>(MapMaxY);
    if (gv >= GV_AoK)
    {
      serialize<float>(MapMaxXplus1);
      serialize<float>(MapMaxYplus1);
    }
  }

  serialize<uint16_t>(TerrainsUsed2);
  if (gv < GV_AoEB)
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

  if (gv >= GV_AoKE3)
  {
    serialize<int32_t>(SearchMapPtr);
    serialize<int32_t>(SearchMapRowsPtr);
    serialize<int8_t>(AnyFrameChange);
  }
  else
  {
    // Padding fix
    int32_t any = AnyFrameChange;
    serialize<int32_t>(any);
    AnyFrameChange = any;

    serialize<int32_t>(SearchMapPtr);
    serialize<int32_t>(SearchMapRowsPtr);
  }
  serialize<int8_t>(MapVisibleFlag);
  serialize<int8_t>(FogFlag); // Always 1

//From here on data is filled in game anyway.
//There are two 32-bit pointers random map and game world, rest should be all 0.
  serialize<int8_t>(SomeBytes, getBytesSize());

  // Few pointers and small numbers.
  serialize<int32_t>(SomeInt32, getSomethingSize());
}

//------------------------------------------------------------------------------
TileSize::TileSize()
{
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
  if (getGameVersion() >= GV_AoE)
    serialize<int16_t>(DeltaY);
}

}
