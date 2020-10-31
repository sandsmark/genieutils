/*
    genieutils - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2015  Mikko "Tapsa" P
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

#include "genie/script/ScnFile.h"

#include "genie/util/Logger.h"
#include "genie/util/Utility.h"

#include <math.h>
#include <sstream>

namespace genie {

std::string ISerializable::scn_ver = "0.00";
float ISerializable::scn_plr_data_ver = 0.f;
float ISerializable::scn_internal_ver = 0.f;
double ISerializable::scn_trigger_ver = 0.0;

Logger &ScnFile::log = Logger::getLogger("genie.ScnFile");

static bool s_verbose = false;

//------------------------------------------------------------------------------
ScnFile::ScnFile() :
    IFile(), compressor_(this)
{
    scn_internal_ver = 0.f;
}

void ScnFile::extractRaw(std::istream &ifs, std::ostream &ofs)
{
    char version[4];
    ifs.read(version, 4);
    ofs.write(version, 4);

    uint32_t headerLen;

    ifs.read(reinterpret_cast<char *>(&headerLen), 4);
    ofs.write(reinterpret_cast<char *>(&headerLen), 4);


    std::vector<char> header(headerLen);

    ifs.read(header.data(), headerLen);
    ofs.write(header.data(), headerLen);

    Compressor::decompress(ifs, ofs);
}

//------------------------------------------------------------------------------
uint32_t ScnFile::getSeparator(void)
{
    return 0xFFFFFF9D;
}

bool ScnFile::verifyVersion()
{
    if (version.size() < 4) {
        log.error("Invalid version %s, too short:w", version);
        //TODO: Exception
        return false;
    }

    if (version[0] < '0' || version[0] > '9') {
        log.error("Invalid version %s", version);
        //TODO: Exception
        return false;
    }

    if (version[1] != '.') {
        log.error("Invalid version %s", version);
        //TODO: Exception
        return false;
    }

    if (version[2] < '0' || version[2] > '9') {
        log.error("Invalid version %s", version);
        //TODO: Exception
        return false;
    }

    if (version[3] < '0' || version[3] > '9') {
        log.error("Invalid version %s", version);
        //TODO: Exception
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
void ScnFile::serializeObject(void)
{
    s_verbose = verbose;

    playerData.verbose = s_verbose;
    ScnPlayerUnits::verbose = s_verbose;
    ScnPlayerResources::verbose = s_verbose;

    serializeVersion();

    if (isOperation(OP_READ) && !verifyVersion()) {
        std::cout << "ERROR" << std::endl;
        return;
    }

    if (isOperation(OP_WRITE)) {
        headerLength_ = 21 + scenarioInstructions.size();
    }

    serialize<uint32_t>(headerLength_); // Used in AoE 1 lobby

    if (s_verbose) std::cout << "Header length: " << headerLength_ << std::endl;

    serialize<int32_t>(saveType);
    if (s_verbose) std::cout << "Save type " << saveType << std::endl;

    // post aoe1 beta/alpha
    if (version[2] >= '1') {
        serialize<uint32_t>(lastSaveTime);
        if (s_verbose) std::cout << "Last save time " << lastSaveTime << std::endl;
    }

    serializeForcedString<uint32_t>(scenarioInstructions);
    if (s_verbose) std::cout << "Scenario instructions " << scenarioInstructions.size() << ": " << util::sanitizeAscii(scenarioInstructions) << std::endl;

    serialize<int32_t>(victoryType);
    if (s_verbose) std::cout << "Victory type " << victoryType << std::endl;

    serialize<uint32_t>(enabledPlayerCount);
    if (s_verbose) std::cout << "Enabled players " << enabledPlayerCount << std::endl;

    if (saveType == AoE2Scenario) {
        serialize<uint32_t>(aoe2ScenUnknown);
        if (s_verbose) std::cout << "AoE2 Scenario unknown: " << aoe2ScenUnknown << std::endl;
        serialize<uint32_t>(usesExpansions);
        if (s_verbose) std::cout << "AoE2 Scenario uses expansions: " << usesExpansions << std::endl;
        if (usesExpansions) {
            serializeSize<uint32_t>(expansionsCount, expansions.size());
            if (s_verbose) std::cout << "AoE2 Scenario expansions: " << expansionsCount << std::endl;
            serialize<uint32_t>(expansions, expansionsCount);
        }
    }


    compressor_.beginCompression();
    if (s_verbose) std::cout << "Started compression" << std::endl;

#if 0
    std::ofstream dump("/tmp/decompressed");

    while (getIStream()->good()) {
        dump.put(getIStream()->get());
    }

    dump.close();
    exit(0);
#endif


    // Compressed header:

    serialize<uint32_t>(nextUnitID);
    if (s_verbose) std::cout << "Next unit ID " << nextUnitID << std::endl;

    serialize<ISerializable>(playerData);
    if (s_verbose) std::cout << "Read player data " << std::endl;

    serialize<ScnMap>(map);
    if (s_verbose) std::cout << "Read map " << map.width << "x" << map.height << std::endl;

    if (scn_ver == "1.20") {
        scn_internal_ver = 1.14f;
    } else if (scn_ver == "1.21") {
        if (assumeSwgb) {
            scn_internal_ver = 1.15f; // SWGB reuses the same version numbers, but is incompatible, so we have this hack
        } else {
            scn_internal_ver = 1.14f;
        }
    } else if (scn_ver == "1.17" || scn_ver == "1.18") {
        scn_internal_ver = 1.13f;
    } else if(scn_ver == "1.19") {
        scn_internal_ver = 1.135f; // demo version, I think, it has some of the stuff from 1.14 (internal) at least
    } else if (scn_ver == "1.14" || scn_ver == "1.15" || scn_ver == "1.16") {
        scn_internal_ver = 1.12f;
    } else if (scn_ver == "1.22") {
        scn_internal_ver = 1.16f;
    } else if (scn_ver == "1.07") {
        scn_internal_ver = 1.07f;
    } else if (scn_ver == "1.09") {
        scn_internal_ver = 1.09f;
    } else {
        throw std::runtime_error("Unknown scenario file version " + scn_ver);
    }
    if (s_verbose) std::cout << "Internal version " << scn_internal_ver << std::endl;

    serializeSize<uint32_t>(playerUnitsCount, playerUnits.size());
    if (s_verbose) std::cout << "Player units count " << playerUnitsCount << std::endl;

    if (scn_internal_ver > 1.06f) {
        serialize<ScnPlayerResources, 8>(playerResources);
        if (s_verbose) std::cout << "Read player resources " << std::endl;
    } else {
        // A lot of data is read here.
    }

    serialize<ScnPlayerUnits>(playerUnits, playerUnitsCount);

    // You would think this would be the size of the player data, but no
    serialize<uint32_t>(playerCount2_);
    serialize<ScnMorePlayerData, 8>(players);
    if (s_verbose) std::cout << "All players loaded " << players.size() << std::endl;

    if (scn_internal_ver != 1.07f && scn_internal_ver != 1.09f) {
        triggerVersion = scn_trigger_ver;
        serialize<double>(triggerVersion);
        if (s_verbose) std::cout << "Trigger version " << triggerVersion << std::endl;
        scn_trigger_ver = triggerVersion;

        if (scn_trigger_ver > 1.4f) {
            serialize<int8_t>(objectivesStartingState);
        }

        serializeSize<uint32_t>(numTriggers_, triggers.size());
        if (s_verbose) std::cout << "num triggers " << numTriggers_ << std::endl;
        serialize<Trigger>(triggers, numTriggers_);

        if (scn_trigger_ver > 1.3f) {
            serialize<int32_t>(triggerDisplayOrder, numTriggers_);
        }
    }

    if (scn_ver == "1.22" || scn_ver == "1.21" || scn_ver == "1.20" || scn_ver == "1.19" || scn_ver == "1.18") {
        serialize<uint32_t>(includeFiles);
        serialize<uint32_t>(perErrorIncluded);

        if (perErrorIncluded) {
            serialize<uint32_t, 99>(perError);
        }

        if (includeFiles) {
            serializeSize<uint32_t>(fileCount_, includedFiles.size());
            serialize<ScnPersonalityScript>(includedFiles, fileCount_);
        }
    }

    compressor_.endCompression();
}

void ScnPersonalityScript::serializeObject(void)
{
    serializeSizedString<uint32_t>(filename, false);
    serializeSizedString<uint32_t>(content, false);
}

//------------------------------------------------------------------------------
void ScnFile::serializeVersion(void)
{
    /* Internal versions
    1.01 - 1.00
    1.02 - 1.03
    1.03 - 1.03
    1.04 - 1.04
    1.05 - 1.04
    1.06 - 1.06
    1.07 - 1.07
    1.08 - 1.08
    1.09 - 1.11
    1.10 - 1.11
    1.11 - 1.11
    1.12 - 1.12
    1.13 - 1.12
    1.14 - 1.12
    1.15 - 1.12
    1.16 - 1.12
    1.17 - 1.14 (CORE BUG: should be 1.13)
    1.18 - 1.13
    1.19 - 1.13
    1.20 - 1.14
    1.21 - 1.14
    */

    // "1.01"
    // "1.02"
    // "1.03"
    // "1.04"
    // "1.05"
    // "1.06"
    // "1.07"
    // "1.08"
    // "1.09"
    // "1.10" Rise of Rome? 1.10 is used for trial versions till 1.19
    // "1.11"
    // "1.12"
    // "1.13"
    // "1.14" from this onwards the handling is same.
    // "1.15"
    // "1.16"
    // "1.17"
    // "1.18" first AoK?
    // "1.19"
    // "1.20"
    // "1.21" The Conquerors?

    /*if (isOperation(OP_WRITE))
    {
    switch (getGameVersion())
    {
      case genie::GV_AoE:
      case genie::GV_RoR:
        version = "1.10";
        break;

      case genie::GV_AoK:
        version = "1.18";
        break;

      case genie::GV_TC:
      case genie::GV_SWGB:
      case genie::GV_CC:
        version = "1.21";
        break;

      default:
        break;
    }
    }*/

    version = scn_ver;
    serialize(version, 4);
    if (s_verbose) std::cout << "Version: " << version << std::endl;

    if (version[0] != '1' || version[1] != '.') {
        std::cerr << "Invalid version " << version << std::endl;
        throw std::runtime_error("Invalid campaign file version " + version);
    }

    if (version[2] < '0' || version[2] > '9' || version[3] < '0' || version[3] > '9') {
        std::cerr << "Invalid version " << version << std::endl;
        throw std::runtime_error("Invalid campaign file version " + version);
    }

    scn_ver = version;
}

