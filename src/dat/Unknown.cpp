/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
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


#include "genie/dat/Unknown.h"

namespace genie
{

//------------------------------------------------------------------------------
Unknown::Unknown()
{
  Pointer = 0;
}

//------------------------------------------------------------------------------
Unknown::~Unknown()
{
}

//------------------------------------------------------------------------------
void Unknown::serializeObject()
{
  serializeSize<uint32_t>(total_unknown_count_, Unknown2ndBlocks.size());

  serialize<int32_t>(Pointer);

  serializeSub<Unknown1stBlock>(Unknown1stBlocks, total_unknown_count_);
  serializeSub<Unknown2ndBlock>(Unknown2ndBlocks, total_unknown_count_);
}

//------------------------------------------------------------------------------
Unknown1stBlock::Unknown1stBlock() : Unknown2()
{
  ScriptNumber = 0;
  BorderSouthWest = 0;
  BorderNorthWest = 0;
  BorderNorthEast = 0;
  BorderSouthEast = 0;
  BorderUsage = 0;
  WaterShape = 10;
  NonBaseTerrain = 1;
  BaseZoneCoverage = 80;
  Unknown9 = 0;
  Pointer1 = 0;
  Pointer2 = 0;
  Pointer3 = 0;
  // Remove these after you have made them automatic.
  Count1 = 0;
  Count2 = 0;
  Count3 = 0;
}

//------------------------------------------------------------------------------
Unknown1stBlock::~Unknown1stBlock()
{
}

//------------------------------------------------------------------------------
void Unknown1stBlock::serializeObject(void)
{
  serialize<int32_t>(ScriptNumber);
  serialize<int32_t>(BorderSouthWest);
  serialize<int32_t>(BorderNorthWest);
  serialize<int32_t>(BorderNorthEast);
  serialize<int32_t>(BorderSouthEast);
  serialize<int32_t>(BorderUsage);
  serialize<int32_t>(WaterShape);
  serialize<int32_t>(NonBaseTerrain);
  serialize<int32_t>(BaseZoneCoverage);
  serialize<int32_t>(Unknown9);
  serialize<uint32_t>(Count1);
  serialize<int32_t>(Pointer1);
  serialize<uint32_t>(Count2);
  serialize<int32_t>(Pointer2);
  serialize<uint32_t>(Count3);
  serialize<int32_t>(Pointer3);
  serialize<int32_t, UNKNOWN2_LEN>(Unknown2);
}

//------------------------------------------------------------------------------
Unknown2ndBlock::Unknown2ndBlock() : Unknown2()
{
  BorderSouthWest = 0;
  BorderNorthWest = 0;
  BorderNorthEast = 0;
  BorderSouthEast = 0;
  BorderUsage = 0;
  WaterShape = 10;
  NonBaseTerrain = 1;
  BaseZoneCoverage = 80;
  Unknown9 = 0;
  Pointer1 = 0;
  Pointer2 = 0;
  Pointer3 = 0;
}

//------------------------------------------------------------------------------
Unknown2ndBlock::~Unknown2ndBlock()
{
}

//------------------------------------------------------------------------------
void Unknown2ndBlock::serializeObject(void)
{
  serialize<int32_t>(BorderSouthWest);
  serialize<int32_t>(BorderNorthWest);
  serialize<int32_t>(BorderNorthEast);
  serialize<int32_t>(BorderSouthEast);
  serialize<int32_t>(BorderUsage);
  serialize<int32_t>(WaterShape);
  serialize<int32_t>(NonBaseTerrain);
  serialize<int32_t>(BaseZoneCoverage);
  serialize<int32_t>(Unknown9);

  serializeSize<uint32_t>(Count1, FirstSubDatas.size());
  serialize<int32_t>(Pointer1);
  serializeSub<FirstSubData>(FirstSubDatas, Count1);

  serializeSize<uint32_t>(Count2, SecondSubDatas.size());
  serialize<int32_t>(Pointer2);
  serializeSub<SecondSubData>(SecondSubDatas, Count2);

  serializeSize<uint32_t>(Count3, ThirdSubDatas.size());
  serialize<int32_t>(Pointer3);
  serializeSub<ThirdSubData>(ThirdSubDatas, Count3);

  serialize<int32_t, UNKNOWN2_LEN>(Unknown2);
}

//------------------------------------------------------------------------------
FirstSubData::FirstSubData() : Unknown5(), Unknown8()
{
  Unknown1 = 1;
  BaseTerrain = 0;
  SpacingBetweenPlayers = 2;
  Unknown4 = 7;
  Unknown5[1] = 1;
  Unknown6 = 0;
  Unknown7 = 0;
  Unknown8[0] = 100;
  Unknown8[1] = 1;
  StartAreaRadius = 10;
  Unknown10 = 25;
  Unknown11 = 8;
}

//------------------------------------------------------------------------------
FirstSubData::~FirstSubData()
{
}

//------------------------------------------------------------------------------
void FirstSubData::serializeObject(void)
{
  serialize<int32_t>(Unknown1);
  serialize<int32_t>(BaseTerrain);
  serialize<int32_t>(SpacingBetweenPlayers);
  serialize<int32_t>(Unknown4);
  serialize<char, UNKNOWN5_LEN>(Unknown5);
  serialize<int32_t>(Unknown6);
  serialize<int32_t>(Unknown7);
  serialize<char, UNKNOWN8_LEN>(Unknown8);
  serialize<int32_t>(StartAreaRadius);
  serialize<int32_t>(Unknown10);
  serialize<int32_t>(Unknown11);
}

//------------------------------------------------------------------------------
SecondSubData::SecondSubData()
{
  Proportion = 0;
  Terrain = -1;
  NumberOfClumps = 1;
  SpacingToOtherTerrains = 1;
  PlacementZone = 0;
  Unknown6 = 20;
}

//------------------------------------------------------------------------------
SecondSubData::~SecondSubData()
{
}

//------------------------------------------------------------------------------
void SecondSubData::serializeObject(void)
{
  serialize<int32_t>(Proportion);
  serialize<int32_t>(Terrain);
  serialize<int32_t>(NumberOfClumps);
  serialize<int32_t>(SpacingToOtherTerrains);
  serialize<int32_t>(PlacementZone);
  serialize<int32_t>(Unknown6);
}

//------------------------------------------------------------------------------
ThirdSubData::ThirdSubData() : Unknown3()
{
  Unit = -1;
  HostTerrain = -1;
  ObjectsPerPlayer = 1;
  Unknown5 = 0;
  GroupsPerPlayer = 1;
  Unknown7 = 1;
  OwnAtStart = 0;
  SetPlaceForAllPlayers = 1;
  MinDistanceToPlayers = 2;
  MaxDistanceToPlayers = 6;
}

//------------------------------------------------------------------------------
ThirdSubData::~ThirdSubData()
{
}

//------------------------------------------------------------------------------
void ThirdSubData::serializeObject(void)
{
  serialize<int32_t>(Unit);
  serialize<int32_t>(HostTerrain);
  serialize<char, UNKNOWN3_LEN>(Unknown3);
  serialize<int32_t>(ObjectsPerPlayer);
  serialize<int32_t>(Unknown5);
  serialize<int32_t>(GroupsPerPlayer);
  serialize<int32_t>(Unknown7);
  serialize<int32_t>(OwnAtStart);
  serialize<int32_t>(SetPlaceForAllPlayers);
  serialize<int32_t>(MinDistanceToPlayers);
  serialize<int32_t>(MaxDistanceToPlayers);
}

}
