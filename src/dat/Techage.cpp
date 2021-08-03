/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
<<<<<<< HEAD
    Copyright (C) 2011 - 2019  Mikko "Tapsa" P
=======
    Copyright (C) 2011 - 2021  Mikko "Tapsa" P
>>>>>>> 65dd660 (More accurate signedness.)

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

#include "TestHelpers.h"

namespace genie {

//------------------------------------------------------------------------------
void Effect::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);

    updateGameVersion(EffectCommands);
}

unsigned short Effect::getNameSize()
{
    return 31;
}

bool Effect::compareTo(const Effect &other) const
{
    COMPARE_MEMBER(Name);
    COMPARE_MEMBER_OBJ_VEC(EffectCommands);

    return true;
}

//------------------------------------------------------------------------------
void Effect::serializeObject()
{
    GameVersion gv = getGameVersion();

<<<<<<< HEAD
    if ((gv > GV_LatestTap && gv < GV_C2) || gv < GV_Tapsa || gv > GV_LatestDE2) {
        serialize(Name, getNameSize());
    } else {
        serializeDebugString(Name);
    }
=======
  int16_t command_count;
  serializeSize<int16_t>(command_count, EffectCommands.size());
  serializeSub<EffectCommand>(EffectCommands, command_count);
}
>>>>>>> 65dd660 (More accurate signedness.)

    uint16_t command_count{};
    serializeSize<uint16_t>(command_count, EffectCommands.size());
    serialize(EffectCommands, command_count);
}
} // namespace genie