//------------------------------------------------------------------------------
void ScnMainPlayerData::serializePlayerDataVersion(void)
{
    /*if (isOperation(OP_WRITE))
    {
    switch (getGameVersion())
    {
      case genie::GV_AoE:
      case genie::GV_RoR:
    //         playerDataVersion = ; //TODO
        break;

      case genie::GV_AoK:
        playerDataVersion = 1.18;
        break;

      case genie::GV_TC:
        playerDataVersion = 1.22;
        break;

      case genie::GV_SWGB:
      case genie::GV_CC:
        playerDataVersion = 1.30;
        break;

      default:
        break;
    }
    }*/

    playerDataVersion = scn_plr_data_ver;
    serialize<float>(playerDataVersion);
    scn_plr_data_ver = playerDataVersion;

    if (s_verbose) std::cout << "Player data version " << playerDataVersion << std::endl;

    /*if (isOperation(OP_READ))
    {
    if (fabs(playerDataVersion - 1.18) < 0.01)
      setGameVersion(genie::GV_AoK);
    else if (fabs(playerDataVersion - 1.22) < 0.01)
        setGameVersion(genie::GV_TC);
    else if (fabs(playerDataVersion - 1.30) < 0.01)
      setGameVersion(genie::GV_SWGB);
    else
      setGameVersion(genie::GV_AoE);
    }*/
}

