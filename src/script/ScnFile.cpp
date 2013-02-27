/*
    genieutils - <description>
    Copyright (C) 2013  Armin Preiml <email>

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

#include "genie/script/scn/ScnPlayerData.h"

#include <math.h>
#include <boost/iostreams/copy.hpp>

namespace genie
{
  
//------------------------------------------------------------------------------
ScnFile::ScnFile() : IFile(), compressor_(this)
{
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
  
//   boost::shared_ptr<std::istream> dec = compressor_.startDecompression(&ifs);
  
//   boost::iostreams::copy((*dec), ofs);
  
//   compressor_.stopDecompression();
  
  ifs.close();
  ofs.close();
  
}

//------------------------------------------------------------------------------
uint32_t ScnFile::getSeparator(void)
{
  return 0xFFFFFF9D;
}

//------------------------------------------------------------------------------
void ScnFile::serializeObject(void)
{
  serializeVersion();
  
  serialize<uint32_t>(headerLength_);
  serialize<int32_t>(unknown1);
  serialize<uint32_t>(lastSaveTime);
  
  serializeSizedString<uint32_t>(scenarioInstructions);
  
  serialize<uint32_t>(unknown2);
  serialize<uint32_t>(playerCount);
  
  std::cout << "Start compression: " << tellg() << std::endl;
  
  compressor_.beginCompression();
 
  std::cout << "Start compression: " << tellg() << std::endl;
  
  // Compressed header:
  
  serialize<uint32_t>(unknown3);
  serializeVersion2();
  
  if (isOperation(OP_READ))
  {
    playerNames.resize(16);
    for (uint8_t i=0; i<16; i++)
      playerNames[i] = readString(256);
  }
  else
    for (uint8_t i=0; i<16; i++)
      writeString(playerNames[i], 256);
  
  if (getGameVersion() >= genie::GV_AoK)
    serialize<uint32_t>(playerNamesStringTable, 16);
  
  
  serializeSub<ScnPlayerData1>(playerData1, 16);
  
  
  serialize<uint32_t>(unknown4);
  
  serialize<char>(unknown5); //TODO
  
  serialize<float>(unknown6);
  
  serializeSizedString<uint16_t>(originalFileName);
  
  // Messages and cinematics
  
  serialize<ISerializable>(resource);
  
  serialize<ISerializable>(playerData2);
  
  uint32_t *victoryConditions = 0; // TODO diplomacy
  
  serialize<uint32_t>(&victoryConditions, 11);
  
  delete [] victoryConditions;
  
  char *diplomacy = 0;
  
  serialize<char>(&diplomacy, 12608);
  
  delete [] diplomacy;
  
  char *disables = 0;
  
  serialize<char>(&disables, 5388);
  
  delete [] disables;
  
  read<uint32_t>();
  
  uint32_t separator = 0;
  
  serialize<uint32_t>(separator);
  std::cout << "sep: " << std::hex << separator << std::endl;
  
  serialize<ISerializable>(map);
  
  compressor_.endCompression();
}

//------------------------------------------------------------------------------
void ScnFile::serializeVersion(void)
{
  if (isOperation(OP_WRITE))
  {
    switch (getGameVersion())
    {
      case genie::GV_AoE:
      case genie::GV_RoR:
        version_ = "1.10";
        break;
        
      case genie::GV_AoK:
        version_ = "1.18";
        break;
        
      case genie::GV_TC:
      case genie::GV_SWGB:
      case genie::GV_CC:
        version_ = "1.21";
        break;
      
      default:
        break;
    }
  }
  
  serialize<std::string>(version_, 4);
}

//------------------------------------------------------------------------------
void ScnFile::serializeVersion2(void)
{
  if (isOperation(OP_WRITE))
  {
    switch (getGameVersion())
    {
      case genie::GV_AoE:
      case genie::GV_RoR:
//         version2_ = ; //TODO
        break;
        
      case genie::GV_AoK:
        version2_ = 1.18;
        break;
        
      case genie::GV_TC:
        version2_ = 1.22;
        break;
        
      case genie::GV_SWGB:
      case genie::GV_CC:
        version2_ = 1.30;
        break;
      
      default:
        break;
    }
  }  
  
  serialize<float>(version2_);
  
  if (isOperation(OP_READ))
  {
    if (fabs(version2_ - 1.18) < 0.01)
      setGameVersion(genie::GV_AoK);
    else if (fabs(version2_ - 1.22) < 0.01)
        setGameVersion(genie::GV_TC);
    else if (fabs(version2_ - 1.30) < 0.01)
      setGameVersion(genie::GV_SWGB);
    else
      setGameVersion(genie::GV_AoE);
  } 
}

}