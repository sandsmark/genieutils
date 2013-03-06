/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml <email>
    Copyright (C) 2011 - 2013  Mikko T P

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

#ifndef GENIE_PROJECTILE_H
#define GENIE_PROJECTILE_H

#include "genie/file/ISerializable.h"
#include "AttackOrArmor.h"

namespace genie
{

namespace unit
{

class Projectile : public ISerializable
{
public:
  Projectile();
  virtual ~Projectile();

  char Unknown20; //2 //aoe/ror 1, aok 1, tc 2
  char Unknown20_1;
  std::vector<unit::AttackOrArmor> Attacks;
  std::vector<unit::AttackOrArmor> Armours;
  int16_t Unknown21;
  float MaxRange;
  float BlastRadius;
  float ReloadTime1;
  int16_t ProjectileUnitID;

  /// Percentage value determining the probability of an attack hiting
  int16_t AccuracyPercent;

  /// Used in AoE/RoR for towers.
  char TowerMode;
  int16_t Delay;

  static const unsigned short GRAPHICDISPLACEMENT_SIZE = 3;
  std::array<float, GRAPHICDISPLACEMENT_SIZE> GraphicDisplacement;

  char Unknown23;
  float MinRange;
  float GarrisonRecoveryRate;
  int16_t AttackGraphic;
  int16_t DisplayedMeleeArmour;
  int16_t DisplayedAttack;
  float DisplayedRange;
  float ReloadTime2;

private:
  uint16_t AttackCount;
  uint16_t ArmourCount;

  virtual void serializeObject(void);
};

}

}

#endif // GENIE_PROJECTILE_H
