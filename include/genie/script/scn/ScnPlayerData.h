/*
    genieutils - <description>
    Copyright (C) 2011 - 2013  Armin Preiml <email>
    Copyright (C) 2015  Mikko "Tapsa" P <email>

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

#ifndef GENIE_SCN_PLAYER_DATA_H
#define GENIE_SCN_PLAYER_DATA_H

#include "genie/file/ISerializable.h"
#include "ScnResource.h"

#include <stdint.h>

namespace genie
{

class ScnPlayerInfo : public ISerializable
{
public:
  ScnPlayerInfo();
  virtual ~ScnPlayerInfo();

  /// boolean
  uint32_t active;

  /// boolean
  uint32_t human;

  uint32_t civilizationID;

  /// constant = 4 ??
  uint32_t unknown1;

private:
  virtual void serializeObject(void);
};

class UnknownData1 : public ISerializable
{
public:
  UnknownData1();
  virtual ~UnknownData1();

  uint16_t unknownCount;
  uint16_t unknown2;
  float unknown3;

private:
  virtual void serializeObject(void);
};

class AiFile : public ISerializable
{
public:
  AiFile();
  virtual ~AiFile();

  uint32_t aiFilenameSize;
  uint32_t cityFileSize;
  uint32_t perFileSize;
  std::string aiFilename;
  std::string cityFilename;
  std::string perFilename;

private:
  virtual void serializeObject(void);

};

class ScnPlayerData1 : public ISerializable
{
public:
  ScnPlayerData1();
  virtual ~ScnPlayerData1();

  std::vector<std::string> playerNames;
  std::vector<uint32_t> playerNamesStringTable;
  std::vector<ScnPlayerInfo> playerInfo;
  uint8_t conquestVictory;
  UnknownData1 unknownData;
  std::string originalFileName;
  ScnMessagesCinematics messagesCinematics;

  std::vector<std::string> aiNames;
  std::vector<std::string> cityNames;
  std::vector<std::string> personalityNames;
  std::vector<AiFile> aiFiles;
  std::vector<uint8_t> aiTypes;

private:
  virtual void serializeObject(void);
};

class Resources : public ISerializable
{
public:
  Resources();
  virtual ~Resources();

  uint32_t gold;
  uint32_t wood;
  uint32_t food;
  uint32_t stone;

  /// ?
  uint32_t oreX;

  /// always 0
  uint32_t padding;

private:
  virtual void serializeObject(void);
};

class PlayerResources : public ISerializable
{
public:
  PlayerResources();
  virtual ~PlayerResources();

  std::vector<Resources> resources;

private:
  virtual void serializeObject(void);
};

class ScnVictory : public ISerializable
{
public:
  ScnVictory();
  virtual ~ScnVictory();

  uint32_t conquestRequired;
  uint32_t unused1;
  uint32_t numRelicsRequired;
  uint32_t unused2;
  uint32_t exploredPerCentRequired;
  uint32_t unused3;
  uint32_t allCustomConditionsRequired;
  uint32_t victoryMode;
  uint32_t scoreRequired;
  uint32_t timeForTimedGame;

private:
  virtual void serializeObject(void);
};

class ScnDiplomacy : public ISerializable
{
public:
  ScnDiplomacy();
  virtual ~ScnDiplomacy();

  std::vector<std::vector<uint32_t>> stances;
  std::vector<uint32_t> unused;

private:
  virtual void serializeObject(void);
};

class ScnDisables : public ISerializable
{
public:
  ScnDisables();
  virtual ~ScnDisables();

  std::vector<uint32_t> alliedVictory;
  std::vector<uint32_t> numDisabledTechs;
  std::vector<std::vector<uint32_t>> disabledTechs;
  std::vector<uint32_t> numDisabledUnits;
  std::vector<std::vector<uint32_t>> disabledUnits;
  std::vector<uint32_t> numDisabledBuildings;
  std::vector<std::vector<uint32_t>> disabledBuildings;
  uint32_t unused1;
  uint32_t unused2;
  uint32_t allTechs;
  std::vector<uint32_t> startingAge;

private:
  virtual void serializeObject(void);
};

}

#endif // GENIE_SCN_PLAYER_DATA_H
