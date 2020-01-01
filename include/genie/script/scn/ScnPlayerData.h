/*
    genieutils - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2015 - 2016  Mikko "Tapsa" P
    Copyright (C) 2015  JustTesting1234

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

namespace genie {

class CombinedResources : public ISerializable
{
public:
    uint32_t enabled;
    uint32_t isHuman;
    uint32_t civilizationID = 0;
    uint32_t unknown1;

    uint32_t gold;
    uint32_t wood;
    uint32_t food;
    uint32_t stone;
    uint32_t ore;
    uint32_t goods;

    static bool playerInfo;

private:
    void serializeObject(void) override;
};

class TimelineEvent : public ISerializable
{
public:
    float timestamp; // 0

    enum TimelineCommands {
        Attack = 0,
        MakeObject = 1,
        MoveObject = 2,
        DestroyObject = 4
    };

    uint8_t command; // 4
    uint16_t objectType; // 6
    uint8_t playerId; // 7
    float x; // 8
    float y; // 12
    float z; // 16

    uint16_t task; // 20

    uint16_t objectId; // 22
    uint16_t targetId; // 24
    uint16_t targetPlayerId; // 28

private:
    void serializeObject(void) override;
};

class Timeline : public ISerializable
{
public:
    uint16_t entryCount;
    uint16_t availableId;
    float time;

    std::vector<TimelineEvent> events;

private:
    void serializeObject(void) override;
};

class AiFile : public ISerializable
{
public:
    uint32_t aiFilenameSize;
    uint32_t cityFileSize;
    uint32_t perFileSize;
    std::string aiFilename;
    std::string cityFilename;
    std::string perFilename;

private:
    void serializeObject(void) override;
};

class ScnVictory : public ISerializable
{
public:
    enum VictoryModes {
        Standard,
        Conquest,
        Score,
        Timed,
        Custom
    };

    uint32_t conquestRequired;
    uint32_t unused1;
    uint32_t numRelicsRequired;
    uint32_t unused2;
    uint32_t exploredPerCentRequired;
    uint32_t unused3;
    uint32_t allConditionsRequired;
    uint32_t victoryMode = 0;
    uint32_t scoreRequired = 0;
    uint32_t timeForTimedGame = 0;

private:
    void serializeObject(void) override;
};

class ScnDiplomacy : public ISerializable
{
public:
    std::array<std::array<uint32_t, 16>, 16> stances;
    std::array<std::array<uint32_t, 180>, 16> individualVictory;

private:
    void serializeObject(void) override;
};

class ScnDisables : public ISerializable
{
public:
    std::array<uint32_t, 16> numDisabledTechs;
    std::array<std::vector<uint32_t>, 16> disabledTechs;
    std::array<uint32_t, 16> numDisabledUnits;
    std::array<std::vector<uint32_t>, 16> disabledUnits;
    std::array<uint32_t, 16> numDisabledBuildings;
    std::array<std::vector<uint32_t>, 16> disabledBuildings;

private:
    void serializeObject(void) override;
};

class ScnMainPlayerData : public ISerializable
{
public:
    ~ScnMainPlayerData() override;

    float playerDataVersion = 0.f;
    static float version;
    std::array<std::string, 16> playerNames;
    std::array<uint32_t, 16> playerNamesStringTable;
    uint8_t conquestVictory = 0;
    Timeline timeline;
    std::string originalFileName = "";

    /// GV >= AoK
    uint32_t instructionsStringTable = -1;

    /// GV >= AoK
    uint32_t hintsStringTable = -1;

    /// GV >= AoK
    uint32_t victoryStringTable = -1;

    /// GV >= AoK
    uint32_t lossStringTable = -1;

    /// GV >= AoK
    uint32_t historyStringTable = -1;

    /// GV >= TC
    uint32_t scoutsStringTable = -1;

    // Messages

    std::string instructions;

    std::string hints;

    std::string victory;

    std::string loss;

    std::string history;

    /// GV >= TC
    std::string scouts;

    std::string oldFilename1;
    std::string oldFilename2;
    std::string oldFilename3;

    std::string pregameCinematicFilename;

    std::string victoryCinematicFilename;

    std::string lossCinematicFilename;

    std::string backgroundFilename;

    // Bitmap

    /// Boolean
    uint32_t bitmapIncluded = 0;

    uint32_t bitmapWidth = 0;

    uint32_t bitmapHeigth = 0;

    /// -1 if there's a bitmap, 1 otherwise
    int16_t hasBitmap = 1;

    uint16_t bitmapByteSize = 0;

    char *bmpHeader = nullptr;
    char *bitmap = nullptr;

    std::array<std::string, 16> aiNames;
    std::array<std::string, 16> cityNames;
    std::array<std::string, 16> personalityNames;
    std::array<AiFile, 16> aiFiles;
    std::array<uint8_t, 16> aiTypes;

    std::array<CombinedResources, 16> resourcesPlusPlayerInfo;

    ScnVictory victoryConditions;
    ScnDiplomacy diplomacy;
    std::vector<uint32_t> alliedVictory;
    ScnDisables disables;
    uint32_t unused1 = 0;
    uint32_t unused2 = 0;
    uint32_t allTechs = 0;
    std::array<int32_t, 16> startingAge;
    int32_t player1CameraX = 0;
    int32_t player1CameraY = 0;
    int32_t aiType = 0;

private:
    uint32_t separator_ = 0;
    void serializePlayerDataVersion(void);
    void serializeObject(void) override;
    void serializeBitmap(void);
};

class ScnPlayerVictoryCondition : public ISerializable
{
    // Need to find a scenario file with the matching struct filled in to use these enums
    enum PointState {
        PointsInactive,
        PointsAbove,
        PointsBelow
    };

    enum VictoryPointCondition {
        AttributeAmount,
        AttributeFirst,
        HighestAttribte,
        HighAttributeOnce,
        HighAttributeAmount
    };

public:
    enum Type : uint8_t {
        Capture = 0,
        Create = 1,
        Destroy = 2,
        DestroyMultiple = 3,
        BringToArea = 4,
        BringObject = 5,
        Attribute = 6,
        Explore = 7,
        CreateInArea = 8,
        DestroyAll = 9,
        DestroyPlayer = 10,
        VictoryPoints = 12
    };

    uint8_t type;
    int32_t objectType; // for create, create in area, destroy, destroy multiple,  destroy all
    int32_t targetPlayer;

    float x0;
    float y0;
    float x1;
    float y1;

    int32_t number; // for an Attribute condition, this is the attribute type, and count is the amount
    int32_t count;
    int32_t object;
    int32_t target;
    uint8_t victoryGroup;
    uint8_t allyFlag;
    uint8_t state;

private:
    void serializeObject(void) override;
};

class ScnMorePlayerData : public ISerializable
{
public:
    std::string playerName;
    float initCameraX = 0.f;
    float initCameraY = 0.f;
    int16_t initCameraX2 = 0;
    int16_t initCameraY2 = 0;
    uint8_t alliedVictory = 0;
    std::vector<uint8_t> diplomacy1;
    std::vector<uint32_t> diplomacy2;
    uint32_t playerColor = 0;

    float victoryConditionVersion = 0.f;
    std::vector<ScnPlayerVictoryCondition> victoryConditions; // found in Grand Theft Empires, and the aztec campaign
    int32_t playerID = -1;
    uint8_t victory;

    int32_t unknown1;
    int32_t unknown2;

    uint32_t totalVictoryPoints;

private:
    uint16_t playerCount_;
    uint32_t victoryConditionsCount;
    uint32_t pointConditionsCount;
    void serializeObject(void) override;
};
} // namespace genie

#endif // GENIE_SCN_PLAYER_DATA_H
