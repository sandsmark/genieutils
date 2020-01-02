#include "genie/savegame/MgxFile.h"
namespace genie {

//std::pair<uint32_t, uint32_t> MgxUnknown::mapSize;

MgxFileHeader::MgxFileHeader() :
    m_compressor(this)
{
}

void MgxFileHeader::serializeObject()
{
    serialize<uint32_t>(m_headerSize);
    serialize<uint32_t>(m_nextHeader);
    m_compressor.beginCompression();

    serialize(m_version, 8);
    serialize<float>(m_savegameVersion);

    //////////////////// save game format, because I'm easily confused, it looks similar to mgx though
    serialize<uint32_t>(m_worldTime);
    serialize<uint32_t>(m_oldWorldTime);
    serialize<float>(m_worldTimeDelta);
    serialize<float>(m_worldTimeDeltaSeconds);
    serialize<float>(m_timer);
    serialize<float>(m_gameSpeed);
    serialize<uint8_t>(m_tempPaused);
    serialize<uint32_t>(m_nextObjectID);
    serialize<uint32_t>(m_nextReusableObjectID);
    serialize<uint32_t>(m_randomSeed);
    serialize<uint16_t>(m_currPlayer);

    serializeSize<uint8_t>(m_numPlayers, m_players.size());

    serialize<uint8_t>(m_gameState);
    serialize<uint32_t>(m_campaignIndex);
    serialize<uint32_t>(m_campaignPlayer);
    serialize<uint32_t>(m_campaignScenario);
    serialize<uint32_t>(m_playerTurn);

    serialize<uint32_t, 9>(m_playerTimeDelta);

    //////// map
    serializePair<uint32_t>(m_map.size);
    serialize<uint8_t>(m_map.visibleFlag);
    serialize<uint8_t>(m_map.fogFlag);
    if (m_savegameVersion < 7) {
        throw std::runtime_error("bit unpacking not implemented");
    } else {
        serialize<MgxMapTile>(m_map.tiles, m_map.size.first * m_map.size.second);
    }
    serialize<uint8_t, 255>(m_map.m_zoneMap.m_zoneInfo);
    serialize<uint32_t, 255>(m_map.m_zoneMap.m_numberTilesInZoneValue);
    serialize<uint8_t>(m_map.m_zoneMap.m_zoneMap, m_map.size.first, m_map.size.second);
    serializeSize<uint32_t>(m_map.m_zoneMap.passabilityRuleCount, m_map.m_zoneMap.terrain_passability_rules.size());
    serialize<float>(m_map.m_zoneMap.terrain_passability_rules, m_map.m_zoneMap.passabilityRuleCount);

    // player
    serialize<MgxPlayerInfo>(m_players, m_numPlayers);

    // todo: this is basically copied from the scenario file stuff, move victory condition list into separate reusable class
    for (MgxPlayerInfo &player : m_players) {
        serialize<float>(player.victoryConditionVersion);
        serializeSize<uint32_t>(player.victoryConditionsCount, player.victoryConditions.size());
        serialize<uint8_t>(player.victory);
        serialize<ScnPlayerVictoryCondition>(player.victoryConditions, player.victoryConditionsCount);
    }

    serialize<ScnMainPlayerData>(m_scenarioData);

    if (m_savegameVersion >= 7.16) {
        serialize<int32_t>(m_difficultyLevel);
    } else {
        m_difficultyLevel = -1;
    }

    if (m_savegameVersion >= 5.6) {
        serialize<int32_t>(m_victoryType);
    }

    if (m_savegameVersion >= 5.3) {
        serialize<int32_t>(m_artifactCount);
        serialize<int32_t>(m_ruinCount);
    }

    if (m_savegameVersion >= 5.4) {
        serialize<uint8_t>(m_countdownVictory);
        serialize<float>(m_countdownClock);
    }

    if (m_savegameVersion >= 6.6) {
        serialize<uint8_t>(m_scoreDisplayed);
    }

    if (m_savegameVersion >= 7) {
        serialize<int8_t>(m_controllingComputerPlayer);
    }

    m_compressor.endCompression();
}

MgxFile::MgxFile()
{
}

void MgxFile::serializeObject()
{
    if (isOperation(OP_READ)) {
        bool hasNext = true;
        while (hasNext) {
            std::shared_ptr<MgxFileHeader> header = std::make_shared<MgxFileHeader>();
            serialize<MgxFileHeader>(*header);
            hasNext = header->hasNext();
            m_headers.push_back(std::move(header));
        }
    } else {
        serialize<MgxFileHeader>(m_headers, m_headers.size());
    }
}

void MgxPlayerInfo::serializeObject()
{
    serialize<uint8_t>(m_type);
    if (m_type != 0) {
        std::cerr << "invalid player! " << m_type << std::endl;
    }

    serializeSize<uint16_t>(unitsCount, m_units.size());
    serialize<int32_t>(m_unitPointers, unitsCount);
    serializeSubWithPointers<Unit>(m_units, unitsCount, m_unitPointers);
}

}//namespace genie
