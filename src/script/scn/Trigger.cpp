/*
    genieutils - <description>
    Copyright (C) 2015  Mikko "Tapsa" P <email>

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

TriggerEffect::TriggerEffect()
{
}

TriggerEffect::~TriggerEffect()
{
}

void TriggerEffect::serializeObject(void)
{
}

TriggerCondition::TriggerCondition()
{
}

TriggerCondition::~TriggerCondition()
{
}

void TriggerCondition::serializeObject(void)
{
}

}
