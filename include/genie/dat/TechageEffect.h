/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2016  Mikko "Tapsa" P

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

#ifndef GENIE_TECHAGEEFFECT_H
#define GENIE_TECHAGEEFFECT_H
#include "genie/file/ISerializable.h"

namespace genie {

/// Stores the effect type and its parameters
/// See http://www.digitization.org/wiki/index.php/Genie_technologies#Effects
/// for more details
class EffectCommand : public ISerializable
{
public:
    enum EffectType : int8_t {
        /// Invalid/unset type
        InvalidEffectType = -1,

        /// Sets a unit attribute
        AbsoluteAttributeModifier = 0,

        /// Modifies a resource (gold, wood, population headroom, etc.)
        ResourceModifier = 1,

        /// Enables a disabled unit
        EnableUnit = 2,

        /// Upgrades a unit type to The Next Level™
        UpgradeUnit = 3,

        /// Modifies a unit attribute (plus or minus)
        RelativeAttributeModifier = 4,

        /// Multiplies an attribute (can be <1, i. e. divide)
        AttributeMultiplier = 5,

        /// Multiplies a resource
        ResourceMultiplier = 6,

        /// Modifies the cost of researching a tech
        TechCostModifier = 101,

        /// Disables a technology
        DisableTech = 102,

        /// Modifies how long it takes to research a tech
        TechTimeModifier = 103
    };

    /// What kind of effect, influences how all other values are parsed
    EffectType Type = InvalidEffectType;

    /// If Type is for modifying a Unit:
    /// If this is specific to a specific unit type, this is set, otherwise see UnitClassID
    int16_t TargetUnit = -1;

    /// When applied as EffectType::ResourceModifier,
    /// 0 means absolute and 1 means relative.
    /// Otherwise it is the unit class it affects (instead of a specific unit).
    int16_t UnitClassID = -1;


    enum Attributes : int16_t {
        /// No Attribute/Invalid Attribute
        InvalidAttribute = -1,

        /// Hit Points
        HitPoints = 0,

        /// Line of Sight
        LineOfSight = 1,

        /// Garrison Capacity
        GarrisonCapacity = 2,

        /// Unit Size X
        UnitSizeX = 3,

        /// Unit Size Y
        UnitSizeY = 4,

        /// Movement Speed (types 20-80)
        MovementSpeed = 5,

        /// Rotation Speed (types 30-80)
        RotationSpeed = 6,

        /// Not used by the game
        Unused = 7,

        /// Armor (types 50-80)
        Armor = 8,

        /// Attack (types 50-80)
        Attack = 9,

        /// Attack Reload Time (types 50-80)
        AttackReloadTime = 10,

        /// Accuracy Percent (types 50-80)
        AccuracyPercent = 11,

        /// Max Range, mostly attacks (types 50-80)
        MaxRange = 12,

        /// Work Rate, how much is gathered per second etc. (types 30-80)
        WorkRate = 13,

        /// Carry Capacity
        CarryCapacity = 14,

        /// Base Armor, i. e. not attack type specific (types 50-80)
        BaseArmor = 15,

        /// Projectile Unit, i. e. what kind of projectiles it fires (types 50-80)
        ProjectileUnit = 16,

        /// Icon/Graphics Angle, don't ask me (type 80)
        IconGraphicsAngle = 17,

        /// Terrain Defense Bonus (always sets, types 50-80)
        TerrainDefenseBonus = 18,

        /// Enable Smart Projectiles (type 60)
        EnableSmartProjectiles = 19,

        /// Min Range (types 50-80)
        MinRange = 20,

        /// Amount of 1st resource storage
        MainResourceStorage = 21,

        /// Blast Width (types 50-80)
        BlastWidth = 22,

        /// Search Radius, for "automatic" actions (types 40-80)
        SearchRadius = 23,

        /// Resource Costs (types 70-80)
        ResourceCosts = 100,

        /// How long it takes to train the unit (types 70-80)
        TrainTime = 101,

        /// Total Missiles (types 70-80)
        TotalMissiles = 102,

        /// Food Costs (types 70-80)
        FoodCosts = 103,

        /// Wood Costs (types 70-80)
        WoodCosts = 104,

        /// Gold Costs (types 70-80)
        GoldCosts = 105,

        /// Stone Costs (types 70-80)
        StoneCosts = 106,

        /// Max Total Missiles (types 70-80)
        MaxTotalMissiles = 107,

        /// Garrison Heal Rate (type 80)
        GarrisonHealRate = 108,

        /// Regeneration Rate (types 40-80)
        RegenerationRate = 109
    };

    /// Defines what attributes this effect affects (~pun intended.)
    Attributes AttributeID = InvalidAttribute;

    /// Absolute or relative if it is a EffectType::ResourceModifier, depending on UnitClassID
    /// Otherwise it depends on the EffectType (multiplier, relativemodifier, etc.)
    float Amount = 0;

    bool compareTo(const EffectCommand &other) const;

private:
    void serializeObject(void) override;
};
} // namespace genie

#endif // GENIE_TECHAGEEFFECT_H
