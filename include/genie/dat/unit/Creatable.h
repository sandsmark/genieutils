/*
    geniedat - A library for reading and writing data files of genie
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

#ifndef GENIE_CREATABLE_H
#define GENIE_CREATABLE_H

#include "genie/file/ISerializable.h"
#include "../ResourceUsage.h"

namespace genie
{

namespace unit
{

class Creatable : public ISerializable
{
public:
  Creatable();
  virtual ~Creatable();
  virtual void setGameVersion(GameVersion gv);

  static short getResourceCostsSize();

  typedef ResourceUsage<int16_t, int16_t, int16_t> ResourceCost;

  static const unsigned short RESOURCECOSTS_SIZE = 3;
  std::array<ResourceCost, RESOURCECOSTS_SIZE> ResourceCosts;
  int16_t TrainTime;
  int16_t TrainLocationID;
  int8_t ButtonID;
  float Unknown26;
  float Unknown27;
  int8_t Unknown28;
  int8_t HeroMode;
  int32_t GarrisonGraphic; 
  float TotalMissiles;
  int8_t TotalMissilesMax;

  static const unsigned short AMDBUNKNOWN_SIZE = 3;
  /// The area from which projectiles spawn. Size and randomness.
  std::array<float, AMDBUNKNOWN_SIZE> MissileSpawningArea;

  int32_t AlternativeProjectileUnit;
  int32_t ChargingGraphic;

  //TODO:
  /*
   * 1.Determines adjacent unit graphics
    2.Enter 0 or 1
    3.If “1”, the building will have the capability to change graphics depending
    on the buildings adjacent to it. For instance, a single wall will be a
    wall end piece, but if a wall is placed between two other walls, it will
    have a different graphic.
   */
  int8_t ChargingMode;
  int16_t DisplayedPierceArmour;

private:
  virtual void serializeObject(void);

};

}

}

#endif // GENIE_CREATABLE_H
