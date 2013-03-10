/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml <email>
    Copyright (C) 2011 - 2013  Mikko T P

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

#include "genie/dat/Research.h"

namespace genie
{

//------------------------------------------------------------------------------
Research::Research() : ResourceCosts()
{
  RequiredTechCount = 0;
  Civ = -1;
  FullTechMode = 0;
  ResearchLocation = -1;
  LanguageDLLName = 7000;
  LanguageDLLDescription = 8000;
  ResearchTime = 0;
  TechageID = -1;
  Type = 0;
  IconID = -1;
  ButtonID = 0;
  LanguageDLLHelp = 107000;
  LanguageDLLName2 = 157000;
  Unknown1 = -1;
  NameLength = 0;
  Name = "";
  NameLength2 = 0;
  Name2 = "";
}

//------------------------------------------------------------------------------
Research::~Research()
{
}

void Research::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  RequiredTechs.resize(getRequiredTechsSize(), -1);
}

//------------------------------------------------------------------------------
unsigned short Research::getRequiredTechsSize()
{
  if (getGameVersion() >= genie::GV_AoKA)
    return 6;
  else
    return 4;
}

//------------------------------------------------------------------------------
void Research::serializeObject(void)
{
  serialize<int16_t>(RequiredTechs, getRequiredTechsSize());

  serializeSub<ResearchResourceCost, RESOURCECOSTS_SIZE>(ResourceCosts);
  serialize<int16_t>(RequiredTechCount);

  if (getGameVersion() >= genie::GV_AoK)
  {
    serialize<int16_t>(Civ);
    serialize<int16_t>(FullTechMode);
  }

  serialize<int16_t>(ResearchLocation);
  serialize<uint16_t>(LanguageDLLName);
  serialize<uint16_t>(LanguageDLLDescription);
  serialize<int16_t>(ResearchTime);
  serialize<int16_t>(TechageID);
  serialize<int16_t>(Type);
  serialize<int16_t>(IconID);
  serialize<char>(ButtonID);
  serialize<int32_t>(LanguageDLLHelp);
  serialize<int32_t>(LanguageDLLName2);
  serialize<int32_t>(Unknown1);

  serializeSize<uint16_t>(NameLength, Name);
  if (NameLength > 0)
    serialize<std::string>(Name, NameLength);

  if (getGameVersion() >= genie::GV_SWGB)
  {
    serializeSize<uint16_t>(NameLength2, Name2);
    if (NameLength2 > 0)
      serialize<std::string>(Name2, NameLength2);
  }

}

}
