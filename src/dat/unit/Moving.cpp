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

#include "genie/dat/unit/Moving.h"

#include "../TestHelpers.h"

namespace genie {

namespace unit {

bool Moving::compareTo(const Moving &other) const
{
    COMPARE_MEMBER(MaxYawPerSecondMoving);
    COMPARE_MEMBER(MaxYawPerSecondStationary);
    COMPARE_MEMBER(MinCollisionSizeMultiplier);
    COMPARE_MEMBER(RotationSpeed);
    COMPARE_MEMBER(StationaryYawRevolutionTime);
    COMPARE_MEMBER(TrackingUnitDensity);
    COMPARE_MEMBER(TurnRadius);
    COMPARE_MEMBER(TurnRadiusSpeed);
    COMPARE_MEMBER(RunningGraphic);
    COMPARE_MEMBER(TrackingUnit);
    COMPARE_MEMBER(WalkingGraphic);
    COMPARE_MEMBER(OldMoveAlgorithm);
    COMPARE_MEMBER(OldSizeClass);
    COMPARE_MEMBER(TrackingUnitMode);

    return true;
}

void Moving::serializeObject()
{
    GameVersion gv = getGameVersion();

    serialize<int16_t>(WalkingGraphic);
    serialize<int16_t>(RunningGraphic);
    serialize<float>(RotationSpeed);
    serialize<int8_t>(OldSizeClass);
    serialize<int16_t>(TrackingUnit);
    serialize<int8_t>(TrackingUnitMode);
    serialize<float>(TrackingUnitDensity);
    serialize<int8_t>(OldMoveAlgorithm);

    if (gv >= GV_AoKB) { // 10.28
        serialize<float>(TurnRadius);
        serialize<float>(TurnRadiusSpeed);
        serialize<float>(MaxYawPerSecondMoving);
        serialize<float>(StationaryYawRevolutionTime);
        serialize<float>(MaxYawPerSecondStationary);
    }

    if (gv <= GV_LatestDE2 && gv >= GV_C14) {
        serialize<float>(MinCollisionSizeMultiplier);
    }
}
} // namespace unit
} // namespace genie
