/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml <email>
    Copyright (C) 2011 - 2014  Mikko T P

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

#include "genie/dat/unit/Creatable.h"

namespace genie
{

namespace unit
{

Creatable::Creatable() : ResourceCosts(), MissileSpawningArea()
{
  TrainTime = 0;
  TrainLocationID = -1;
  ButtonID = 0;
  Unknown26 = 0;
  Unknown27 = 0;
  MissileGraphicDelay = 0;
  HeroMode = 0;
  GarrisonGraphic = -1;
  DuplicatedMissilesMin = 1;
  DuplicatedMissilesMax = 1;
  AlternativeProjectileUnit = -1;
  ChargingGraphic = -1;
  ChargingMode = 0;
  DisplayedPierceArmour = 0;
}

Creatable::~Creatable()
{
}

//------------------------------------------------------------------------------
void Creatable::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

short Creatable::getResourceCostsSize()
{
  return 3;
}

void Creatable::serializeObject(void)
{
  serializeSub<ResourceCost, RESOURCECOSTS_SIZE>(ResourceCosts);
  serialize<int16_t>(TrainTime);
  serialize<int16_t>(TrainLocationID);
  serialize<int8_t>(ButtonID);

  if (getGameVersion() >= genie::GV_AoKA) // 9.07
  {
    serialize<int32_t>(Unknown26);
    serialize<int32_t>(Unknown27);
    serialize<int8_t>(MissileGraphicDelay);

    if (getGameVersion() >= genie::GV_AoKB)
    {
      serialize<int8_t>(HeroMode); // 10.49
      serialize<int32_t>(GarrisonGraphic); // 10.73
    }

    serialize<float>(DuplicatedMissilesMin);
    serialize<int8_t>(DuplicatedMissilesMax);
    serialize<float, AMDBUNKNOWN_SIZE>(MissileSpawningArea);
    serialize<int32_t>(AlternativeProjectileUnit); // 9.08
    // 9.2
    {
      serialize<int32_t>(ChargingGraphic);
      serialize<int8_t>(ChargingMode);
    }
  }

  serialize<int16_t>(DisplayedPierceArmour); // 7.01
}

}

}
