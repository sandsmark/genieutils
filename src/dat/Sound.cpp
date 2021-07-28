/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2019  Mikko "Tapsa" P

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

#include "genie/dat/Sound.h"

#include "TestHelpers.h"

namespace genie {

//------------------------------------------------------------------------------
void Sound::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);

    updateGameVersion(Items);
}

bool Sound::compareTo(const Sound &other) const
{
    COMPARE_MEMBER(ID);
    COMPARE_MEMBER(PlayDelay);
    COMPARE_MEMBER(CacheTime); // 5 minutes
    COMPARE_MEMBER(TotalProbability);
    COMPARE_MEMBER_OBJ_VEC(Items);

    return true;
}

void Sound::serializeObject()
{
    GameVersion gv = getGameVersion();
    serialize<int16_t>(ID);
    serialize<int16_t>(PlayDelay);

    uint16_t file_count{};
    serializeSize<uint16_t>(file_count, Items.size());

    if (gv >= GV_TEST) {
        serialize<int32_t>(CacheTime);

        if ((gv >= GV_T8 && gv <= GV_LatestTap) || (gv >= GV_C11 && gv <= GV_LatestDE2)) {
            serialize<int16_t>(TotalProbability);
        }
    }

    serialize(Items, file_count);
}
} // namespace genie
