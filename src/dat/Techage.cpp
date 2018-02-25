/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2017  Mikko "Tapsa" P

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

#include "genie/dat/Techage.h"

namespace genie
{

//------------------------------------------------------------------------------
Effect::Effect()
{
}

//------------------------------------------------------------------------------
Effect::~Effect()
{
}

//------------------------------------------------------------------------------
void Effect::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  updateGameVersion(EffectCommands);
}

unsigned short Effect::getNameSize(void)
{
  return 31;
}

//------------------------------------------------------------------------------
void Effect::serializeObject(void)
{
  GameVersion gv = getGameVersion();
  if (gv > GV_LatestTap || gv < GV_Tapsa)
  {
    serialize(Name, getNameSize());
  }
  else
  {
    serializeDebugString(Name);
  }

  uint16_t command_count;
  serializeSize<uint16_t>(command_count, EffectCommands.size());
  serializeSub<EffectCommand>(EffectCommands, command_count);
}

}