void CpxFile::serializeObject()
{
    //TODO: support DE1/DE2: https://github.com/withmorten/rge_campaign/commit/e3ab7f1bea375343fdcf9bcfce3c502d82f21005

    s_verbose = verbose;

    serialize(version, 4);
    if (s_verbose) std::cout << "Campaign version: " << version << std::endl;

    serialize(name, 256);

    if (s_verbose) std::cout << "Campaign name: " << name << std::endl;
    serializeSize<uint32_t>(filecount, m_files.size());
    if (s_verbose) std::cout << "Files: " << filecount << std::endl;
    serialize<CpxIncludedFile>(m_files, filecount);

    // Only differentiator I've found for SWGB vs. normal campaign files
    if (isOperation(OP_READ)) {
        for (CpxIncludedFile &file : m_files) {
            if (util::stringStartsWith(util::toLowercase(file.filename), util::toLowercase(name))) {
                file.probablySwgb = true;
            }
        }
    }
}

std::vector<std::string> CpxFile::getFilenames() const
{
    std::vector<std::string> ret;

    for (const CpxIncludedFile &f : m_files) {
        ret.push_back(f.filename);
    }

    return ret;
}

ScnFilePtr CpxFile::getScnFile(const std::string &filename)
{
    for (CpxIncludedFile &f : m_files) {
        if (f.filename == filename) {
            return f.getScnFile();
        }
    }

    return nullptr;
}

