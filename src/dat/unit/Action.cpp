/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2020  Mikko "Tapsa" P

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

#include "genie/dat/unit/Action.h"

#include "../TestHelpers.h"

namespace genie::unit {
//------------------------------------------------------------------------------
Action::Action() : DropSites(2, -1)
{
}

void Action::setGameVersion(GameVersion gv)
{
    if (gv == getGameVersion()) {
        return;
    }

    ISerializable::setGameVersion(gv);

    DropSites.resize(getDropSiteCount(), -1);

    updateGameVersion(TaskList);
}

unsigned short Action::getDropSiteCount()
{
    GameVersion gv = getGameVersion();

    if (gv >= GV_C15 && gv <= GV_LatestDE2) {
        return 3;
    }

    if (gv >= GV_TEST) {
        return 2;
    }

    return 1;
}

bool Action::compareTo(const Action &other) const
{
    COMPARE_MEMBER(SearchRadius);
    COMPARE_MEMBER(WorkRate);
    COMPARE_MEMBER(AttackSound);
    COMPARE_MEMBER(DefaultTaskID);
    COMPARE_MEMBER(MoveSound);
    COMPARE_MEMBER(RunPattern);
    COMPARE_MEMBER(TaskSwapGroup);
    COMPARE_MEMBER_VEC(DropSites);
    COMPARE_MEMBER_OBJ_VEC(TaskList);
    COMPARE_MEMBER(WwiseAttackSoundID);
    COMPARE_MEMBER(WwiseMoveSoundID);

    return true;
}

void Action::serializeObject()
{
    GameVersion gv = getGameVersion();

    serialize<int16_t>(DefaultTaskID);
    serialize<float>(SearchRadius);
    serialize<float>(WorkRate);
    serialize<int16_t>(DropSites, getDropSiteCount());
    serialize<int8_t>(TaskSwapGroup);
    serialize<int16_t>(AttackSound);

    if (gv >= GV_AoEB) {
        serialize<int16_t>(MoveSound); // 6.92
    }

    if (gv >= GV_C4 && gv <= GV_LatestDE2) {
        serialize<uint32_t>(WwiseAttackSoundID);
        serialize<uint32_t>(WwiseMoveSoundID);
    }

    serialize<int8_t>(RunPattern);

    if (gv < GV_AoK // 11.24
            || (gv >= GV_C15 && gv <= GV_LatestDE2)) {
        uint16_t task_count{};
        serializeSize<uint16_t>(task_count, TaskList.size());
        serialize(TaskList, task_count);
    }
}
} // namespace genie
