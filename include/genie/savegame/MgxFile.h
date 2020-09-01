#pragma once

#include "genie/file/IFile.h"
#include "genie/file/Compressor.h"
#include "genie/dat/Unit.h"
#include "genie/script/scn/ScnPlayerData.h"

namespace genie {

class MgxAi : public ISerializable
{

};

struct MgxZoneMap
{
    std::array<uint8_t, 255> m_zoneInfo;
    std::array<uint32_t, 255> m_numberTilesInZoneValue;
    std::vector<std::vector<uint8_t>> m_zoneMap;

    uint32_t passabilityRuleCount;
    std::vector<float> terrain_passability_rules;
};

// If save game version < 7 this is packed (wrt. bits), otherwise it is unpacked
struct MgxMapTile
{
    uint16_t x, y; // 32
    uint8_t terrainID; // 40
    uint8_t elevation; // 48

    uint8_t terrainType;// : 5;
    uint8_t height;// : 3;
    uint8_t borderType;// : 4;
    uint8_t borderShape;// : 4;
};

struct MgxMap
{
    std::pair<uint32_t, uint32_t> size;
    uint8_t visibleFlag;
    uint8_t fogFlag;
    std::vector<MgxMapTile> tiles;
    MgxZoneMap m_zoneMap;
};

class MgxPlayerInfo : public ISerializable
{
public:
    float victoryConditionVersion = 0.f;
    uint8_t victory;
    std::vector<ScnPlayerVictoryCondition> victoryConditions;
    uint32_t victoryConditionsCount;

protected:
    void serializeObject() override;

private:
    uint8_t m_type;
    uint16_t unitsCount;
    std::vector<int32_t> m_unitPointers;
    std::vector<Unit> m_units;

};

class MgxAchievement : public ISerializable
{
protected:
    void serializeObject() override;
};

class MgxScenarioHeader : public ISerializable
{
protected:
    void serializeObject() override;
};

class MgxInstruction : public ISerializable
{
protected:
    void serializeObject() override;
};

class MgxFileHeader : public ISerializable
{
public:
    MgxFileHeader();

    bool hasNext() const { return m_nextHeader != 0; }

protected:
    void serializeObject() override;

private:
    std::string m_version;
    float m_savegameVersion;

    bool m_includeAi;

    std::vector<MgxAi> m_ais;

    uint32_t m_unknown1;
    uint32_t m_gameSpeed1;
    uint32_t m_unknownZero1;
    uint32_t m_gameSpeed2;
    uint16_t m_recOwner;
    std::array<uint8_t, 29> m_unknown29chars;
    std::array<uint8_t, 62> m_unknown62chars;

    float m_unknownFloat1;
    uint32_t m_unknown2;

//    std::array<uint8_t, 17> m_unknown17chars;

    ////////////////
    uint32_t m_worldTime;
    uint32_t m_oldWorldTime;
    float m_worldTimeDelta;
    float m_worldTimeDeltaSeconds;
    float m_timer;
    float m_gameSpeed;
    uint8_t m_tempPaused; // 13
    uint32_t m_nextObjectID; // 12
    uint32_t m_nextReusableObjectID; // 8
    uint32_t m_randomSeed; // 4
    uint16_t m_currPlayer;
    uint8_t m_numPlayers;
    uint8_t m_gameState;
    uint32_t m_campaignIndex;
    uint32_t m_campaignPlayer;
    uint32_t m_campaignScenario;
    uint32_t m_playerTurn;
    std::array<uint32_t, 9> m_playerTimeDelta;

    MgxMap m_map;

    std::vector<MgxPlayerInfo> m_players;

    ScnMainPlayerData m_scenarioData;

    int32_t m_difficultyLevel;
    /////////////

    int32_t m_victoryType;
    int32_t m_artifactCount;
    int32_t m_ruinCount;
    uint8_t m_countdownVictory;
    float m_countdownClock;
    uint8_t m_scoreDisplayed;
    int8_t m_controllingComputerPlayer;

    Compressor m_compressor;
    uint32_t m_headerSize;
    uint32_t m_nextHeader;
    std::string m_headerVersion;
};

class MgxFile : public IFile
{
public:
    MgxFile();

protected:
    void serializeObject() override;

private:
    std::vector<std::shared_ptr<MgxFileHeader>> m_headers;
};


} // namespace genie
