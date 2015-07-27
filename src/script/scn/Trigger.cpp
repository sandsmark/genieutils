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
  triggerVersion = scn_trigger_ver;
  serialize<double>(triggerVersion);
  scn_trigger_ver = triggerVersion;

  if (scn_trigger_ver > 1.4)
    serialize<int8_t>(objectivesStartingState);
  serializeSize<int32_t>(numTriggers_, triggers.size());
  serializeSub<TriggerSub>(triggers, numTriggers_);
  if (scn_trigger_ver > 1.3)
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
  serialize<int32_t>(startingState);
  serialize<int8_t>(looping);
  serialize<int32_t>(stringTableID);
  serialize<int8_t>(isObjective);
  serialize<int32_t>(descriptionOrder);
  if (scn_trigger_ver > 1.5)
    serialize<int32_t>(startingTime);
  serializeSizedString<int32_t>(description);
  serializeSizedString<int32_t>(name);
  serializeSize<int32_t>(numEffects_, effects.size());
  serializeSub<TriggerEffect>(effects, numEffects_);
  if (scn_trigger_ver > 1.2)
    serialize<int32_t>(effectDisplayOrder, numEffects_);
  serializeSize<int32_t>(numConditions_, conditions.size());
  serializeSub<TriggerCondition>(conditions, numConditions_);
  if (scn_trigger_ver > 1.2)
    serialize<int32_t>(conditionDisplayOrder, numConditions_);
}

TriggerCondition::TriggerCondition()
{
  type = 0;
  usedVariables = 0;
  amount = -1;
  resource = -1;
  setObject = -1;
  nextObject = -1;
  object = -1;
  sourcePlayer = -1;
  technology = -1;
  timer = -1;
  trigger = -1;
  areaFrom.x = -1;
  areaFrom.y = -1;
  areaTo.x = -1;
  areaTo.y = -1;
  objectGroup = -1;
  objectType = -1;
  aiSignal = -1;
}

TriggerCondition::~TriggerCondition()
{
}

void TriggerCondition::serializeObject(void)
{
  serialize<int32_t>(type);
  if (scn_trigger_ver > 1.0)
    serialize<int32_t>(usedVariables);
  else usedVariables = 13;

  if (isOperation(OP_WRITE)) // Automatic compression. Change this to void pointer checking values.
  {
    usedVariables = 0;
    if (aiSignal != -1) usedVariables = 16;
    else if (objectType != -1) usedVariables = 15;
    else if (objectGroup != -1) usedVariables = 14;
    else if (areaTo.y != -1) usedVariables = 13;
    else if (areaFrom.y != -1) usedVariables = 11;
    else if (trigger != -1) usedVariables = 9;
    else if (timer != -1) usedVariables = 8;
    else if (technology != -1) usedVariables = 7;
    else if (sourcePlayer != -1) usedVariables = 6;
    else if (object != -1) usedVariables = 5;
    else if (nextObject != -1) usedVariables = 4;
    else if (setObject != -1) usedVariables = 3;
    else if (resource != -1) usedVariables = 2;
    else if (amount != -1) usedVariables = 1;
  }

  switch (usedVariables) // Wrong! Make new function into ISerializable
  {
  case 16: serialize<int32_t>(aiSignal);
  case 15: serialize<int32_t>(objectType);
  case 14: serialize<int32_t>(objectGroup);
  case 13: serialize<int32_t>(areaTo.y);
  serialize<int32_t>(areaTo.x);
  case 11: serialize<int32_t>(areaFrom.y);
  serialize<int32_t>(areaFrom.x);
  case 9: serialize<int32_t>(trigger);
  case 8: serialize<int32_t>(timer);
  case 7: serialize<int32_t>(technology);
  case 6: serialize<int32_t>(sourcePlayer);
  case 5: serialize<int32_t>(object);
  case 4: serialize<int32_t>(nextObject);
  case 3: serialize<int32_t>(setObject);
  case 2: serialize<int32_t>(resource);
  case 1: serialize<int32_t>(amount);
  }
}

