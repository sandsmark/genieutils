/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2013  Mikko "Tapsa" P

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

#include "genie/dat/Civ.h"

namespace genie
{

Civ::Civ()
{
  One = 1;
  Name = "";
  Name2 = "";
  TechTreeID = -1;
  TeamBonusID = -1;
  IconSet = 0;
}

Civ::~Civ()
{
}

void Civ::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  updateGameVersion(Units);
}

unsigned short Civ::getNameSize(void)
{
  return 20;
}

void Civ::serializeObject(void)
{
  serialize<int8_t>(One); //TODO: enabled flag
  serialize<std::string>(Name, getNameSize());
  serializeSize<uint16_t>(ResourceCount, Resources.size());

  if (getGameVersion() >= genie::GV_MIK)
  {
    serialize<int16_t>(TechTreeID);
    if (getGameVersion() >= genie::GV_AoKB) // 10.38
    {
      serialize<int16_t>(TeamBonusID);

      if (getGameVersion() >= genie::GV_SWGB)
      {
        serialize<std::string>(Name2, getNameSize());
        serialize<int16_t>(UniqueUnitsResearches, SWUNIQUE_SIZE);
      }
    }
  }

  serialize<float>(Resources, ResourceCount);

  serialize<int8_t>(IconSet);

  serializeSize<uint16_t>(UnitCount, UnitPointers.size());
  serialize<int32_t>(UnitPointers, UnitCount);
  serializeSubWithPointers<Unit>(Units, UnitCount, UnitPointers);
}

}
