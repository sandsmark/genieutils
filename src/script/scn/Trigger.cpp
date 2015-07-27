/*
    genieutils - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2015  Mikko "Tapsa" P
    Copyright (C) 2015  JustTesting1234

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

#include "genie/script/scn/Trigger.h"

#include <fstream>

namespace genie
{

Trigger::Trigger()
{
}

Trigger::~Trigger()
{
}

void Trigger::serializeObject(void)
{
  serialize<double>(triggerVersion);
  serialize<int8_t>(unknown);
  serializeSize<int32_t>(numTriggers_, triggers.size());
  serializeSub<TriggerSub>(triggers, numTriggers_);
  serialize<int32_t>(triggerDisplayOrder, numTriggers_);
}

TriggerSub::TriggerSub()
{
}

TriggerSub::~TriggerSub()
{
}

void TriggerSub::serializeObject(void)
{
  serialize<uint32_t>(enabled);
  serialize<uint32_t>(looping);
  serialize<uint8_t>(unknown1);
  serialize<uint8_t>(isObjective);
  serialize<uint32_t>(objectiveDescriptionOrder);
  serialize<uint32_t>(unknown2);
  serializeSizedString<uint32_t>(triggerDescription);
  serializeSizedString<uint32_t>(triggerName);
  serializeSize<int32_t>(numEffects_, effects.size());
  serializeSub<TriggerEffect>(effects, numEffects_);
  serialize<int32_t>(effectDisplayOrder, numEffects_);
  serializeSize<int32_t>(numConditions_, conditions.size());
  serializeSub<TriggerCondition>(conditions, numConditions_);
  serialize<int32_t>(conditionDisplayOrder, numConditions_);
}

TriggerCondition::TriggerCondition()
{
}

TriggerCondition::~TriggerCondition()
{
}

void TriggerCondition::serializeObject(void)
{
  serialize<int32_t>(type);
  serialize<int32_t>(check);
  serialize<int32_t>(amount);
  serialize<int32_t>(resource);
  serialize<int32_t>(unitObject);
  serialize<int32_t>(unitLocation);
  serialize<int32_t>(unitType);
  serialize<int32_t>(player);
  serialize<int32_t>(technology);
  serialize<int32_t>(timer);
  serialize<int32_t>(unknown);
  serialize<int32_t>(areaLL.x);
  serialize<int32_t>(areaLL.y);
  serialize<int32_t>(areaUR.x);
  serialize<int32_t>(areaUR.y);
  serialize<int32_t>(unitGroup);
  serialize<int32_t>(unitClass);
  serialize<int32_t>(aiSignal);
}

TriggerEffect::TriggerEffect()
{
}

TriggerEffect::~TriggerEffect()
{
}

void TriggerEffect::serializeObject(void)
{
  serialize<int32_t>(type);
  serialize<int32_t>(check);
  serialize<int32_t>(aiGoal);
  serialize<int32_t>(amount);
  serialize<int32_t>(resource);
  serialize<int32_t>(diplomacy);
  serializeSize<int32_t>(numSelected_, selectedUnits.size());
  serialize<int32_t>(locationUnit);
  serialize<int32_t>(unitType);
  serialize<int32_t>(playerSource);
  serialize<int32_t>(playerTarget);
  serialize<int32_t>(technology);
  serialize<int32_t>(stringID);
  serialize<int32_t>(unknown);
  serialize<int32_t>(displayTime);
  serialize<int32_t>(triggerID);
  serialize<int32_t>(location.x);
  serialize<int32_t>(location.y);
  serialize<int32_t>(areaLL.x);
  serialize<int32_t>(areaLL.y);
  serialize<int32_t>(areaUR.x);
  serialize<int32_t>(areaUR.y);
  serialize<int32_t>(unitGroup);
  serialize<int32_t>(unitClass);
  serialize<int32_t>(instructionPanel);
  serializeSizedString<uint32_t>(text);
  serializeSizedString<uint32_t>(soundFile);
  serialize<int32_t>(selectedUnits, numSelected_);
}

}
