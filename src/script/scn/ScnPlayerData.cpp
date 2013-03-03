/*
    genieutils - <description>
    Copyright (C) 2011 - 2013  Armin Preiml <email>

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

#include "genie/script/scn/ScnPlayerData.h"
#include "genie/script/ScnFile.h"

namespace genie
{
  
ScnPlayerData1::ScnPlayerData1()
{
}

ScnPlayerData1::~ScnPlayerData1()
{
}

void ScnPlayerData1::serializeObject(void)
{
  serialize<uint32_t>(active);
  serialize<uint32_t>(human);
  serialize<uint32_t>(civilizationId);
  serialize<uint32_t>(unknown1);
}

  
ScnPlayerData2::ScnPlayerData2()
{
  separator_ = ScnFile::getSeparator();
}

ScnPlayerData2::~ScnPlayerData2()
{
}

void ScnPlayerData2::serializeObject(void)
{
  serializeSizedStrings<uint16_t>(unknownStrings, 32);
  serializeSizedStrings<uint16_t>(aiNames, 16);
  
  serializeSub(aiFiles, 16);
  
  serialize<uint8_t>(aiTypes, 16);

  serialize<uint32_t>(separator_);
  
  //TODO
  if (separator_ != ScnFile::getSeparator())
    std::cerr << "Scn: File corruption!" << std::endl;
  
  serializeSub(resources, 16);
}
  
AiFile::AiFile()
{
}

AiFile::~AiFile()
{
}
  
void AiFile::serializeObject(void) 
{
  serialize<uint32_t>(unknown1);
  serialize<uint32_t>(unknown2);
  
  serializeSizedString<uint32_t>(perFile);
}

Resources::Resources()
{
}

Resources::~Resources()
{
}

void Resources::serializeObject(void)
{
  serialize<uint32_t>(gold);
  serialize<uint32_t>(wood);
  serialize<uint32_t>(food);
  serialize<uint32_t>(stone);
  serialize<uint32_t>(oreX);
  serialize<uint32_t>(padding);
}

}