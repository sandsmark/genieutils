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
    /// If this player's stuff is defined by this
    uint32_t enabled = 0;

    /// Is this player human
    uint32_t isHuman = 0;

    /// ID of player civ
    uint32_t civilizationID = 0;

    /// Beats me
    uint32_t unknown1 = 0;

    /// Initial amount of gold
    uint32_t gold = 0;

    /// Initial amount of wood
    uint32_t wood = 0;

    /// Initial amount of food
    uint32_t food = 0;

    /// Initial amount of stone
    uint32_t stone = 0;

    /// Initial amount of ore
    uint32_t ore = 0;

    /// Initial amount of goods
    uint32_t goods = 0;

    /// Hack to reuse this for (de)serializing player info
    /// Not sure if apreiml or tapsa was lazy
    static bool playerInfo;

private:
    void serializeObject(void) override;
};

class TimelineEvent : public ISerializable
{
public:
    /// Time when it happened
    float timestamp; // 0

    enum TimelineCommands : uint8_t {
        /// Invalid/unset
        Invalid = 255,

        /// Someone attacked something
        Attack = 0,

        /// Something was created or trained
        MakeObject = 1,

        /// Something moved
        MoveObject = 2,

        /// Something was destroyed
        DestroyObject = 4
    };

    /// Command that was triggered
    TimelineCommands command = Invalid; // 4

    /// Object type that was affected
    uint16_t objectType = 0; // 6

    /// Player that initiated the command
    uint8_t playerId = 0; // 7

    /// X position of event
    float x = 0.f; // 8

    /// Y position of event
    float y = 0.f; // 12

    /// Height of event
    float z = 0.f; // 16

    /// Which task was associated with the event
    uint16_t task = 0; // 20

    /// ID of object that triggered event
    uint16_t objectId = 0; // 22

    /// Target of object that triggered event
    uint16_t targetId = 0; // 24

    /// Owner of the target object
    uint16_t targetPlayerId = 0; // 28

private:
    void serializeObject(void) override;
};

class Timeline : public ISerializable
{
public:
    /// Number of events in timeline
    uint16_t entryCount = 0;

    /// Next unused event ID
    uint16_t availableId = 0;

    /// Time of last event
    float time = 0.f;

    std::vector<TimelineEvent> events;

private:
    void serializeObject(void) override;
};

class AiFile : public ISerializable
{
public:
    /// Length of filename
    uint32_t aiFilenameSize = 0;

    /// Size of city definition file
    uint32_t cityFileSize = 0;

    /// Personality file size
    uint32_t perFileSize = 0;

    /// AI script filename
    std::string aiFilename;

    /// City filename
    std::string cityFilename;

    /// Personality filename
    std::string perFilename;

private:
    void serializeObject(void) override;
};

class ScnVictory : public ISerializable
{
public:
    enum VictoryModes : uint32_t {
        Standard = 0,
        Conquest = 1,
        Score = 2,
        Timed = 3,
        Custom = 4
    };

    /// Needs to kill everyone else
    uint32_t conquestRequired = 0;

    /// Just for memory padding, I think
    uint32_t unused1 = 0;

    /// Needs to capture at least this many relics
    uint32_t numRelicsRequired = 0;

    /// More padding, I think
    uint32_t unused2 = 0;

    /// Needs to explore at least this many % of the map
    uint32_t exploredPerCentRequired = 0;

    /// Even more padding? Or am I missing something
    uint32_t unused3 = 0;

    /// Do all conditions need to be satisfied, or just one
    uint32_t allConditionsRequired = 0;

    /// What kind of victory
    VictoryModes victoryMode = Standard;

    /// Minimum score required
    uint32_t scoreRequired = 0;

    /// Max playtime
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

    /// I think this is to look up in the language*.dll
    std::array<uint32_t, 16> playerNamesStringTable;

    /// If the player can win by conquest
    uint8_t conquestVictory = 0;

    /// The timeline of what has happened in the game
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

    /// Instructions shown to the player
    std::string instructions;

    /// Hints shown to the player
    std::string hints;

    /// What to show on victory
    std::string victory;

    /// What to show on loss
    std::string loss;

    /// History to show
    std::string history;

    /// GV >= TC
    std::string scouts;

    std::string oldFilename1;
    std::string oldFilename2;
    std::string oldFilename3;

    /// Video file to play before starting
    std::string pregameCinematicFilename;

    /// Video file to play when winning
    std::string victoryCinematicFilename;

    /// Video file to play when losing
    std::string lossCinematicFilename;

    /// Background graphics
    std::string backgroundFilename;

    // Bitmap

    /// Boolean
    uint32_t bitmapIncluded = 0;

    /// Width of included bitmap file
    uint32_t bitmapWidth = 0;

    /// Height of included bitmap file
    uint32_t bitmapHeigth = 0;

    /// -1 if there's a bitmap, 1 otherwise
    int16_t hasBitmap = 1;

    /// Size of embedded bitmap file
    uint16_t bitmapByteSize = 0;

    /// Standard BMP header
    char *bmpHeader = nullptr;

    /// BMP data
    char *bitmap = nullptr;

    /// Names of AIs
    std::array<std::string, 16> aiNames;

    /// Names of cities
    std::array<std::string, 16> cityNames;

    /// Names of AI personalities
    std::array<std::string, 16> personalityNames;

    /// AI script files
    std::array<AiFile, 16> aiFiles;

    /// Types of AIs (difficulty, I think)
    std::array<uint8_t, 16> aiTypes;

