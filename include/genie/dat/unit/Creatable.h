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

#include <genie/file/ISerializable.h>
#include <genie/dat/ResourceUsage.h>

namespace genie {

namespace unit {

class Creatable : public ISerializable
{
public:
    Creatable();

    static unsigned short getResourceCostsSize();

    typedef Resource<int16_t, int16_t> ResourceCost;

    std::vector<ResourceCost> ResourceCosts;
    int16_t TrainTime = 0;

    /// ID of unit where this is made
    int16_t TrainLocationID = -1;

    /// Position in UI
    // First page (also second in TC) 1-15
    // Second (dock) page 21-35
    // Third page same as first (Star Wars)
    // First page in AoE/RoR 1-10
    // Second page in AoE/RoR 11-20
    int8_t ButtonID = 0;

    float RearAttackModifier = 0;
    float FlankAttackModifier = 0;

    enum CreatableTypes : int8_t {
        NonHumanType = 0, // building, animal, ship
        VillagerType = 1, // villager, king
        InfantryType = 2, // soldier, siege, predator, trader
        CavalryType = 3, // camel rider
        RelicType = 4,
        ArcherType = 5,
        MonkType = 6,
        TransportShipType = 21
    };
    int8_t CreatableType = 0; // obsolete according to AGE, but still set for units

    int8_t HeroMode = 0;
    int32_t GarrisonGraphic = -1;

    /// Total missiles including both normal and duplicated projectiles
    float TotalProjectiles = 0;

    /// Total missiles when garrison capacity is full
    int8_t MaxTotalProjectiles = 0;

    /// The area from which projectiles spawn. Size and randomness.
    /// X: Spawning area's width
    /// Y: Spawning area's length
    /// Z: 0   From a single spot
    ///    1   Totally randomly inside the spawning area
    ///    1+  Less randomly
    std::vector<float> ProjectileSpawningArea = { 0, 0, 0 };

    /// Uses its own attack values
    int32_t SecondaryProjectileUnit = -1;

    /// Activates depending on special ability
    int32_t SpecialGraphic = -1;

    //TODO:
    /*
    * 1.Determines adjacent unit graphics
    2.Enter 0 or 1
    3.If “1”, the building will have the capability to change graphics depending
    on the buildings adjacent to it. For instance, a single wall will be a
    wall end piece, but if a wall is placed between two other walls, it will
    have a different graphic.
    */

    /// Charging mode
    /// 0   None
    /// These work only when facing the hit angle.
    /// 1   Block
    ///     Activates special graphic when receiving damage and
    ///     not pursuing the attacker.
    ///     While idle, blocking decreases damage taken by 1/3.
    /// 2   Counter Charge
    ///     Activates special graphic when idle and enemy is near.
    ///     While idle, attacks back once on first received hit.
    ///     Enemy must be unit type 70 and have less than 0.2 max range.
    /// 3   Charge
    ///     Activates special graphic when closer than two tiles to the target.
    ///     Deals 2X damage on 1st hit.
    ///     Planned but never implemented ship special attacks.
    ///     Read the design document of Age of Empires II.
    ///     These were supposed to be mutually exclusive technologies.
    /// 4   Ram
    ///     Charge against another ship, losing some hit points yourself.
    /// 5   Greek Fire
    ///     Fry units on ships passing through your sea of fire.
    /// 6   Board
    ///     Attach to another ship, resulting in takeover or sinking.
    int8_t SpecialAbility = 0;
    int16_t DisplayedPierceArmour = 0;

    int16_t SpawningGraphic = -1;
    int16_t UpgradeGraphic = -1;

    bool compareTo(const Creatable &other) const;

protected:
    void serializeObject(void) override;
};
} // namespace unit
} // namespace genie
