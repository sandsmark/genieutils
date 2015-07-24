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

  

private:
  virtual void serializeObject(void);
};

class TriggerEffect : public ISerializable
{
public:
  TriggerEffect();
  virtual ~TriggerEffect();

  

private:
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
