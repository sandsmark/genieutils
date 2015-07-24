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

#ifndef GENIE_SCN_TRIGGER_H
#define GENIE_SCN_TRIGGER_H

#include "genie/file/ISerializable.h"

#include <stdint.h>

namespace genie
{

struct MapPoint
{
    int32_t x, y;
};

class TriggerCondition : public ISerializable
{
public:
  TriggerCondition();
  virtual ~TriggerCondition();

  int32_t type;
  int32_t check; //always = 0x10 (for internal use)
  int32_t amount; //of objects, difficult level
  int32_t resource; //resource type (see constants below)
  int32_t unitObject;
  int32_t unitLocation;
  int32_t unitType;
  int32_t player;
  int32_t technology;
  int32_t timer;
  int32_t unknown;
  MapPoint areaLL;
  MapPoint areaUR;
  int32_t unitGroup;
  int32_t unitClass; //Civilian, Military, Building, Other
  int32_t aiSignal;

private:
  virtual void serializeObject(void);
};

class TriggerEffect : public ISerializable
{
public:
  TriggerEffect();
  virtual ~TriggerEffect();

  int32_t type;
  int32_t check;
  int32_t aiGoal;
  int32_t amount;
  int32_t resource;
  int32_t diplomacy;
  int32_t locationUnit;
  int32_t unitType;
  int32_t playerSource;
  int32_t playerTarget;
  int32_t technology;
  int32_t stringID;
  int32_t unknown;
  int32_t displayTime;
  int32_t triggerID;
  MapPoint location;
  MapPoint areaLL; //lower-left corner of area
  MapPoint areaUR; //upper-right corner
  int32_t unitGroup;
  int32_t unitClass; //Civilian, Military, Building, Other
  int32_t instructionPanel;
  std::string text;	//Instructions/Chat
  std::string soundFile;
  std::vector<int32_t> selectedUnits;

private:
  int32_t numSelected_;
  virtual void serializeObject(void);
};

class TriggerSub : public ISerializable
{
public:
  TriggerSub();
  virtual ~TriggerSub();

  uint32_t enabled;
  uint32_t looping;
  uint8_t unknown1;
  uint8_t isObjective;
  uint32_t objectiveDescriptionOrder;
  uint32_t unknown2;
  std::string triggerDescription;
  std::string triggerName;
  std::vector<TriggerEffect> effects;
  std::vector<int32_t> effectDisplayOrder;
  std::vector<TriggerCondition> conditions;
  std::vector<int32_t> conditionDisplayOrder;

private:
  int32_t numEffects_;
  int32_t numConditions_;
  virtual void serializeObject(void);
};

class Trigger : public ISerializable
{
public:
  Trigger();
  virtual ~Trigger();

  int8_t unknown;
  std::vector<TriggerSub> triggers;
  std::vector<int32_t> triggerDisplayOrder;

private:
  int32_t numTriggers_;
  virtual void serializeObject(void);
};

}

#endif // GENIE_SCN_TRIGGER_H
