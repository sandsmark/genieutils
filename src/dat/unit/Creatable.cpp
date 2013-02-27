/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2013  Armin Preiml <email>

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

Creatable::Creatable() : ResourceCosts(getResourceCostsSize()),
                         Unknown27(UNKNOWN27_SIZE),
                         AttackMissileDuplicationSpawning(AMDBUNKNOWN_SIZE)
{
  TrainTime = 0;
  TrainLocationID = -1;
  ButtonID = 0;
  Unknown26 = 0;
  Unknown28 = 0;
  MissileGraphicDelay = 0;
  HeroMode = 0;
  GarrisonGraphic.first = -1;
  GarrisonGraphic.second = -1;
  AttackMissileDuplicationAmount1 = 1;
  AttackMissileDuplicationAmount2 = 1;
  AttackMissileDuplicationUnit = -1;
  AttackMissileDuplicationGraphic = -1;
  Unknown29 = 0;
  DisplayedPierceArmour = 0;
}

Creatable::~Creatable()
{

}

short Creatable::getResourceCostsSize()
{
  return 3;
}

void Creatable::serializeObject(void )
{
  serializeSub<ResourceCost>(ResourceCosts, getResourceCostsSize());
  serialize<int16_t>(TrainTime);
  serialize<int16_t>(TrainLocationID);
  serialize<char>(ButtonID);
  
  if (getGameVersion() >= genie::GV_AoK)
  {
    serialize<char>(Unknown26);
    serialize<int16_t>(Unknown27, UNKNOWN27_SIZE);
    serialize<char>(Unknown28);
    serialize<char>(MissileGraphicDelay);
    serialize<char>(HeroMode);
    serialize<int16_t>(GarrisonGraphic);
    serialize<float>(AttackMissileDuplicationAmount1);
    serialize<char>(AttackMissileDuplicationAmount2);
    serialize<float>(AttackMissileDuplicationSpawning, AMDBUNKNOWN_SIZE);
    serialize<int32_t>(AttackMissileDuplicationUnit);
    serialize<int32_t>(AttackMissileDuplicationGraphic);
    serialize<char>(Unknown29);
  }
  
  serialize<int16_t>(DisplayedPierceArmour);
}

}

}

