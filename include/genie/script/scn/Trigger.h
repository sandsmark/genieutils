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
  int32_t usedVariables; //always = 0x10 (for internal use)
  int32_t amount; //of objects, difficult level
  int32_t resource; //resource type (see constants below)
  int32_t setObject;
  int32_t nextObject;
  int32_t object;
  int32_t sourcePlayer;
  int32_t technology;
  int32_t timer;
  int32_t trigger;
  MapPoint areaFrom;
  MapPoint areaTo;
  int32_t objectGroup;
  int32_t objectType; //Civilian, Military, Building, Other
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
  int32_t usedVariables;
  int32_t aiGoal;
  int32_t amount;
  int32_t resource;
  int32_t diplomacy;
  int32_t setObjects;
  int32_t nextObject;
  int32_t object;
  int32_t sourcePlayer;
  int32_t targetPlayer;
  int32_t technology;
  int32_t stringTableID;
  int32_t soundResourceID;
  int32_t timer;
  int32_t trigger;
  MapPoint location;
  MapPoint areaFrom; //lower-left corner of area
  MapPoint areaTo; //upper-right corner
  int32_t objectGroup;
  int32_t objectType; //Civilian, Military, Building, Other
  int32_t instructionPanel;
  std::string message;	//Instructions/Chat
  std::string soundFile;
  std::vector<int32_t> selectedUnits;

private:
  virtual void serializeObject(void);
};

class Trigger : public ISerializable
{
public:
  Trigger();
  virtual ~Trigger();

  int32_t startingState;
  int8_t looping;
  int32_t stringTableID;
  int8_t isObjective;
  int32_t descriptionOrder;
  int32_t startingTime;
  std::string description;
  std::string name;
  std::vector<TriggerEffect> effects;
  std::vector<int32_t> effectDisplayOrder;
  std::vector<TriggerCondition> conditions;
  std::vector<int32_t> conditionDisplayOrder;

private:
  int32_t numEffects_;
  int32_t numConditions_;
  virtual void serializeObject(void);
};

}

#endif // GENIE_SCN_TRIGGER_H
