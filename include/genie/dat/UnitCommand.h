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

#ifndef GENIE_UNITCOMMAND_H
#define GENIE_UNITCOMMAND_H
#include "genie/file/ISerializable.h"

namespace genie {

class Task : public ISerializable
{
public:
    Task();
    virtual ~Task();
    virtual void setGameVersion(GameVersion gv);

    // Always 1?
    int16_t TaskType = 1;

    int16_t ID = -1;

    /// Basically useless
    /// Used to setup task when discovered,
    /// but you can directly set it in unit data
    int8_t IsDefault = false;

    /// What type of command/action is it
    /// From AGE
    enum ActionTypes : int16_t {
        None = 0, // not set for anything

        /// In AoC
        Garrison = 3,
        GatherRebuild = 5, // villagers, fishing ship
        Graze = 6,
        Combat = 7,
        Fly = 10,
        ScareHunt = 11, // Predatory animals
        UnloadBoat = 12, // longboat, elite longboat, transport ship
        Guard = 13,
        SetInitialResources = 21, // farms and fish traps
        Build = 101,
        Convert = 104,
        Heal = 105,
        Repair = 106,
        GetAutoConverted = 107, // sheep, turkey, monument, relic cart
        RetreatToShootingRage = 109, // automatically run away when attacked or something? doesn't set any parameters except autosearch and search wait time
        Hunt = 110,
        Trade = 111,
        GenerateWonderVictory = 120,
        FarmDeselectWhenBuilt = 121, // farms and fish traps
        Loot = 122, // villager, scouts, light cavalry
        UnpackAndAttack = 125, // trebuchets
        OffMapTrade1 = 131, // trade carts
        PickupRelic = 132, // monks without relics
        KidnapUnit = 135, // scout/light cavalry
        DepositRelic = 136, // monks with relics

        /// Not in AoC
        MoveTo = 1,
        Follow = 2,
        Explore = 4,
        Shoot = 8,
        Attack = 9,
        Escape = 20,
        Num = 100,
        MakeUnit = 102,
        MakeTech = 103,
        DiscoveryArtifact = 108,
        Housing = 123,
        Pack = 124,
        OffMapTrade0 = 130,
        ChargeAttack = 133,
        TransformUnit = 134,
        Shear = 149,

        /// Only in HD, I guess
        HDSiegeTowerAbility = 14,
        HDRegeneration = 150,
        HDFeitoria = 151,

        Stop = 10000, // not used in aoc
        AboutFace = 10001, // not used in aoc
        Wheel = 10002, // not used in aoc
    };
    int16_t ActionType = 0;
    static const std::string &actionTypeName(const int16_t type);
    const std::string &actionTypeName() const { return actionTypeName(ActionType); }

    int16_t ClassID = -1;
    int16_t UnitID = -1;
    int16_t TerrainID = -1;

    /// Resource gained by gathering
    int16_t ResourceIn = -1;

    /// Prod resource
    /// Resource that multiplies the amount you gather
    int16_t ResourceMultiplier = -1;

    /// Resource deposited on drop site
    int16_t ResourceOut = -1;

    int16_t UnusedResource = -1;

    /// Work rate modifier
    /// Minimum conversion time
    float WorkValue1 = 0;

    /// Maximum conversion time
    float WorkValue2 = 0;

    float WorkRange = 0;

    /// If 1, then auto-search for targets
    int8_t AutoSearchTargets = 0;

    /// Unused
    float SearchWaitTime = 0;

    /// AoE alphas: Target choosing based on combat level
    /// 0   No targeting
    /// 1   Allows units to select their targets
    int8_t EnableTargeting = 0;

    /// Unused Flag
    int8_t CombatLevelFlag = false;

    /// Work Flag 1
    /// AoE 1: Used when farm is dead but still exists
    /// 0   Plunder from resource
    /// 1   Plunder from players
    /// 2   Raider thing?
    int16_t GatherType = 0;

    /// Unused
    int16_t WorkFlag2 = 0;

    /// Owner Type
    /// Determines what you can select as targets
    /// > 7 is same as 0 and 7
    enum TargetDiplomacies : int8_t {
        TargetAnyDiplo = 0, ///< All objects
        TargetSelf = 1, ///< Your objects only
        TargetNeutralsEnemies = 2, ///< Neutral and enemy objects only
        TargetGaiaOnly = 3, ///< Gaia only
        TargetSelfAllyGaia = 4, ///< Gaia, your and ally objects only
        TargetGaiaNeutralEnemies = 5, ///< Gaia, neutral and enemy objects only
        TargetOthers = 6, ///< All but your objects
        TargetAnyDiplo2 = 7, ///< All objects
    };
    int8_t TargetDiplomacy = 0;

    /// Holding Attribute
    /// 0   Right-click target is defined by the target diplomacy.
    /// 1   Preceding the above, checks if target has more than 0 resource.
    int8_t CarryCheck = false;

    /// 1   Choose this task if targeting a construction
    int8_t PickForConstruction = false;

    /// walking with tool but no resource
    int16_t MovingGraphicID = -1;

    /// proceeding resource gathering or attack
    int16_t ProceedingGraphicID = -1;

    /// Used when actually gathering a resource or attacking/converting
    int16_t WorkingGraphicID = -1;

    /// Used when carrying a resource
    /// display resources in hands
    int16_t CarryingGraphicID = -1;

    /// Example: Plays when lumberjack starts chopping wood
    int16_t ResourceGatheringSoundID = -1;

    /// Example: Plays when lumberjack drops his wood into TC
    int16_t ResourceDepositSoundID = -1;

private:
    void serializeObject(void) override;
};
}

#endif // GENIE_UNITCOMMAND_H
