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

class CombinedResources : public ISerializable
{
public:
  CombinedResources();
  virtual ~CombinedResources();

  uint32_t state;
  uint32_t type;
  uint32_t civilizationID;
  uint32_t unknown1;

  uint32_t gold;
  uint32_t wood;
  uint32_t food;
  uint32_t stone;
  uint32_t ore;
  uint32_t goods;

  static bool playerInfo;
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
  uint32_t allConditionsRequired;
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
  std::vector<std::vector<uint32_t>> individualVictory;

private:
  virtual void serializeObject(void);
};

class ScnDisables : public ISerializable
{
public:
  ScnDisables();
  virtual ~ScnDisables();

  std::vector<uint32_t> numDisabledTechs;
  std::vector<std::vector<uint32_t>> disabledTechs;
  std::vector<uint32_t> numDisabledUnits;
  std::vector<std::vector<uint32_t>> disabledUnits;
  std::vector<uint32_t> numDisabledBuildings;
  std::vector<std::vector<uint32_t>> disabledBuildings;

private:
  virtual void serializeObject(void);
};

class ScnPlayerData1 : public ISerializable
{
public:
  ScnPlayerData1();
  virtual ~ScnPlayerData1();

  float playerDataVersion;
  static float version;
  std::vector<std::string> playerNames;
  std::vector<uint32_t> playerNamesStringTable;
  uint8_t conquestVictory;
  UnknownData1 unknownData;
  std::string originalFileName;
  ScnMessagesCinematics messagesCinematics;

  std::vector<std::string> aiNames;
  std::vector<std::string> cityNames;
  std::vector<std::string> personalityNames;
  std::vector<AiFile> aiFiles;
  std::vector<uint8_t> aiTypes;

  std::vector<CombinedResources> resourcesPlusPlayerInfo;

  ScnVictory victoryConditions;
  ScnDiplomacy diplomacy;
  std::vector<uint32_t> alliedVictory;
  ScnDisables disables;
  uint32_t unused1;
  uint32_t unused2;
  uint32_t allTechs;
  std::vector<uint32_t> startingAge;

private:
  uint32_t separator_;
  void serializePlayerDataVersion(void);
  virtual void serializeObject(void);
};

class ScnPlayerData3Sub : public ISerializable
{
public:
  ScnPlayerData3Sub();
  virtual ~ScnPlayerData3Sub();

  std::string playerName;
  float initCameraX;
  float initCameraY;
  int16_t initCameraX2;
  int16_t initCameraY2;
  uint8_t alliedVictory;
  std::vector<uint8_t> diplomacy1;
  std::vector<uint32_t> diplomacy2;
  uint32_t playerColor;
  float unknown1;
  std::vector<uint8_t> unknown2;
  std::vector<uint8_t> unknown3;// found in Grand Theft Empires
  std::vector<uint8_t> unknown4;
  int32_t unknown5;

private:
  uint16_t playerCount_;
  uint16_t unknownCount_;
  virtual void serializeObject(void);
};

class ScnPlayerData3 : public ISerializable
{
public:
  ScnPlayerData3();
  virtual ~ScnPlayerData3();

  std::vector<ScnPlayerData3Sub> playerData;

private:
  uint32_t playerCount_;
  virtual void serializeObject(void);
};

}

#endif // GENIE_SCN_PLAYER_DATA_H
