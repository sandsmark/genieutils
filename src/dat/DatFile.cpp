/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2020  Mikko "Tapsa" P

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

#include "genie/dat/DatFile.h"

#include "TestHelpers.h"

#include <fstream>
#include <vector>
#include <unordered_map>

#include "genie/Types.h"

namespace genie {

float ISerializable::dat_internal_ver = 0.f;
GameVersion GV_LatestTap = GV_T8;
GameVersion GV_LatestDE2 = GV_C16;

//------------------------------------------------------------------------------
DatFile::DatFile() :
    compressor_(this)
{
}

std::string DatFile::gameName()
{
    if (int(getGameVersion()) >= GV_None && int(getGameVersion()) <= GV_CC) {
        return versionName(getGameVersion());
    }

    return FileVersion;
}

GenieGame DatFile::game() const
{
    GameVersion version = getGameVersion();
    if (version <= GV_RoR) {
        return GenieGame::AgeOfEmpires1;
    }
    if (version <= GV_TC) {
        return GenieGame::AgeOfEmpires2;
    }
    if (version <= GV_LatestDE2) {
        return GenieGame::AgeOfEmpires2DefinitiveEdition;
    }
    if (version >= GV_SWGB) {
        return GenieGame::StarWarsGalacticBattlegrounds;
    }

    return GenieGame::Unknown;
}

std::string DatFile::versionName(const GameVersion version)
{
    if (int(version) < GV_None) {
        return "Invalid game version " + std::to_string(int(version));
    }
    if (int(version) > GV_CC) {
        return "Unknown game version " + std::to_string(int(version));
    }
    switch(version) {
        case genie::GV_None: return "None";
        case genie::GV_TEST: return "Test";
        case genie::GV_MIK: return "Mike";
        case genie::GV_DAVE: return "Dave";
        case genie::GV_MATT: return "Matt";
        case genie::GV_AoEB: return "Age of Empires Beta";
        case genie::GV_AoE: return "Age of Empires";
        case genie::GV_RoR: return "Age of Empires: Rise of Rome";
        case genie::GV_Tapsa: return "Age of Empires 2: Tapsa Test";
        case genie::GV_T2: return "Age of Empires 2: Tapsa Test 2";
        case genie::GV_T3: return "Age of Empires 2: Tapsa Test 3";
        case genie::GV_T4: return "Age of Empires 2: Tapsa Test 4";
        case genie::GV_T5: return "Age of Empires 2: Tapsa Test 5";
        case genie::GV_T6: return "Age of Empires 2: Tapsa Test 6";
        case genie::GV_T7: return "Age of Empires 2: Tapsa Test 7";
        case genie::GV_T8: return "Age of Empires 2: Tapsa Test 8";
        case genie::GV_AoKE3: return "Age of Empires 2: Age of Kings E3 Preview";
        case genie::GV_AoKA: return "Age of Empires 2: Age of Kings Alpha";
        case genie::GV_AoKB: return "Age of Empires 2: Age of Kings Beta";
        case genie::GV_AoK: return "Age of Empires 2: Age of Kings";
        case genie::GV_TC: return "Age of Empires 2: The Conquerors";
        case genie::GV_UP15: return "Age of Empires 2: The Conquerors with User Patch 1.5";
        case genie::GV_Cysion: return "Age of Empires 2: Cysion";
        case genie::GV_C2: return "Age of Empires 2: Definitive Edition";
        case genie::GV_C3: return "Age of Empires 2: Definitive Edition C3";
        case genie::GV_C4: return "Age of Empires 2: Definitive Edition C4";
        case genie::GV_CK: return "Age of Empires 2: Definitive Edition Ck";
        case genie::GV_C5: return "Age of Empires 2: Definitive Edition C5";
        case genie::GV_C6: return "Age of Empires 2: Definitive Edition C6";
        case genie::GV_C7: return "Age of Empires 3: Definitive Edition C7";
        case genie::GV_C8: return "Age of Empires 3: Definitive Edition C8";
        case genie::GV_C9: return "Age of Empires 3: Definitive Edition C9";
        case genie::GV_C10: return "Age of Empires 3: Definitive Edition C10";
        case genie::GV_C11: return "Age of Empires 3: Definitive Edition C11";
        case genie::GV_C12: return "Age of Empires 3: Definitive Edition C12";
        case genie::GV_C13: return "Age of Empires 3: Definitive Edition C13";
        case genie::GV_C14: return "Age of Empires 3: Definitive Edition C14";
        case genie::GV_C15: return "Age of Empires 3: Definitive Edition C15";
        case genie::GV_C16: return "Age of Empires 3: Definitive Edition C16";
        case genie::GV_SWGB: return "Star Wars: Galactic Battlegrounds";
        case genie::GV_CC: return "Star Wars: Galactic Battlegrounds: Clone Campaigns";
        // No default, so we get compiler warnings if new are added
    }
    throw std::runtime_error("Unhandled state in DatFile::versionName");
    return "Invalid state";
}

GameVersion DatFile::gameVersionFromString(const std::string &name)
{
    if (name.size() != FILE_VERSION_SIZE - 1) { // null terminated in file, std::string does not count that
        std::cerr << "Invalid length of version, " << name.size() << " vs expected " << (FILE_VERSION_SIZE - 1) << std::endl;
        return GV_None;
    }
    int major = name[4] - '0';
    int minor = name[6] - '0';
    if (major < 0 || major > 9) {
        std::cerr << "Invalid major version '" << major << "' (" << char(name[4]) << ") in name " << name << std::endl;
        return GV_None;
    }
    if (minor < 0 || minor > 9) {
        std::cerr << "Invalid minor version in name " << name << std::endl;
        return GV_None;
    }
    switch (major) {
    case 7:
        switch(minor) {
        case 0:
            return GV_C13;
        case 1:
            return GV_C14;
        case 2:
            return GV_C15;
        case 3:
            return GV_C16;
        default:
            break;
        }
        break;
    case 6:
        switch(minor) {
        case 0:
            return GV_C4;
        case 1:
            return GV_CK;
        case 2:
            return GV_C5;
        case 3:
            return GV_C6;
        case 4:
            return GV_C7;
        case 5:
            return GV_C8;
        case 6:
            return GV_C9;
        case 7:
            return GV_C10;
        case 8:
            return GV_C11;
        case 9:
            return GV_C12;
        default:
            break;
        }
        break;
    case 5:
        switch(minor) {
        case 7:
            return GV_TC;
        case 8:
            return GV_C2;
        case 9:
            return GV_C3;
        default:
            break;
        }
        break;
    case 4:
        switch(minor) {
        case 0:
            return GV_T3;
        case 1:
            return GV_T4;
        case 2:
            return GV_T5;
        case 3:
            return GV_T6;
        case 4:
            return GV_T7;
        case 5:
            return GV_T8;
        default:
            break;
        }
        break;
    case 3:
        switch(minor) {
        case 9:
            return GV_T2;
        default:
            break;
        }
        break;
    default:
        break;
    }

    std::cout << "Unknown game version " << name << std::endl;

    return GV_None;
}

//------------------------------------------------------------------------------
void DatFile::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);

    updateGameVersion(TerrainRestrictions);
    updateGameVersion(PlayerColours);
    updateGameVersion(Sounds);
    updateGameVersion(Graphics);
    updateGameVersion(Effects);
    updateGameVersion(UnitLines);
    updateGameVersion(UnitHeaders);
    updateGameVersion(Civs);
    updateGameVersion(Techs);
    TerrainBlock.setGameVersion(gv);
    RandomMaps.setGameVersion(gv);
    TechTree.setGameVersion(gv);
}

