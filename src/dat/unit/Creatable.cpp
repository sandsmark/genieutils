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

#include "genie/dat/unit/Creatable.h"

#include "../TestHelpers.h"

namespace genie {

namespace unit {

Creatable::Creatable() :
    //Type50(),
    ResourceCosts(3)
{
}

unsigned short Creatable::getResourceCostsSize()
{
    return 3;
}

bool Creatable::compareTo(const Creatable &other) const
{
    COMPARE_MEMBER(FlankAttackModifier);
    COMPARE_MEMBER(RearAttackModifier);
    COMPARE_MEMBER(TotalProjectiles);
    COMPARE_MEMBER(DisplayedPierceArmour);
    COMPARE_MEMBER(SpawningGraphic);
    COMPARE_MEMBER(TrainLocationID);
    COMPARE_MEMBER(TrainTime);
    COMPARE_MEMBER(UpgradeGraphic);
    COMPARE_MEMBER(GarrisonGraphic);
    COMPARE_MEMBER(SecondaryProjectileUnit);
    COMPARE_MEMBER(SpecialGraphic);
    COMPARE_MEMBER(ButtonID);
    COMPARE_MEMBER(CreatableType);
    COMPARE_MEMBER(HeroMode);
    COMPARE_MEMBER(MaxTotalProjectiles);
    COMPARE_MEMBER(SpecialAbility);
    COMPARE_MEMBER_VEC(ResourceCosts);
    COMPARE_MEMBER(ProjectileSpawningArea);

    return true;
}

void Creatable::serializeObject(void)
{
    GameVersion gv = getGameVersion();

    serialize(ResourceCosts, 3);
    serialize<int16_t>(TrainTime);
    serialize<int16_t>(TrainLocationID);
    serialize<int8_t>(ButtonID);

    if (gv >= GV_AoEB) { // 7.01
        if (gv >= GV_AoKE3) { // 9.07
            serialize<float>(RearAttackModifier);
            serialize<float>(FlankAttackModifier);
            serialize<int8_t>(CreatableType);

            if (gv >= GV_AoKB) {
                serialize<int8_t>(HeroMode); // 10.49
                serialize<int32_t>(GarrisonGraphic); // 10.73

                if (gv <= GV_LatestDE2 && gv >= GV_C13) {
                    serialize<int16_t>(SpawningGraphic);
                    serialize<int16_t>(UpgradeGraphic);
                }
            }

            serialize<float>(TotalProjectiles);
            serialize<int8_t>(MaxTotalProjectiles);
            serialize<float>(ProjectileSpawningArea, 3);
            serialize<int32_t>(SecondaryProjectileUnit); // 9.08
            // 9.2
            {
                serialize<int32_t>(SpecialGraphic);
                serialize<int8_t>(SpecialAbility);
            }
        }

        serialize<int16_t>(DisplayedPierceArmour);
    }
}
} // namespace unit
} // namespace genie
