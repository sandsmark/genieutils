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

    /// This armor is used for attack types that have no corresponding armor type.
    /** Can be negative only in The Conquerors and later games.
      * int8_t for AoE1, including definitive edition, int16_t otherwise
      */
    int16_t BaseArmor = 1000;

    /// List of attack types with their damage
    std::vector<unit::AttackOrArmor> Attacks;

    /// List of armor values for different attack types
    std::vector<unit::AttackOrArmor> Armours;

    /// Terrain table ID
    /** Receive damage based on which terrain this unit stands on.
      * Effect attribute 18 changes this.
      * The damage received by this unit is
      * multiplied by the accessible values on
      * the specified terrain table. */
    int16_t DefenseTerrainBonus = -1;

    /// Maximum distance the unit can attack from
    float MaxRange = 0;

    /// The splash damage radius
    /**
     * If object has 0 blast radius and does not hit the unit it had targeted always does half damage
     */
    float BlastWidth = 0;

    /// Amount of time it takes to reload the weapon
    float ReloadTime = 0;

    /// ID of the unit spawned for projectiles
    int16_t ProjectileUnitID = -1;

    /// Percentage value determining the probability of an attack hiting
    int16_t AccuracyPercent = 0;

    /// Used in AoE/RoR for towers.
    int8_t BreakOffCombat = 0;

    /// Graphical delay in frames before projectile is shot
    int16_t FrameDelay = 0;

    /// Projectile spawning offset from the unit (x, y, z)
    XYZF GraphicDisplacement = { 0, 0, 0 };

    enum BlastAttackLevels : int8_t {
        /// Invalid/unset
        InvalidBlastLevel = -1,

        /// Damages resources also
        DamageResources = 0,

        /// Also damages trees
        DamageTrees = 1,

        /// Damages nearby units (aka. splash damage)
        DamageNearbyUnits = 2,


        /// Damages only targeted unit
        DamageOnlyTargetUnit = 3
    };

    /// Blasts damage units that have higher or same blast armor level
    BlastAttackLevels BlastAttackLevel = InvalidBlastLevel;

    /// Minimum attack range, if closer than this have to move away from target unit
    float MinRange = 0;

    /// Arc multiplier of the sector where the projectiles may hit, AoK Beta and later.
    /**
     * Higher values will make missed hits disperse more.
     */
    float AccuracyDispersion = 0;

    /// Sprite shown while attacking
    int16_t AttackGraphic = -1;

    /// Melee armor shown in the UI, AoE1 Beta and later
    int16_t DisplayedMeleeArmour = 0;

    /// Attack damage shown in the UI, AoE1 Beta and later
    int16_t DisplayedAttack = 0;

    /// Attack range shown in the UI, AoE1 Beta and later
    float DisplayedRange = 0;

    /// Reload time shown in the UI, AoE1 Beta and later
    float DisplayedReloadTime = 0;

    bool compareTo(const Combat &other) const;

protected:
    void serializeObject() override;
};
} // namespace unit
} // namespace genie