//------------------------------------------------------------------------------
void DatFile::extractRaw(const std::string &inFile, const std::string &outFile)
{
    std::ifstream ifs;
    std::ofstream ofs;

    ifs.open(inFile, std::ios::binary);
    ofs.open(outFile, std::ios::binary);

    if (!ifs.is_open()) {
        throw std::ios_base::failure("Can't open input file '" + inFile + "'");
    }

    if (!ofs.is_open()) {
        throw std::ios_base::failure("Can't open output file '" + outFile + "'");
    }

    Compressor::decompress(ifs, ofs);

    ifs.close();
    ofs.close();
}

void DatFile::saveRaw(const std::string &outFile) try
{
    std::ofstream file;

    file.open(outFile, std::ofstream::binary);

    if (file.fail()) {
        file.close();
        throw std::ios_base::failure("Can't write to file: '" + outFile + "'");
    } else {
        m_rawMode = true;
        writeObject(file);
    }

    file.close();
} catch (const std::exception &e) {
    // make sure we always set it back
    m_rawMode = false;
    throw e;
}

//------------------------------------------------------------------------------
void DatFile::setVerboseMode(bool verbose)
{
    verbose_ = verbose;
}

bool DatFile::compareTo(const DatFile &other) const
{
    COMPARE_MEMBER(FileVersion);
    COMPARE_MEMBER_VEC(FloatPtrTerrainTables);
    COMPARE_MEMBER_VEC(TerrainPassGraphicPointers);
    COMPARE_MEMBER_OBJ_VEC(TerrainRestrictions);
    COMPARE_MEMBER_OBJ_VEC(PlayerColours);
    COMPARE_MEMBER_OBJ_VEC(Sounds);
    COMPARE_MEMBER_VEC(GraphicPointers);

    COMPARE_MEMBER_OBJ_VEC(Graphics);
    COMPARE_MEMBER_OBJ_VEC(Effects);
    COMPARE_MEMBER_OBJ_VEC(UnitHeaders);
    COMPARE_MEMBER_OBJ_VEC(Civs);

    COMPARE_MEMBER_OBJ_VEC(Techs);
    COMPARE_MEMBER_OBJ_VEC(UnitLines);

    COMPARE_MEMBER(TimeSlice);
    COMPARE_MEMBER(UnitKillRate);
    COMPARE_MEMBER(UnitKillTotal);
    COMPARE_MEMBER(UnitHitPointRate);
    COMPARE_MEMBER(UnitHitPointTotal);
    COMPARE_MEMBER(RazingKillRate);
    COMPARE_MEMBER(RazingKillTotal);
    COMPARE_MEMBER(TerrainsUsed1);
    COMPARE_MEMBER(SUnknown2);
    COMPARE_MEMBER(SUnknown3);
    COMPARE_MEMBER(swgbBlendModes);
    COMPARE_MEMBER(swgbMaxBlendmodes);

    COMPARE_MEMBER(SUnknown7);
    COMPARE_MEMBER(SUnknown8);

    return RandomMaps.compareTo(other.RandomMaps) &&
            TerrainBlock.compareTo(other.TerrainBlock) &&
            TechTree.compareTo(other.TechTree);
}

