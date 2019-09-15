/*
    geniedat - A library for reading and writing data files of genie
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

#pragma once

#include "genie/file/ISerializable.h"
#include <limits>

namespace genie {

namespace unit {

/// Attributes for Unit of type >= DeadFish.
class Moving : public ISerializable
{
public:
    int16_t WalkingGraphic = -1;
    int16_t RunningGraphic = -1;
    float RotationSpeed = 0;
    int8_t OldSizeClass = 0;
    int16_t TrackingUnit = -1;

    /// 0. Not used
    /// 1. Appears while moving and at the start of the game
    /// 2. Appears while moving, based on density
    int8_t TrackingUnitMode = 0;

    /// Only with TrackingUnitMode 2
    float TrackingUnitDensity = 0;

    int8_t OldMoveAlgorithm = 0;

    // All can change while playing
    float TurnRadius = 0;
    float TurnRadiusSpeed = std::numeric_limits<float>::max();
    float MaxYawPerSecondMoving = std::numeric_limits<float>::max();
    float StationaryYawRevolutionTime = 0;
    float MaxYawPerSecondStationary = std::numeric_limits<float>::max();

protected:
    void serializeObject(void) override;
};
} // namespace unit
} // namespace genie