ScnFilePtr CpxFile::getScnFile(size_t index)
{
    if (m_files.empty()) {
        std::cerr << "no files available" << std::endl;
        return nullptr;
    }

    if (index >= m_files.size()) {
        std::cerr << "index out of range: " << index << " " << m_files.size() << std::endl;
        index = 0;
    }

    return m_files[index].getScnFile();
}

CpxIncludedFile CpxFile::getRawFile(const std::string filename)
{
    for (CpxIncludedFile &f : m_files) {
        if (f.filename == filename) {
            return f;
        }
    }

    return {};
}

ScnFilePtr CpxIncludedFile::getScnFile()
{
    ScnFilePtr ret = std::make_shared<ScnFile>();
    ret->assumeSwgb = probablySwgb;
    ret->verbose = s_verbose;
    ret->setInitialReadPosition(offset);
    ret->readObject(*getIStream());
    return ret;
}

bool CpxIncludedFile::extractTo(const std::string &outPath)
{
    ScnFilePtr ret = std::make_shared<ScnFile>();
    ret->verbose = s_verbose;
    ret->setInitialReadPosition(offset);
    std::string content;
    content.resize(size);

    getIStream()->seekg(offset);
    getIStream()->read(content.data(), content.size());

    std::istringstream istr(content);
//    std::ifstream ifs;
    std::ofstream ofs;
    ofs.open(outPath, std::ios::binary);
    if (!ofs.is_open()) {
        std::cerr << "Failed to open " << outPath << std::endl;
        return false;
    }
    ret->extractRaw(istr, ofs);
    return true;
}

void CpxIncludedFile::serializeObject()
{
    serialize<uint32_t>(size);
    if (s_verbose) std::cout << "File size: " << size << std::endl;
    serialize<uint32_t>(offset);
    if (s_verbose) std::cout << "File offset: " << offset << std::endl;
    serialize(identifier, 255);
    if (s_verbose) std::cout << "File identifier: " << util::sanitizeAscii(identifier) << std::endl;
    if (!util::isAscii(identifier)) {
        std::cerr << "Invalid identifier" << std::endl;
        throw std::runtime_error("Invalid file identifier '" + util::sanitizeAscii(identifier) + "'");
    }
    serialize(filename, 257);
    if (s_verbose) std::cout << "File name: " << util::sanitizeAscii(filename) << std::endl;
    if (!util::isAscii(filename)) {
        std::cerr << "Invalid filename" << std::endl;
        throw std::runtime_error("Invalid file name '" + util::sanitizeAscii(filename) + "'");
    }
}

BlnFile::BlnFile() :
    compressor_(this)
{
}

void BlnFile::serializeObject()
{
    compressor_.beginCompression();

    serialize<float>(version);

    for (Frame &frame : frames) {
        for (Palette &palette : frame.palettes) {
            serialize<uint8_t, 256>(palette.colors);
        }
    }

    compressor_.endCompression();
}

} // namespace genie
