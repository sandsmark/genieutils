/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2016  Mikko "Tapsa" P

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
  if (getGameVersion() < genie::GV_AoEB)
  {
    serializeSize<uint32_t>(total_randommaps_count, OldMaps.size());

    if (isOperation(OP_READ))
      OldMaps.resize(total_randommaps_count);
    for (auto &sub: OldMaps)
      serialize<int32_t>(sub, 852);
  }
  else
  {
    serializeSize<uint32_t>(total_randommaps_count, Maps.size());

    serialize<int32_t>(RandomMapPointer);

    serializeSub<MapHeader>(MapHeaders, total_randommaps_count);
    serializeSub<Map>(Maps, total_randommaps_count);
  }
}

//------------------------------------------------------------------------------
MapHeader::MapHeader()
{
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
  serialize<uint32_t>(MapUnknownCount);
  serialize<int32_t>(MapUnknownPointer);
}

//------------------------------------------------------------------------------
Map::Map()
{
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
  updateGameVersion(MapUnknowns);
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

  serializeSize<uint32_t>(MapUnknownCount, MapUnknowns.size());
  serialize<int32_t>(MapUnknownPointer);
  serializeSub<MapUnknown>(MapUnknowns, MapUnknownCount);
}

//------------------------------------------------------------------------------
BaseZone::BaseZone()
{
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
  serialize<int8_t>(Unknown5, 4);
  serialize<int32_t>(Unknown6);
  serialize<int32_t>(Unknown7);
  serialize<int8_t>(Unknown8, 4);
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
MapUnit::MapUnit()
{
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
  serialize<int8_t>(Unknown3, 4);
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