    std::array<CombinedResources, 16> resourcesPlusPlayerInfo;

    /// Old/less specific victory condition, ScnMorePlayerData::victoryConditions
    /// lets you define more specific ones.
    ScnVictory victoryConditions;

    /// Initial diplomacy
    ScnDiplomacy diplomacy;

    /// Which players have allied victory
    std::vector<uint32_t> alliedVictory;

    /// List of units, techs, etc. disabled
    ScnDisables disables;

    uint32_t unused1 = 0;
    uint32_t unused2 = 0;

    /// IDK
    uint32_t allTechs = 0;

    /// Which age all players start in (I think this might be overriden by the resources)
    std::array<int32_t, 16> startingAge;

    /// The human player camera position, x axis
    int32_t player1CameraX = 0;
    /// The human player camera position, y axis
    int32_t player1CameraY = 0;

    /// Default AI type, I think?
    int32_t aiType = 0;

    /// For debugging
    bool verbose = false;

private:
    uint32_t separator_ = 0;
    void serializePlayerDataVersion(void);
    void serializeObject(void) override;
    void serializeBitmap(void);
};

/// Defines a condition necessary for winning
class ScnPlayerVictoryCondition : public ISerializable
{
    // Need to find a scenario file with the matching struct filled in to use these enums
    enum PointState {
        /// Not bothering with points
        PointsInactive,

        /// Points/score needs to be above
        PointsAbove,

        /// Needs to have less points than the target value
        PointsBelow
    };

    enum VictoryPointCondition {
        /// Just needs a certain amount
        AttributeAmount,

        /// Needs to have the attribute first
        AttributeFirst,

        /// Needs to have the highest attribute of all players
        HighestAttribute,

        /// Needs to have had high attribute once
        HighAttributeOnce,

        /// In general needs to have a high attributes
        HighAttributeAmount
    };

public:
    enum Type : uint8_t {
        /// Invalid/unset
        Invalid = 255,

        /// Needs to capture something
        Capture = 0,

        /// Needs to create something
        Create = 1,

        /// Need to destroy a single thing
        Destroy = 2,

        /// Need to destroy at least `count` things (see below)
        DestroyMultiple = 3,

        /// Needs to bring something to a specific area, defined by x0, y0, x1 and y1
        BringToArea = 4,

        /// Needs to bring an object (not sure to where)
        BringObject = 5,

        /// Needs an attribute at a certain value
        Attribute = 6,

        /// Needs to explore a specific area
        Explore = 7,

        /// Needs to create something in a specific area
        CreateInArea = 8,

        /// Needs to destroy all of a certain type
        DestroyAll = 9,

        /// Needs to kill a player
        DestroyPlayer = 10,

        /// Needs to have a above or below a certain amount of points
        VictoryPoints = 12
    };

    Type type = Invalid;

    /// for create, create in area, destroy, destroy multiple,  destroy all
    int32_t objectType = 0;

    /// For killing a certain number of units of a certain player, for example, or killing a specific player
    int32_t targetPlayer = 0;

    /// Left border of rect it needs to be in
    float x0 = 0.f;

    /// Top border of rect it needs to be in
    float y0 = 0.f;

    /// Right border of rect it needs to be in
    float x1 = 0.f;

    /// Bottom border of rect it needs to be in
    float y1 = 0.f;

    /// For an Attribute condition, this is the attribute type, and count is the amount
    int32_t number = 0;

    /// Number of things to kill/create/capture/etc.
    int32_t count = 0;

    /// Object necessary to kill/create/capture/etc. win
    int32_t object = 0;

    /// Specific target to capture/whatever
    int32_t target = 0;

    /// If this makes a certain group of players win
    uint8_t victoryGroup = 0;

    /// If allies also win
    uint8_t allyFlag = 0;

    /// Current state (enabled or disabled)
    uint8_t state = 0;

private:
    void serializeObject(void) override;
};

class ScnMorePlayerData : public ISerializable
{
public:
    /// Name of a player
    std::string playerName;

    /// Initial camera position for this player, x axis
    float initCameraX = 0.f;

    /// Initial camera position for this player, y axis
    float initCameraY = 0.f;

    /// I have no idea why this is twice
    int16_t initCameraX2 = 0;

    /// I have no idea why this is twice
    int16_t initCameraY2 = 0;

    /// Will all allies win when one wins?
    uint8_t alliedVictory = 0;

    /// Initial diplomacy state with other players
    std::vector<uint8_t> diplomacy1;

    /// Initial diplomacy state with other players, dunno why this is twice
    std::vector<uint32_t> diplomacy2;

    /// The color this player has
    uint32_t playerColor = 0;

    /// File version of the victory conditions
    float victoryConditionVersion = 0.f;

    /// List of conditions necessary for winning
    std::vector<ScnPlayerVictoryCondition> victoryConditions; // found in Grand Theft Empires, and the aztec campaign

    /// Player ID
    int32_t playerID = -1;

    /// If this player has won, I think?
    uint8_t victory = 0;

    int32_t unknown1 = 0;
    int32_t unknown2 = 0;

    /// Number of "victory points" (see the victory conditions"
    uint32_t totalVictoryPoints = 0;

private:
    uint16_t playerCount_;
    uint32_t victoryConditionsCount;
    uint32_t pointConditionsCount;
    void serializeObject(void) override;
};
} // namespace genie

#endif // GENIE_SCN_PLAYER_DATA_H
