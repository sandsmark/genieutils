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
    void setGameVersion(GameVersion gv) override;

    enum EffectType {
        AbsoluteAttributeModifier = 0,
        ResourceModifier = 1,
        EnableUnit = 2,
        UpgradeUnit = 3,
        RelativeAttributeModifier = 4,
        AttributeMultiplier = 5,
        ResourceMultiplier = 6,
        TechCostModifier = 101,
        DisableTech = 102,
        TechTimeModifier = 103
    };

    int8_t Type = -1;


    int16_t TargetUnit = -1;

    // For ResourceModifier, 0 means absolute and 1 means relative
    int16_t UnitClassID = -1;

    /// -1 No Attribute/Invalid Attribute
    /// 0 - Hit Points
    /// 1 - Line of Sight
    /// 2 - Garrison Capacity
    /// 3 - Unit Size X
    /// 4 - Unit Size Y
    /// 5 - Movement Speed (types 20-80)
    /// 6 - Rotation Speed (types 30-80)
    /// 7 - Unused
    /// 8 - Armor (types 50-80)
    /// 9 - Attack (types 50-80)
    /// 10 - Attack Reload Time (types 50-80)
    /// 11 - Accuracy Percent (types 50-80)
    /// 12 - Max Range (types 50-80)
    /// 13 - Work Rate (types 30-80)
    /// 14 - Carry Capacity
    /// 15 - Base Armor (types 50-80)
    /// 16 - Projectile Unit (types 50-80)
    /// 17 - Icon/Graphics Angle (type 80)
    /// 18 - Terrain Defense Bonus (always sets, types 50-80)
    /// 19 - Enable Smart Projectiles (type 60)
    /// 20 - Min Range (types 50-80)
    /// 21 - Amount of 1st resource storage
    /// 22 - Blast Width (types 50-80)
    /// 23 - Search Radius (types 40-80)
    /// 100 - Resource Costs (types 70-80)
    /// 101 - Train Time (types 70-80)
    /// 102 - Total Missiles (types 70-80)
    /// 103 - Food Costs (types 70-80)
    /// 104 - Wood Costs (types 70-80)
    /// 105 - Gold Costs (types 70-80)
    /// 106 - Stone Costs (types 70-80)
    /// 107 - Max Total Missiles (types 70-80)
    /// 108 - Garrison Heal Rate (type 80)
    /// 109 - Regeneration Rate (types 40-80)

    enum Attributes : int16_t {
        InvalidAttribute = -1,
        HitPoints = 0,
        LineOfSight = 1,
        GarrisonCapacity = 2,
        UnitSizeX = 3,
        UnitSizeY = 4,
        MovementSpeed = 5,
        RotationSpeed = 6,
        Armor = 8,
        Attack = 9,
        AttackReloadTime = 10,
        AccuracyPercent = 11,
        MaxRange = 12,
        WorkRate = 13,
        CarryCapacity = 14,
        BaseArmor = 15,
        ProjectileUnit = 16,
        IconGraphicsAngle = 17,
        TerrainDefenseBonus = 18,
        EnableSmartProjectiles = 19,
        MinRange = 20,
        MainResourceStorage = 21,
        BlastWidth = 22,
        SearchRadius = 23,
        ResourceCosts = 100,
        TrainTime = 101,
        TotalMissiles = 102,
        FoodCosts = 103,
        WoodCosts = 104,
        GoldCosts = 105,
        StoneCosts = 106,
        MaxTotalMissiles = 107,
        GarrisonHealRate = 108,
        RegenerationRate = 109
    };

    int16_t AttributeID = -1;
    float Amount = 0;

private:
    void serializeObject(void) override;
};
}

#endif // GENIE_TECHAGEEFFECT_H
