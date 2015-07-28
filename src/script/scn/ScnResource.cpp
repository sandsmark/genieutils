/*
    genieutils - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
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

#include "genie/script/scn/ScnResource.h"

#include <fstream>

namespace genie
{

ScnPlayerResources::ScnPlayerResources()
{
  ore = 0;
  goods = 0;
  popLimit = 75;
}

ScnPlayerResources::~ScnPlayerResources()
{
}

void ScnPlayerResources::serializeObject(void)
{
  serialize<float>(food);
  serialize<float>(wood);
  serialize<float>(gold);
  serialize<float>(stone);
  if (scn_internal_ver > 1.12)
  {
    serialize<float>(ore);
    if (scn_internal_ver < 1.3)
      serialize<float>(goods);
  }
  if (scn_internal_ver > 1.13)
    serialize<float>(popLimit); // game forces range from 25 to 200, defaults to 75
}

ScnPlayerUnits::ScnPlayerUnits()
{
}

ScnPlayerUnits::~ScnPlayerUnits()
{
}

void ScnPlayerUnits::serializeObject(void)
{
  serializeSize<uint32_t>(unitCount_, units.size());
  serializeSub<ScnUnit>(units, unitCount_);
}

ScnUnit::ScnUnit()
{
}

ScnUnit::~ScnUnit()
{
}

void ScnUnit::serializeObject(void)
{
  serialize<float>(positionX);
  serialize<float>(positionY);
  serialize<float>(positionZ);
  serialize<uint32_t>(spawnID);
  serialize<uint16_t>(objectID); // units with hardcoded behaviour 102, 66, 59, 768, 420, 770, 691
  serialize<uint8_t>(state);
  serialize<float>(rotation);
  if (scn_ver != "1.14")
    serialize<uint16_t>(initAnimationFrame);
  serialize<uint32_t>(garrisonedInID);
}

}
