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

  if (isOperation(OP_WRITE)) // Automatic compression.
  {
    usedVariables = 16;
    int32_t *browser = &aiSignal;
    while (usedVariables > 0)
    {
      if (*browser != -1) break;
      --usedVariables;
      --browser;
    }
  }

  int32_t *start = &amount;
  serialize<int32_t>(&start, usedVariables);
}

TriggerEffect::TriggerEffect()
{
  setObjects = -1;
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
    usedVariables = 23;
    int32_t *browser = &instructionPanel;
    while (usedVariables > 0)
    {
      if (*browser != -1) break;
      --usedVariables;
      --browser;
    }
  }

  int32_t *start = &aiGoal;
  serialize<int32_t>(&start, usedVariables);
  serializeSizedString<int32_t>(message);
  serializeSizedString<int32_t>(soundFile);
  if (scn_trigger_ver > 1.1 && usedVariables >= 5 && setObjects > 0)
    serialize<int32_t>(selectedUnits, setObjects);
}

}
