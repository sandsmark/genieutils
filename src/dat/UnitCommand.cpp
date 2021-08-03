/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
<<<<<<< HEAD
    Copyright (C) 2011 - 2019  Mikko "Tapsa" P
=======
    Copyright (C) 2011 - 2021  Mikko "Tapsa" P
>>>>>>> 65dd660 (More accurate signedness.)

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

#include "genie/dat/UnitCommand.h"

#include "TestHelpers.h"

#include <unordered_map>

namespace genie {

const std::string &Task::actionTypeName(const int16_t type)
{
    static const std::unordered_map<int16_t, std::string> knownNames = {
        { 0, "None" },
        { 1, "Move to" },
        { 2, "Follow" },
        { 3, "Garrison" },
        { 4, "Explore" },
        { 5, "Gather/Rebuild" },
        { 6, "Graze" },
        { 7, "Combat" },
        { 8, "Shoot" },
        { 9, "Attack" },
        { 10, "Fly" },
        { 11, "Scare/Hunt" },
        { 12, "Unload (Boat-Like)" },
        { 13, "Guard" },
        { 14, "HD Siege Tower Ability" },
        { 20, "Escape, deleted?" },
        { 21, "Make" },
        { 100, "Num" },
        { 101, "Build" },
        { 102, "Make a Unit" },
        { 103, "Make a Tech" },
        { 104, "Convert" },
        { 105, "Heal" },
        { 106, "Repair" },
        { 107, "Get Auto-converted" },
        { 108, "Discovery Artifact" },
        { 109, "Retreat to attack range" },
        { 110, "Hunt" },
        { 111, "Trade" },
        { 120, "Generate Wonder Victory" },
        { 121, "Deselect when Tasked (Farm)" },
        { 122, "Loot" },
        { 123, "Housing" },
        { 124, "Pack" },
        { 125, "Unpack & Attack" },
        { 130, "Unknown, offmaptrade 2?" },
        { 131, "Off-Map Trade" },
        { 132, "Pickup Unit" },
        { 133, "Charge Attack" },
        { 134, "Transform Unit" },
        { 135, "Kidnap Unit" },
        { 136, "Deposit Unit" },
        { 149, "Shear" },
        { 150, "HD Regeneration" },
        { 151, "HD Feitoria Ability" },
        { 10000, "Stop" },
        { 10001, "About face" },
        { 10002, "Wheel" },
    };

    if (knownNames.find(type) == knownNames.end()) {
        static const std::string unknownString("Unknown class");
        return unknownString;
    }

    return knownNames.at(type);
}

bool Task::compareTo(const Task &other) const
{
    COMPARE_MEMBER(SearchWaitTime);
    COMPARE_MEMBER(WorkRange);
    COMPARE_MEMBER(WorkValue1);
    COMPARE_MEMBER(WorkValue2);
    COMPARE_MEMBER_ENUM_CLASS(ActionType);
    COMPARE_MEMBER(CarryingGraphicID);
    COMPARE_MEMBER(ClassID);
    COMPARE_MEMBER(GatherType);
    COMPARE_MEMBER(ID);
    COMPARE_MEMBER(MovingGraphicID);
    COMPARE_MEMBER(ProceedingGraphicID);
    COMPARE_MEMBER(ResourceDepositSoundID);
    COMPARE_MEMBER(ResourceGatheringSoundID);
    COMPARE_MEMBER(ResourceIn);
    COMPARE_MEMBER(ResourceMultiplier);
    COMPARE_MEMBER(ResourceOut);
    COMPARE_MEMBER(TaskType);
    COMPARE_MEMBER(TerrainID);
    COMPARE_MEMBER(UnitID);
    COMPARE_MEMBER(UnusedResource);
    COMPARE_MEMBER(WorkFlag2);
    COMPARE_MEMBER(WorkingGraphicID);
    COMPARE_MEMBER(AutoSearchTargets);
    COMPARE_MEMBER(CarryCheck);
    COMPARE_MEMBER(CombatLevelFlag);
    COMPARE_MEMBER(EnableTargeting);
    COMPARE_MEMBER(IsDefault);
    COMPARE_MEMBER(PickForConstruction);
    COMPARE_MEMBER(TargetDiplomacy);
    COMPARE_MEMBER(WwiseResourceDepositSoundID);
    COMPARE_MEMBER(WwiseResourceGatheringSoundID);

    return true;
}

//------------------------------------------------------------------------------
void Task::serializeObject()
{
    GameVersion gv = getGameVersion();

<<<<<<< HEAD
    serialize<int16_t>(TaskType);
    serialize<int16_t>(ID);
    serialize<int8_t>(IsDefault);
    serialize<int16_t>(ActionType);
    serialize<int16_t>(ClassID);
    serialize<int16_t>(UnitID);
    serialize<int16_t>(TerrainID);
    serialize<int16_t>(ResourceIn);
    serialize<int16_t>(ResourceMultiplier);
    serialize<int16_t>(ResourceOut);
    serialize<int16_t>(UnusedResource);
    serialize<float>(WorkValue1);
    serialize<float>(WorkValue2);
    serialize<float>(WorkRange);
    serialize<int8_t>(AutoSearchTargets);
    serialize<float>(SearchWaitTime);
    serialize<int8_t>(EnableTargeting);
    serialize<int8_t>(CombatLevelFlag);
    serialize<int16_t>(GatherType);
    serialize<int16_t>(WorkFlag2);
    serialize<int8_t>(TargetDiplomacy);
    serialize<int8_t>(CarryCheck);
    serialize<int8_t>(PickForConstruction);
    serialize<int16_t>(MovingGraphicID);
    serialize<int16_t>(ProceedingGraphicID);
    serialize<int16_t>(WorkingGraphicID);
    serialize<int16_t>(CarryingGraphicID);
    serialize<int16_t>(ResourceGatheringSoundID);
    serialize<int16_t>(ResourceDepositSoundID);
=======
  serialize<int16_t>(TaskType);
  serialize<int16_t>(ID);
  serialize<uint8_t>(IsDefault);
  serialize<int16_t>(ActionType);
  serialize<int16_t>(ClassID);
  serialize<int16_t>(UnitID);
  serialize<int16_t>(TerrainID);
  serialize<int16_t>(ResourceIn);
  serialize<int16_t>(ResourceMultiplier);
  serialize<int16_t>(ResourceOut);
  serialize<int16_t>(UnusedResource);
  serialize<float>(WorkValue1);
  serialize<float>(WorkValue2);
  serialize<float>(WorkRange);
  serialize<uint8_t>(AutoSearchTargets);
  serialize<float>(SearchWaitTime);
  serialize<uint8_t>(EnableTargeting);
  serialize<uint8_t>(CombatLevelFlag);
  serialize<int16_t>(GatherType);
  serialize<int16_t>(WorkFlag2);
  serialize<uint8_t>(TargetDiplomacy);
  serialize<uint8_t>(CarryCheck);
  serialize<uint8_t>(PickForConstruction);
  serialize<int16_t>(MovingGraphicID);
  serialize<int16_t>(ProceedingGraphicID);
  serialize<int16_t>(WorkingGraphicID);
  serialize<int16_t>(CarryingGraphicID);
  serialize<int16_t>(ResourceGatheringSoundID);
  serialize<int16_t>(ResourceDepositSoundID);
  if (gv >= GV_C4 && gv <= GV_LatestDE2)
  {
    serialize<uint32_t>(WwiseResourceGatheringSoundID);
    serialize<uint32_t>(WwiseResourceDepositSoundID);
  }
}
>>>>>>> 65dd660 (More accurate signedness.)

    if (gv >= GV_C4 && gv <= GV_LatestDE2) {
        serialize<uint32_t>(WwiseResourceGatheringSoundID);
        serialize<uint32_t>(WwiseResourceDepositSoundID);
    }
}
} // namespace genie