std::string DatFile::resourceFilename(const uint32_t id)
{
    if (m_resourceFilenames.empty()) {
        loadResourceFilenames();
    }

    if (m_resourceFilenames.contains(id)) {
        return m_resourceFilenames[id];
    }

    if (!m_resourceFilenames.contains(id)) {
        return std::to_string(id);
    }

    return m_resourceFilenames[id];
}

const std::unordered_map<uint32_t, std::string> &DatFile::resourceFilenames()
{
    if (m_resourceFilenames.empty()) {
        loadResourceFilenames();
    }

    return m_resourceFilenames;
}

void DatFile::loadResourceFilenames()
{
    // TODO could check for conflicts, but that's slower
    for (const Sound &sound: Sounds) {
        for (const SoundItem &item : sound.Items) {
            m_resourceFilenames[item.ResourceID]  = item.FileName;
        }
    }

    for (const Terrain &terrain: TerrainBlock.Terrains) {
        if (!terrain.Name2.empty()) { // name2 is supposed to be slp name
            m_resourceFilenames[terrain.SLP] = terrain.Name2;
        } else {
            m_resourceFilenames[terrain.SLP] = terrain.Name; // long name, just to have a fallback
        }
    }

    for (const Graphic &graphic : Graphics) {
        if (graphic.SLP == -1) { // Graphics with just deltas
            continue;
        }
        if (graphic.SLP == 0) { // Free graphic slots
            continue;
        }
        if (graphic.FileName.empty()) {
            std::cout << "Missing name " << graphic.SLP << ' ' << graphic.ID << ' ' << graphic.Name << std::endl;
            continue;
        }
        m_resourceFilenames[graphic.SLP] = graphic.FileName;
    }
}

