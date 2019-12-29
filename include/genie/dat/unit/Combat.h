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
#include "AttackOrArmor.h"

namespace genie {

namespace unit {

class Combat : public ISerializable
{
public:
    void setGameVersion(GameVersion gv) override;

    /// This armor is used for attack types that have no corresponding armor type
    /// Can be negative only in The Conquerors and later games
    int16_t BaseArmor = 1000; // uint8_t below TC

    std::vector<unit::AttackOrArmor> Attacks;
    std::vector<unit::AttackOrArmor> Armours;

    /// Terrain table ID
    /// Receive damage based on which terrain this unit stands on.
    /// Effect attribute 18 changes this.
    /// The damage received by this unit is
    /// multiplied by the accessible values on
    /// the specified terrain table.
    int16_t DefenseTerrainBonus = -1;

    float MaxRange = 0;

    /// If object has 0 blast radius
    /// and does not hit the unit it had targeted
    /// always does half damage
    float BlastWidth = 0;

    float ReloadTime = 0;
    int16_t ProjectileUnitID = -1;

    /// Percentage value determining the probability of an attack hiting
    int16_t AccuracyPercent = 0;

    /// Used in AoE/RoR for towers.
    int8_t BreakOffCombat = 0;

    /// Graphical delay in frames before projectile is shot
    int16_t FrameDelay = 0;

    /// Left/Right distance
    /// Spawning distance from the unit
    /// Height
    std::vector<float> GraphicDisplacement = { 0, 0, 0 };

    /// Blasts damage units that have higher or same blast armor level
    /// 0. Damages resources also
    /// 1. Damages trees also
    /// 2. Damages nearby units
    /// 3. Damages only targeted unit
    int8_t BlastAttackLevel = 0;

    float MinRange = 0;

    /// Arc multiplier of the sector where the projectiles may hit.
    /// Higher values will make missed hits disperse more.
    float AccuracyDispersion = 0;

    int16_t AttackGraphic = -1;
    int16_t DisplayedMeleeArmour = 0;
    int16_t DisplayedAttack = 0;
    float DisplayedRange = 0;
    float DisplayedReloadTime = 0;

    bool compareTo(const Combat &other) const;

protected:
    void serializeObject(void) override;
};
} // namespace unit
} // namespace genie
