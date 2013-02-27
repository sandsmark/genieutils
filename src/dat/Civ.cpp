/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
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


#include "genie/dat/Civ.h"

namespace genie
{

Civ::Civ(GameVersion gv)
{
  setGameVersion(gv);
  One = 1;
  Name = "";
  Name2 = "";
  TechTreeID = -1;
  TeamBonusID = -1;
  GraphicSet = 0;
  SUnknown1.resize(4);
}

Civ::~Civ()
{
}

void Civ::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
  
  updateGameVersion(Units);
}

uint16_t Civ::getNameSize(void)
{
  return 20;
}

void Civ::serializeObject(void )
{
  serialize<char>(One); //TODO: enabled flag
  serialize<std::string>(Name, getNameSize());
  serializeSize<uint16_t>(ResourceCount, Resources.size());
  serialize<int16_t>(TechTreeID);
  
  if (getGameVersion() >= genie::GV_AoK)
    serialize<int16_t>(TeamBonusID);
  
  if (getGameVersion() >= genie::GV_SWGB)
  {
    serialize<std::string>(Name2, getNameSize());
    serialize<int16_t>(SUnknown1, 4);
  }
  
  serialize<float>(Resources, ResourceCount);
  
  serialize<char>(GraphicSet);
  
  serializeSize<uint16_t>(UnitCount, UnitPointers.size());
  serialize<int32_t>(UnitPointers, UnitCount);
  serializeSubWithPointers<Unit>(Units, UnitCount, UnitPointers);
}

}
