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
    std::cout << "begin compression" << std::endl;
    m_compressor.beginCompression();
    std::cout << "began compression" << std::endl;
#if 0
    std::ofstream dump("/tmp/decompressed");

    while (getIStream()->good()) {
        dump.put(getIStream()->get());
    }

    dump.close();
    exit(0);
#endif

    serialize(m_version, 8);
    std::cout << "version: " << m_version << std::endl;
    serialize<float>(m_savegameVersion);
    std::cout << "savegame version: " << m_savegameVersion << std::endl;
    serialize<uint32_t>(m_includeAi);
    std::cout << "includes ai? " << m_includeAi << std::endl;

    serialize<uint32_t>(m_unknown1);
    std::cout << "unknown1 " << m_unknown1 << std::endl;
    serialize<uint32_t>(m_gameSpeed1);
    std::cout << "gamespeed1 " << m_gameSpeed1 << std::endl;
    serialize<uint32_t>(m_unknownZero1);
    std::cout << "unknownzero " << m_unknownZero1 << std::endl;
    serialize<uint32_t>(m_gameSpeed2);
    std::cout << "gamespeed2 " << m_gameSpeed2 << std::endl;
    serialize(m_unknown29chars);
    serialize<uint16_t>(m_recOwner);
    std::cout << "rec owner " << m_recOwner << std::endl;
    serialize<uint8_t>(m_numPlayers);
    std::cout << int(m_numPlayers) << " players" << std::endl;

    serialize(m_unknown62chars);

    //serializePair<uint32_t>(m_map.size);
    //std::cout << m_map.size.first << "x" << m_map.size.second << std::endl;
    //serialize<uint32_t>(m_unknown2);
    //std::cout << "unknown2 " << m_unknown2 << std::endl;

#if 0
    //////////////////// save game format, because I'm easily confused, it looks similar to mgx though
    serialize<uint32_t>(m_worldTime);
    std::cout << "world time: " << m_worldTime << std::endl;
    serialize<uint32_t>(m_oldWorldTime);
    std::cout << "old world time: " << m_oldWorldTime << std::endl;
    serialize<float>(m_worldTimeDelta);
    std::cout << "world time delta : " << m_worldTimeDelta << std::endl;
    serialize<float>(m_worldTimeDeltaSeconds);
    std::cout << "world time delta seconds: " << m_worldTimeDeltaSeconds << std::endl;
    serialize<float>(m_timer);
    std::cout << "timer: " << m_timer << std::endl;
    serialize<float>(m_gameSpeed);
    std::cout << "game speed: " << m_gameSpeed << std::endl;
    serialize<uint8_t>(m_tempPaused);
    serialize<uint32_t>(m_nextObjectID);
    serialize<uint32_t>(m_nextReusableObjectID);
    serialize<uint32_t>(m_randomSeed);
    serialize<uint16_t>(m_currPlayer);

    serializeSize<uint8_t>(m_numPlayers, m_players.size());
    std::cout << int(m_numPlayers) << " players" << std::endl;

    serialize<uint8_t>(m_gameState);
    serialize<uint32_t>(m_campaignIndex);
    serialize<uint32_t>(m_campaignPlayer);
    serialize<uint32_t>(m_campaignScenario);
    serialize<uint32_t>(m_playerTurn);

    serialize<uint32_t, 9>(m_playerTimeDelta);
#endif

    //////// map
    serializePair<uint32_t>(m_map.size);
    std::cout << m_map.size.first << "x" << m_map.size.second << std::endl;
    //serialize<uint8_t>(m_map.visibleFlag);
    //std::cout << "visibile flag: " << int(m_map.visibleFlag);
    //serialize<uint8_t>(m_map.fogFlag);
    //std::cout << "fog flag " << int(m_map.fogFlag) << std::endl;
    //if (m_savegameVersion < 7) {
    //    throw std::runtime_error("bit unpacking not implemented");
    //}
    std::cout << "getting map" << std::endl;
    std::cout << m_map.size.first << "x" << m_map.size.second << std::endl;
    serialize<MgxMapTile>(m_map.tiles, m_map.size.first * m_map.size.second);
    std::cout << "getting zones" << std::endl;
    serialize<uint8_t, 255>(m_map.m_zoneMap.m_zoneInfo);
    std::cout << "getting tiles in zones" << std::endl;
    serialize<uint32_t, 255>(m_map.m_zoneMap.m_numberTilesInZoneValue);
    std::cout << "getting zone map" << std::endl;
    serialize<uint8_t>(m_map.m_zoneMap.m_zoneMap, m_map.size.first, m_map.size.second);
    std::cout << "getting passability" << std::endl;
    serializeSize<uint32_t>(m_map.m_zoneMap.passabilityRuleCount, m_map.m_zoneMap.terrain_passability_rules.size());
    std::cout << "getting terrain passability " << m_map.m_zoneMap.passabilityRuleCount << std::endl;
    serialize<float>(m_map.m_zoneMap.terrain_passability_rules, m_map.m_zoneMap.passabilityRuleCount);

    std::cout << "getting players" << std::endl;
    // player
    serialize<MgxPlayerInfo>(m_players, m_numPlayers);
    std::cout << "got players" << std::endl;

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
