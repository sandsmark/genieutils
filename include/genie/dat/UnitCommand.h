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

#ifndef GENIE_UNITCOMMAND_H
#define GENIE_UNITCOMMAND_H
#include "genie/file/ISerializable.h"

#include "ActionType.h"

namespace genie {

class Task : public ISerializable
{
public:
    /// Always 1?
    int16_t TaskType = 1;

    /// Unique ID for this Task
    int16_t ID = -1;

    /// Basically useless
    /// Used to setup task when discovered,
    /// but you can directly set it in unit data
    int8_t IsDefault = false;

    /// Type of action
    genie::ActionType ActionType = ActionType::None;

    static const std::string &actionTypeName(const int16_t type);
    const std::string &actionTypeName() const { return actionTypeName(int(ActionType)); }

    /// Unit class this applies to
    int16_t ClassID = -1;

    /// Unit ID this applies to
    int16_t UnitID = -1;

    /// Terrain ID this applies to
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
    int8_t CombatLevelFlag = 0;

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
    TargetDiplomacies TargetDiplomacy = TargetAnyDiplo;

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

    /// Wwise sound to play when gathering, DE only
    uint32_t WwiseResourceGatheringSoundID = 0; // TODO doc, new in de

    /// Wwise sound to play when depositing, DE only
    uint32_t WwiseResourceDepositSoundID = 0; // TODO doc, new in de

    bool compareTo(const Task &other) const;

private:
    void serializeObject(void) override;
};
} // namespace genie

#endif // GENIE_UNITCOMMAND_H
