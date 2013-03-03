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

class ThirdSubData : public ISerializable
{
public:
  ThirdSubData();
  virtual ~ThirdSubData();

  int32_t Unit;
  int32_t HostTerrain;
  static const unsigned short UNKNOWN3_LEN = 4;
  std::array<char, UNKNOWN3_LEN> Unknown3;
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

class SecondSubData : public ISerializable
{
public:
  SecondSubData();
  virtual ~SecondSubData();

  int32_t Proportion;
  int32_t Terrain;
  int32_t NumberOfClumps;
  int32_t SpacingToOtherTerrains;
  int32_t PlacementZone;
  int32_t Unknown6;

private:
  virtual void serializeObject(void);
};

class FirstSubData : public ISerializable
{
public:
  FirstSubData();
  virtual ~FirstSubData();

  int32_t Unknown1;
  int32_t BaseTerrain;
  int32_t SpacingBetweenPlayers;
  int32_t Unknown4;
  static const unsigned short UNKNOWN5_LEN = 4;
  std::array<char, UNKNOWN5_LEN> Unknown5;
  int32_t Unknown6;
  int32_t Unknown7;
  static const unsigned short UNKNOWN8_LEN = 4;
  std::array<char, UNKNOWN8_LEN> Unknown8;
  int32_t StartAreaRadius;
  int32_t Unknown10;
  int32_t Unknown11;

private:
  virtual void serializeObject(void);
};

class Unknown2ndBlock : public ISerializable
{
public:
  Unknown2ndBlock();
  virtual ~Unknown2ndBlock();

  int32_t BorderSouthWest;
  int32_t BorderNorthWest;
  int32_t BorderNorthEast;
  int32_t BorderSouthEast;
  int32_t BorderUsage;
  int32_t WaterShape;
  int32_t NonBaseTerrain;
  int32_t BaseZoneCoverage;
  int32_t Unknown9;

  int32_t Pointer1;
  std::vector<FirstSubData> FirstSubDatas;
  int32_t Pointer2;
  std::vector<SecondSubData> SecondSubDatas;
  int32_t Pointer3;
  std::vector<ThirdSubData> ThirdSubDatas;

  static const unsigned short UNKNOWN2_LEN = 2;
  std::array<int32_t, UNKNOWN2_LEN> Unknown2;

private:
  uint32_t Count1;
  uint32_t Count2;
  uint32_t Count3;

  virtual void serializeObject(void);
};

class Unknown1stBlock : public ISerializable
{
public:
  Unknown1stBlock();
  virtual ~Unknown1stBlock();

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

  int32_t Pointer1;
  int32_t Pointer2;
  int32_t Pointer3;

  static const unsigned short UNKNOWN2_LEN = 2;
  std::array<int32_t, UNKNOWN2_LEN> Unknown2;

  uint32_t Count1;
  uint32_t Count2;
  uint32_t Count3;

private:
  virtual void serializeObject(void);
};

class Unknown : public ISerializable
{
public:
  Unknown();
  virtual ~Unknown();

  int32_t Pointer;

  std::vector<Unknown1stBlock> Unknown1stBlocks;
  std::vector<Unknown2ndBlock> Unknown2ndBlocks;

private:
  uint32_t total_unknown_count_;

  virtual void serializeObject(void);
};

}

#endif // GENIE_UNKNOWN_H
