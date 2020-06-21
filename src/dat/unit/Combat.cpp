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

#include "genie/dat/unit/Combat.h"

#include "../TestHelpers.h"

namespace genie {

namespace unit {
//------------------------------------------------------------------------------
void Combat::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);
}

bool Combat::compareTo(const Combat &other) const
{
    COMPARE_MEMBER(AccuracyDispersion);
    COMPARE_MEMBER(BlastWidth);
    COMPARE_MEMBER(DisplayedRange);
    COMPARE_MEMBER(DisplayedReloadTime);
    COMPARE_MEMBER(MaxRange);
    COMPARE_MEMBER(MinRange);
    COMPARE_MEMBER(ReloadTime);
    COMPARE_MEMBER(AccuracyPercent);
    COMPARE_MEMBER(AttackGraphic);
    COMPARE_MEMBER(BaseArmor);
    COMPARE_MEMBER(DefenseTerrainBonus);
    COMPARE_MEMBER(DisplayedAttack);
    COMPARE_MEMBER(DisplayedMeleeArmour);
    COMPARE_MEMBER(FrameDelay);
    COMPARE_MEMBER(ProjectileUnitID);
    COMPARE_MEMBER(BlastAttackLevel);
    COMPARE_MEMBER(BreakOffCombat);
    COMPARE_MEMBER_VEC(GraphicDisplacement);
    COMPARE_MEMBER_OBJ_VEC(Armours);
    COMPARE_MEMBER_OBJ_VEC(Attacks);

    return true;
}

void Combat::serializeObject(void)
{
    GameVersion gv = getGameVersion();

    if (gv < GV_TC // 11.52
            && (gv > GV_LatestTap || gv < GV_T3)) {
        uint8_t defarmor_byte = BaseArmor;
        serialize<uint8_t>(defarmor_byte);
        BaseArmor = defarmor_byte;
    } else {
        serialize<int16_t>(BaseArmor);
    }

    uint16_t attack_count{};
    serializeSize<uint16_t>(attack_count, Attacks.size());
    serialize(Attacks, attack_count);

    uint16_t armours_count{};
    serializeSize<uint16_t>(armours_count, Armours.size());
    serialize(Armours, armours_count);

    serialize<int16_t>(DefenseTerrainBonus);
    serialize<float>(MaxRange);
    serialize<float>(BlastWidth);
    serialize<float>(ReloadTime);
    serialize<int16_t>(ProjectileUnitID);
    serialize<int16_t>(AccuracyPercent);
    serialize<int8_t>(BreakOffCombat); // Not used anymore
    serialize<int16_t>(FrameDelay);
    serialize<float, 3>(GraphicDisplacement);
    serialize<int8_t>(BlastAttackLevel);
    serialize<float>(MinRange);

    if (gv >= GV_AoKB) { // 10.36
        serialize<float>(AccuracyDispersion);
    }

    serialize<int16_t>(AttackGraphic);

    if (gv >= GV_AoEB) { // 7.01
        serialize<int16_t>(DisplayedMeleeArmour);
        serialize<int16_t>(DisplayedAttack);
        serialize<float>(DisplayedRange);
        serialize<float>(DisplayedReloadTime);
    }
}
} // namespace unit
} // namespace genie
