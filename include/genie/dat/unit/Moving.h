/*
    geniedat - A library for reading and writing data files of genie
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

#pragma once

#include "genie/file/ISerializable.h"
#include <limits>

namespace genie {

namespace unit {

/// Attributes for Unit of type >= DeadFish.
class Moving : public ISerializable
{
public:
    /// Sprite used when unit is moving normally
    int16_t WalkingGraphic = -1;

    /// Sprite when unit is running
    int16_t RunningGraphic = -1;

    /// Max rotational speed
    float RotationSpeed = 0;

    /// Old type of size
    int8_t OldSizeClass = 0;

    /// Which unit is this tracking (?)
    int16_t TrackingUnit = -1;

    /// 0. Not used\n
    /// 1. Appears while moving and at the start of the game\n
    /// 2. Appears while moving, based on density\n
    int8_t TrackingUnitMode = 0;

    /// Only with TrackingUnitMode 2
    float TrackingUnitDensity = 0;

    /// Old algorithm used to define how the unit moves
    int8_t OldMoveAlgorithm = 0;

    /// How far is the unit turned, changed while playing
    float TurnRadius = 0;

    /// Speed of rotation, changed while playing
    float TurnRadiusSpeed = std::numeric_limits<float>::max();

    /// Max yaw change when moving, changed when playing
    float MaxYawPerSecondMoving = std::numeric_limits<float>::max();

    /// How long has the unit been not rotating, changed while playing
    float StationaryYawRevolutionTime = 0;

    /// Max yaw change per second when not moving forward, changed while playing
    float MaxYawPerSecondStationary = std::numeric_limits<float>::max();

    float MinCollisionSizeMultiplier = 1.0f;

    bool compareTo(const Moving &other) const;

protected:
    void serializeObject() override;
};
} // namespace unit
} // namespace genie
