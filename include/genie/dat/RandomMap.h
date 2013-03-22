/*
    geniedat - A library for reading and writing data files of genie
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

#ifndef GENIE_UNKNOWN_H
#define GENIE_UNKNOWN_H
#include "genie/file/ISerializable.h"

namespace genie
{

class MapUnit : public ISerializable
{
public:
  MapUnit();
  virtual ~MapUnit();
  virtual void setGameVersion(GameVersion gv);

  int32_t Unit;
  int32_t HostTerrain;
  static const unsigned short UNKNOWN3_SIZE = 4;
  std::array<int8_t, UNKNOWN3_SIZE> Unknown3;
  int32_t ObjectsPerPlayer;
  int32_t Unknown5;
  int32_t GroupsPerPlayer;
  int32_t Unknown7;
  int32_t OwnAtStart;
  int32_t SetPlaceForAllPlayers;
  int32_t MinDistanceToPlayers;
  int32_t MaxDistanceToPlayers;

private:
  virtual void serializeObject(void);
};

class MapTerrain : public ISerializable
{
public:
  MapTerrain();
  virtual ~MapTerrain();
  virtual void setGameVersion(GameVersion gv);

  int32_t Proportion;
  int32_t Terrain;
  int32_t NumberOfClumps;
  int32_t SpacingToOtherTerrains;
  int32_t PlacementZone;
  int32_t Unknown6;

private:
  virtual void serializeObject(void);
};

class BaseZone : public ISerializable
{
public:
  BaseZone();
  virtual ~BaseZone();
  virtual void setGameVersion(GameVersion gv);

  int32_t Unknown1;
  int32_t BaseTerrain;
  int32_t SpacingBetweenPlayers;
  int32_t Unknown4;
  static const unsigned short UNKNOWN5_SIZE = 4;
  std::array<int8_t, UNKNOWN5_SIZE> Unknown5;
  int32_t Unknown6;
  int32_t Unknown7;
  static const unsigned short UNKNOWN8_SIZE = 4;
  std::array<int8_t, UNKNOWN8_SIZE> Unknown8;
  int32_t StartAreaRadius;
  int32_t Unknown10;
  int32_t Unknown11;

private:
  virtual void serializeObject(void);
};

class Map : public ISerializable
{
public:
  Map();
  virtual ~Map();
  virtual void setGameVersion(GameVersion gv);

  int32_t BorderSouthWest;
  int32_t BorderNorthWest;
  int32_t BorderNorthEast;
  int32_t BorderSouthEast;
  int32_t BorderUsage;
  int32_t WaterShape;
  int32_t NonBaseTerrain;
  int32_t BaseZoneCoverage;
  int32_t Unknown9;

  int32_t BaseZonePointer;
  std::vector<BaseZone> BaseZones;
  int32_t MapTerrainPointer;
  std::vector<MapTerrain> MapTerrains;
  int32_t MapUnitPointer;
  std::vector<MapUnit> MapUnits;

  static const unsigned short UNKNOWN2_SIZE = 2;
  std::array<int32_t, UNKNOWN2_SIZE> Unknown2;

private:
  uint32_t BaseZoneCount;
  uint32_t MapTerrainCount;
  uint32_t MapUnitCount;

  virtual void serializeObject(void);
};

class MapHeader : public ISerializable
{
public:
  MapHeader();
  virtual ~MapHeader();
  virtual void setGameVersion(GameVersion gv);

  int32_t ScriptNumber;
  int32_t BorderSouthWest;
  int32_t BorderNorthWest;
  int32_t BorderNorthEast;
  int32_t BorderSouthEast;
  int32_t BorderUsage;
  int32_t WaterShape;
  int32_t NonBaseTerrain;
  int32_t BaseZoneCoverage;
  int32_t Unknown9;

  int32_t BaseZonePointer;
  int32_t MapTerrainPointer;
  int32_t MapUnitPointer;

  static const unsigned short UNKNOWN2_SIZE = 2;
  std::array<int32_t, UNKNOWN2_SIZE> Unknown2;

  uint32_t BaseZoneCount;
  uint32_t MapTerrainCount;
  uint32_t MapUnitCount;

private:
  virtual void serializeObject(void);
};

class RandomMaps : public ISerializable
{
public:
  RandomMaps();
  virtual ~RandomMaps();
  virtual void setGameVersion(GameVersion gv);

  int32_t RandomMapPointer;

  std::vector<MapHeader> MapHeaders;
  std::vector<Map> Maps;

private:
  uint32_t total_randommaps_count;

  virtual void serializeObject(void);
};

}

#endif // GENIE_UNKNOWN_H