TriggerEffect::TriggerEffect()
{
  setObjects_ = -1;
  type = 0;
  usedVariables = 0;
  aiGoal = -1;
  amount = -1;
  resource = -1;
  diplomacy = -1;
  nextObject = -1;
  object = -1;
  sourcePlayer = -1;
  targetPlayer = -1;
  technology = -1;
  stringTableID = -1;
  soundResourceID = -1;
  timer = -1;
  trigger = -1;
  location.x = -1;
  location.y = -1;
  areaFrom.x = -1;
  areaFrom.y = -1;
  areaTo.x = -1;
  areaTo.y = -1;
  objectGroup = -1;
  objectType = -1;
  instructionPanel = -1;
  message = "";
  soundFile = "";
}

TriggerEffect::~TriggerEffect()
{
}

void TriggerEffect::serializeObject(void)
{
  serialize<int32_t>(type);
  if (scn_trigger_ver > 1.0)
    serialize<int32_t>(usedVariables);
  else usedVariables = 16;

  if (isOperation(OP_WRITE)) // Automatic compression.
  {
    usedVariables = 0;
    if (instructionPanel != -1) usedVariables = 23;
    else if (objectType != -1) usedVariables = 22;
    else if (objectGroup != -1) usedVariables = 21;
    else if (areaTo.y != -1) usedVariables = 20;
    else if (areaFrom.y != -1) usedVariables = 18;
    else if (location.y != -1) usedVariables = 16;
    else if (trigger != -1) usedVariables = 14;
    else if (timer != -1) usedVariables = 13;
    else if (soundResourceID != -1) usedVariables = 12;
    else if (stringTableID != -1) usedVariables = 11;
    else if (technology != -1) usedVariables = 10;
    else if (targetPlayer != -1) usedVariables = 9;
    else if (sourcePlayer != -1) usedVariables = 8;
    else if (object != -1) usedVariables = 7;
    else if (nextObject != -1) usedVariables = 6;
    else if (setObjects_ > 0) usedVariables = 5;
    else if (diplomacy != -1) usedVariables = 4;
    else if (resource != -1) usedVariables = 3;
    else if (amount != -1) usedVariables = 2;
    else if (aiGoal != -1) usedVariables = 1;
  }

  if (usedVariables < 1) goto TE_END;
  serialize<int32_t>(aiGoal);
  if (usedVariables < 2) goto TE_END;
  serialize<int32_t>(amount);
  if (usedVariables < 3) goto TE_END;
  serialize<int32_t>(resource);
  if (usedVariables < 4) goto TE_END;
  serialize<int32_t>(diplomacy);
  if (usedVariables < 5) goto TE_END;
  serializeSize<int32_t>(setObjects_, selectedUnits.size()); // -1 is considered 0.
  if (usedVariables < 6) goto TE_END;
  serialize<int32_t>(nextObject);
  if (usedVariables < 7) goto TE_END;
  serialize<int32_t>(object);
  if (usedVariables < 8) goto TE_END;
  serialize<int32_t>(sourcePlayer);
  if (usedVariables < 9) goto TE_END;
  serialize<int32_t>(targetPlayer);
  if (usedVariables < 10) goto TE_END;
  serialize<int32_t>(technology);
  if (usedVariables < 11) goto TE_END;
  serialize<int32_t>(stringTableID);
  if (usedVariables < 12) goto TE_END;
  serialize<int32_t>(soundResourceID);
  if (usedVariables < 13) goto TE_END;
  serialize<int32_t>(timer);
  if (usedVariables < 14) goto TE_END;
  serialize<int32_t>(trigger);
  if (usedVariables < 15) goto TE_END;
  serialize<int32_t>(location.x);
  if (usedVariables < 16) goto TE_END;
  serialize<int32_t>(location.y);
  if (usedVariables < 17) goto TE_END;
  serialize<int32_t>(areaFrom.x);
  if (usedVariables < 18) goto TE_END;
  serialize<int32_t>(areaFrom.y);
  if (usedVariables < 19) goto TE_END;
  serialize<int32_t>(areaTo.x);
  if (usedVariables < 20) goto TE_END;
  serialize<int32_t>(areaTo.y);
  if (usedVariables < 21) goto TE_END;
  serialize<int32_t>(objectGroup);
  if (usedVariables < 22) goto TE_END;
  serialize<int32_t>(objectType);
  if (usedVariables < 23) goto TE_END;
  serialize<int32_t>(instructionPanel);
TE_END:
  serializeSizedString<int32_t>(message);
  serializeSizedString<int32_t>(soundFile);
  if (scn_trigger_ver > 1.1 && usedVariables >= 5 && setObjects_ > 0)
    serialize<int32_t>(selectedUnits, setObjects_);
}

}
