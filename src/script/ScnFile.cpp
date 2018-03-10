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

#include <math.h>

namespace genie {

std::string ISerializable::scn_ver = "0.00";
float ISerializable::scn_plr_data_ver = 0.f;
float ISerializable::scn_internal_ver = 0.f;
double ISerializable::scn_trigger_ver = 0.0;

Logger &ScnFile::log = Logger::getLogger("genie.ScnFile");




//////////
// cpx format:
// header
//  - 4 char version
//  - 5 char name
//  - 254 bytes total header?
//  - 40 bytes offset:
//     - 0xff ff ff ff == separator?
//  -
// block header per file
//  - int32 length
//  - int32 pos/offset in cpx
//  - 519 - 256 char == name?
//  - 256 char = filename
// Files in plain (.scn etc.)
//

//------------------------------------------------------------------------------
ScnFile::ScnFile() :
    IFile(), compressor_(this)
{
    scn_internal_ver = 0.f;
}

//------------------------------------------------------------------------------
ScnFile::~ScnFile()
{
}

void ScnFile::extractRaw(const char *from, const char *to)
{
    std::ifstream ifs;
    std::ofstream ofs;

    ifs.open(from, std::ios::binary);
    ofs.open(to, std::ios::binary);

    char version[4];
    ifs.read(version, 4);
    ofs.write(version, 4);

    uint32_t headerLen;

    ifs.read(reinterpret_cast<char *>(&headerLen), 4);
    ofs.write(reinterpret_cast<char *>(&headerLen), 4);

    char header[headerLen];

    ifs.read(header, headerLen);
    ofs.write(header, headerLen);

    Compressor::decompress(ifs, ofs);

    //   std::shared_ptr<std::istream> dec = compressor_.startDecompression(&ifs);

    //   compressor_.stopDecompression();

    ifs.close();
    ofs.close();
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
    serializeVersion();
    if (isOperation(OP_READ) && !verifyVersion()) {
        std::cout << "ERROR" << std::endl;
        return;
    }
    std::cout << "version: " << version << std::endl;

    if (isOperation(OP_WRITE)) {
        headerLength_ = 21 + scenarioInstructions.size();
    }

    serialize<uint32_t>(headerLength_); // Used in AoE 1 lobby

    std::cout << "header length: " << headerLength_ << std::endl;

    serialize<int32_t>(saveType);
    std::cout << "save type: " << headerLength_ << std::endl;
    serialize<uint32_t>(lastSaveTime);
    serializeForcedString<uint32_t>(scenarioInstructions);
    serialize<uint32_t>(victoryType);
    serialize<uint32_t>(playerCount);

    compressor_.beginCompression();

    // Compressed header:

    serialize<uint32_t>(nextUnitID);

    serialize<ISerializable>(playerData);

    serialize<ISerializable>(map);

    if (scn_ver == "1.20" || scn_ver == "1.21")
        scn_internal_ver = 1.14f;
    else if (scn_ver == "1.17" || scn_ver == "1.18" || scn_ver == "1.19")
        scn_internal_ver = 1.13f;
    else if (scn_ver == "1.14" || scn_ver == "1.15" || scn_ver == "1.16")
        scn_internal_ver = 1.12f;

    serializeSize<uint32_t>(playerCount1_, playerUnits.size());
    if (scn_internal_ver > 1.06f)
        serializeSub<ScnPlayerResources>(playerResources, 8);
    else {
        // A lot of data is read here.
    }
    serializeSub<ScnPlayerUnits>(playerUnits, playerCount1_);

    serialize<uint32_t>(playerCount2_);
    serializeSub<ScnMorePlayerData>(players, 8);

    triggerVersion = scn_trigger_ver;
    serialize<double>(triggerVersion);
    scn_trigger_ver = triggerVersion;

    if (scn_trigger_ver > 1.4f)
        serialize<int8_t>(objectivesStartingState);
    serializeSize<uint32_t>(numTriggers_, triggers.size());
    serializeSub<Trigger>(triggers, numTriggers_);
    if (scn_trigger_ver > 1.3f)
        serialize<int32_t>(triggerDisplayOrder, numTriggers_);

    if (scn_ver == "1.21" || scn_ver == "1.20" || scn_ver == "1.19" || scn_ver == "1.18") {
        serialize<uint32_t>(includeFiles);
        serialize<uint32_t>(perErrorIncluded);
        if (perErrorIncluded)
            serialize<uint32_t>(perError, 99);
        if (includeFiles) {
            serializeSize<uint32_t>(fileCount_, includedFiles.size());
            serializeSub<ScnIncludedFile>(includedFiles, fileCount_);
        }
    }

    compressor_.endCompression();
}

ScnIncludedFile::ScnIncludedFile()
{
}

ScnIncludedFile::~ScnIncludedFile()
{
}

void ScnIncludedFile::serializeObject(void)
{
    serializeSizedString<uint32_t>(perFileName, false);
    serializeSizedString<uint32_t>(someString, false);
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
}
