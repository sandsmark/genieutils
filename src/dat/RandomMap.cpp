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

#include "genie/dat/RandomMap.h"

namespace genie
{

//------------------------------------------------------------------------------
RandomMaps::RandomMaps()
{
  RandomMapPointer = 0;
}

//------------------------------------------------------------------------------
RandomMaps::~RandomMaps()
{
}

//------------------------------------------------------------------------------
void RandomMaps::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  updateGameVersion(MapHeaders);
  updateGameVersion(Maps);
}

//------------------------------------------------------------------------------
void RandomMaps::serializeObject()
{
  serializeSize<uint32_t>(total_randommaps_count, Maps.size());

  serialize<int32_t>(RandomMapPointer);

  serializeSub<MapHeader>(MapHeaders, total_randommaps_count);
  serializeSub<Map>(Maps, total_randommaps_count);
}

//------------------------------------------------------------------------------
MapHeader::MapHeader() : Unknown2()
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
  BaseZonePointer = 0;
  MapTerrainPointer = 0;
  MapUnitPointer = 0;
  // Remove these after you have made them automatic.
  BaseZoneCount = 0;
  MapTerrainCount = 0;
  MapUnitCount = 0;
}

//------------------------------------------------------------------------------
MapHeader::~MapHeader()
{
}

//------------------------------------------------------------------------------
void MapHeader::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

//------------------------------------------------------------------------------
void MapHeader::serializeObject(void)
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
  serialize<uint32_t>(BaseZoneCount);
  serialize<int32_t>(BaseZonePointer);
  serialize<uint32_t>(MapTerrainCount);
  serialize<int32_t>(MapTerrainPointer);
  serialize<uint32_t>(MapUnitCount);
  serialize<int32_t>(MapUnitPointer);
  serialize<int32_t, UNKNOWN2_SIZE>(Unknown2);
}

//------------------------------------------------------------------------------
Map::Map() : Unknown2()
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
  BaseZonePointer = 0;
  MapTerrainPointer = 0;
  MapUnitPointer = 0;
}

//------------------------------------------------------------------------------
Map::~Map()
{
}

//------------------------------------------------------------------------------
void Map::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  updateGameVersion(BaseZones);
  updateGameVersion(MapTerrains);
  updateGameVersion(MapUnits);
}

//------------------------------------------------------------------------------
void Map::serializeObject(void)
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

  serializeSize<uint32_t>(BaseZoneCount, BaseZones.size());
  serialize<int32_t>(BaseZonePointer);
  serializeSub<BaseZone>(BaseZones, BaseZoneCount);

  serializeSize<uint32_t>(MapTerrainCount, MapTerrains.size());
  serialize<int32_t>(MapTerrainPointer);
  serializeSub<MapTerrain>(MapTerrains, MapTerrainCount);

  serializeSize<uint32_t>(MapUnitCount, MapUnits.size());
  serialize<int32_t>(MapUnitPointer);
  serializeSub<MapUnit>(MapUnits, MapUnitCount);

  serialize<int32_t, UNKNOWN2_SIZE>(Unknown2);
}

//------------------------------------------------------------------------------
BaseZone::BaseZone() : Unknown5(), Unknown8()
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
BaseZone::~BaseZone()
{
}

//------------------------------------------------------------------------------
void BaseZone::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

//------------------------------------------------------------------------------
void BaseZone::serializeObject(void)
{
  serialize<int32_t>(Unknown1);
  serialize<int32_t>(BaseTerrain);
  serialize<int32_t>(SpacingBetweenPlayers);
  serialize<int32_t>(Unknown4);
  serialize<int8_t, UNKNOWN5_SIZE>(Unknown5);
  serialize<int32_t>(Unknown6);
  serialize<int32_t>(Unknown7);
  serialize<int8_t, UNKNOWN8_SIZE>(Unknown8);
  serialize<int32_t>(StartAreaRadius);
  serialize<int32_t>(Unknown10);
  serialize<int32_t>(Unknown11);
}

//------------------------------------------------------------------------------
MapTerrain::MapTerrain()
{
  Proportion = 0;
  Terrain = -1;
  NumberOfClumps = 1;
  SpacingToOtherTerrains = 1;
  PlacementZone = 0;
  Unknown6 = 20;
}

//------------------------------------------------------------------------------
MapTerrain::~MapTerrain()
{
}

//------------------------------------------------------------------------------
void MapTerrain::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

//------------------------------------------------------------------------------
void MapTerrain::serializeObject(void)
{
  serialize<int32_t>(Proportion);
  serialize<int32_t>(Terrain);
  serialize<int32_t>(NumberOfClumps);
  serialize<int32_t>(SpacingToOtherTerrains);
  serialize<int32_t>(PlacementZone);
  serialize<int32_t>(Unknown6);
}

//------------------------------------------------------------------------------
MapUnit::MapUnit() : Unknown3()
{
  Unit = -1;
  HostTerrain = -1;
  ObjectsPerGroup = 1;
  Fluctuation = 0;
  GroupsPerPlayer = 1;
  GroupRadius = 1;
  OwnAtStart = 0;
  SetPlaceForAllPlayers = 1;
  MinDistanceToPlayers = 2;
  MaxDistanceToPlayers = 6;
}

//------------------------------------------------------------------------------
MapUnit::~MapUnit()
{
}

//------------------------------------------------------------------------------
void MapUnit::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

//------------------------------------------------------------------------------
void MapUnit::serializeObject(void)
{
  serialize<int32_t>(Unit);
  serialize<int32_t>(HostTerrain);
  serialize<int8_t, UNKNOWN3_SIZE>(Unknown3);
  serialize<int32_t>(ObjectsPerGroup);
  serialize<int32_t>(Fluctuation);
  serialize<int32_t>(GroupsPerPlayer);
  serialize<int32_t>(GroupRadius);
  serialize<int32_t>(OwnAtStart);
  serialize<int32_t>(SetPlaceForAllPlayers);
  serialize<int32_t>(MinDistanceToPlayers);
  serialize<int32_t>(MaxDistanceToPlayers);
}

//------------------------------------------------------------------------------
MapUnknown::MapUnknown()
{
  Unknown1 = 0;
  Unknown2 = 0;
  Unknown3 = 0;
  Unknown4 = 0;
  Unknown5 = 0;
  Unknown6 = 0;
}

//------------------------------------------------------------------------------
MapUnknown::~MapUnknown()
{
}

//------------------------------------------------------------------------------
void MapUnknown::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

//------------------------------------------------------------------------------
void MapUnknown::serializeObject(void)
{
  serialize<int32_t>(Unknown1);
  serialize<int32_t>(Unknown2);
  serialize<int32_t>(Unknown3);
  serialize<int32_t>(Unknown4);
  serialize<int32_t>(Unknown5);
  serialize<int32_t>(Unknown6);
}

}