//------------------------------------------------------------------------------
void DatFile::serializeObject()
{
    if (!m_rawMode) {
        compressor_.beginCompression();
    }

    serialize(FileVersion, FILE_VERSION_SIZE);

    if (verbose_) {
        std::cout << "file version: " << FileVersion << std::endl;
    }

    // Handle all different versions while in development.
    if (getGameVersion() == GV_C2) { // 5.8
        GameVersion guessedVersion = gameVersionFromString(FileVersion);
        if (guessedVersion != GV_None) {
            setGameVersion(guessedVersion);
        }
    } else if (getGameVersion() == GV_Tapsa) {
        GameVersion guessedVersion = gameVersionFromString(FileVersion);
        std::cout << "Guessed game version " << versionName(guessedVersion) << " from " << FileVersion << std::endl;
        if (guessedVersion != GV_None) {
            setGameVersion(guessedVersion);
        }
    }

    GameVersion gv = getGameVersion();
    if (gv == GV_None) {
        throw std::runtime_error("No game version defined!");
    }

    uint16_t count16{};
    uint32_t count32{};

    if (gv >= GV_SWGB) {
        serializeSize<uint16_t>(count16, Civs.size());
        serialize<int32_t>(SUnknown2);
        serialize<int32_t>(SUnknown3);
        serialize<int32_t>(swgbBlendModes);
        serialize<int32_t>(swgbMaxBlendmodes);

        if (verbose_) {
            std::cout << "Unkown2: " << SUnknown2 << std::endl;
            std::cout << "Unkown3: " << SUnknown3 << std::endl;
            std::cout << "swgb blend modes: " << swgbBlendModes << std::endl;
            std::cout << "swgb max blend modes: " << swgbMaxBlendmodes << std::endl;
        }
    }

    serializeSize<uint16_t>(count16, TerrainRestrictions.size());
    serialize<uint16_t>(TerrainsUsed1);

    if (verbose_) {
        std::cout << "TerrainRestriction size: " << TerrainRestrictions.size() << " " << count16 << std::endl;

        std::cout << FileVersion;
        std::cout << std::endl
                  << "TerRestrictionCount: " << count16 << std::endl;
        std::cout << "TerCount: " << TerrainsUsed1 << std::endl;
    }

    serialize<int32_t>(FloatPtrTerrainTables, count16);
    if (verbose_) {
        std::cout << "FloatPtrTerrainTables: " << FloatPtrTerrainTables.size() << std::endl;
    }

    if (gv >= GV_AoKA) {
        serialize<int32_t>(TerrainPassGraphicPointers, count16);
    }

    TerrainRestriction::setTerrainCount(TerrainsUsed1);

    serialize(TerrainRestrictions, count16);

    if (verbose_) {
        std::cout << "TerrainRestrictions: " << count16 << std::endl;
    }

    serializeSize<uint16_t>(count16, PlayerColours.size());

    if (verbose_) {
        std::cout << "PlayerColours: " << count16 << std::endl;
    }

    serialize(PlayerColours, count16);

    serializeSize<uint16_t>(count16, Sounds.size());

    if (verbose_) {
        std::cout << "Sounds: " << count16 << std::endl;
    }

    serialize(Sounds, count16);

    serializeSize<uint16_t>(count16, Graphics.size());

    if (gv < GV_AoE) {
        serialize(Graphics, count16);
    } else {
        serialize<int32_t>(GraphicPointers, count16);
        serializeSubWithPointers<Graphic>(Graphics, count16, GraphicPointers);
    }

//    std::pos pos_cnt = tellg();
    if (verbose_) {
        std::cout << "Graphics: " << Graphics.size() << std::endl;
    }

    serialize<ISerializable>(TerrainBlock);

    if (verbose_) {
        std::cout << "Tile sizes: " << TerrainBlock.TileSizes.size() << std::endl;
        std::cout << "Terrains: " << TerrainBlock.Terrains.size() << std::endl;
        std::cout << "Borders: " << TerrainBlock.TerrainBorders.size() << std::endl;
        std::cout << "Some bytes: " << TerrainBlock.SomeBytes.size() << std::endl;
        std::cout << "Some ints: " << TerrainBlock.SomeInt32.size() << std::endl;
//        pos_cnt = tellg() - pos_cnt;
//        std::cout << "Terrain block "
//                  << "(0x" << std::hex << pos_cnt;
//        std::cout << " to 0x" << tellg() << std::dec << ") size " << pos_cnt << std::endl;
    }

    // This data seems to be needed only in AoE and RoR.
    // In later games it is removable.
    // It exists in Star Wars games too, but is not used.
    serialize<ISerializable>(RandomMaps);

    if (verbose_) {
        std::cout << "Random maps: " << RandomMaps.Maps.size() << std::endl;
    }

    serializeSize<uint32_t>(count32, Effects.size());

    if (verbose_) {
        std::cout << "Effects: " << count32 << std::endl;
    }

    serialize(Effects, count32);

    if (gv >= GV_SWGB) { //pos: 0x111936
        serializeSize<uint16_t>(count16, UnitLines.size());
        serialize(UnitLines, count16);
    }

    if (gv >= GV_AoK) {
        serializeSize<uint32_t>(count32, UnitHeaders.size());

        if (verbose_) {
            std::cout << "Units: " << count32 << std::endl;
        }

        serialize(UnitHeaders, count32);
    }

    serializeSize<uint16_t>(count16, Civs.size());

    if (verbose_) {
        std::cout << "Civs: " << count16 << std::endl;
    }

    serialize(Civs, count16);

    if (gv >= GV_SWGB) {
        serialize<int8_t>(SUnknown7);
    }

    serializeSize<uint16_t>(count16, Techs.size());

    if (verbose_) {
        std::cout << "Techs: " << count16 << std::endl;
    }

    serialize(Techs, count16);

//    if (verbose_) {
//        pos_cnt = tellg();
//        std::cout << "TechTrees (before eof) (0x" << std::hex << pos_cnt;
//    }

    if (gv >= GV_SWGB) {
        serialize<int8_t>(SUnknown8);
    }

    if (gv >= GV_AoKA) { // 9.38
        serialize<int32_t>(TimeSlice);
        serialize<int32_t>(UnitKillRate);
        serialize<int32_t>(UnitKillTotal);
        serialize<int32_t>(UnitHitPointRate);
        serialize<int32_t>(UnitHitPointTotal);
        serialize<int32_t>(RazingKillRate);
        serialize<int32_t>(RazingKillTotal);

        serialize<ISerializable>(TechTree);
    }

//    if (verbose_) {
//        pos_cnt = tellg() - pos_cnt;
//        std::cout << " to 0x" << tellg() << std::dec << ") size " << pos_cnt << std::endl;
//    }

    if (!m_rawMode) {
        compressor_.endCompression();
    }
}

//------------------------------------------------------------------------------
void DatFile::unload()
{
    FloatPtrTerrainTables.clear();
    TerrainPassGraphicPointers.clear();
    TerrainRestrictions.clear();
    PlayerColours.clear();
    Sounds.clear();
    GraphicPointers.clear();
    Graphics.clear();
    TerrainBlock.Terrains.clear();
    TerrainBlock.TerrainBorders.fill(TerrainBorder());
    TerrainBlock.AoEAlphaUnknown.clear();
    TerrainBlock.SomeBytes.clear();
    TerrainBlock.SomeInt32.clear();
    RandomMaps.Maps.clear();
    Effects.clear();
    UnitLines.clear();
    UnitHeaders.clear();
    Civs.clear();
    Techs.clear();
    TechTree.TechTreeAges.clear();
    TechTree.BuildingConnections.clear();
    TechTree.UnitConnections.clear();
    TechTree.ResearchConnections.clear();
}
} // namespace genie
