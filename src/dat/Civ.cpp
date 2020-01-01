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

#include "genie/dat/Civ.h"

#include "TestHelpers.h"

namespace genie {

void Civ::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);

    updateGameVersion(Units);
}

unsigned short Civ::getNameSize(void)
{
    return 20;
}

bool Civ::compareTo(const Civ &other) const
{
    COMPARE_MEMBER(TeamBonusID);
    COMPARE_MEMBER(TechTreeID);
    COMPARE_MEMBER(IconSet);
    COMPARE_MEMBER(PlayerType);
    COMPARE_MEMBER(Name);
    COMPARE_MEMBER(Name2);
    COMPARE_MEMBER_OBJ_VEC(Units);
    COMPARE_MEMBER_VEC(Resources);
    COMPARE_MEMBER_VEC(UniqueUnitsTechs);
    COMPARE_MEMBER_VEC(UnitPointers);

    return true;
}

void Civ::serializeObject(void)
{
    GameVersion gv = getGameVersion();

    serialize<int8_t>(PlayerType);

    if ((gv > GV_LatestTap && gv < GV_C2) || gv < GV_Tapsa || gv > GV_LatestDE2) {
        serialize(Name, getNameSize());
    } else {
        serializeDebugString(Name);
    }

    uint16_t count{};
    serializeSize<uint16_t>(count, Resources.size());

    if (gv >= GV_MIK) {
        serialize<int16_t>(TechTreeID);

        if (gv >= GV_AoKB) { // 10.38
            serialize<int16_t>(TeamBonusID);

            if (gv >= GV_SWGB) {
                serialize(Name2, getNameSize());
                serialize<int16_t>(UniqueUnitsTechs, 4);
            }
        }
    }

    serialize<float>(Resources, count);

    serialize<int8_t>(IconSet);

    serializeSize<uint16_t>(count, Units.size());
    serialize<int32_t>(UnitPointers, count);
    serializeSubWithPointers<Unit>(Units, count, UnitPointers);
}
} // namespace genie
