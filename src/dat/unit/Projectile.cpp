/*
    genie/dat - A library for reading and writing data files of genie
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

#include "genie/dat/unit/Projectile.h"

namespace genie
{

namespace unit
{

Projectile::Projectile() : GraphicDisplacement()
{
  Unknown20 = 0;
  Unknown20_1 = 0;
  Unknown21 = -1;
  MaxRange = 0;
  BlastRadius = 0;
  ReloadTime1 = 0;
  ProjectileUnitID = -1;
  AccuracyPercent = 0;
  TowerMode = 0;
  Delay = 0;
  Unknown23 = 0;
  MinRange = 0;
  GarrisonRecoveryRate = 0;
  AttackGraphic = -1;
  DisplayedMeleeArmour = 0;
  DisplayedAttack = 0;
  DisplayedRange = 0;
  ReloadTime2 = 0;
}

Projectile::~Projectile()
{
}

void Projectile::serializeObject(void)
{
  serialize<char>(Unknown20);

  if (getGameVersion() >= genie::GV_TC)
    serialize<char>(Unknown20_1);

  serializeSize<uint16_t>(AttackCount, Attacks.size());
  serializeSub<unit::AttackOrArmor>(Attacks, AttackCount);

  serializeSize<uint16_t>(ArmourCount, Armours.size());
  serializeSub<unit::AttackOrArmor>(Armours, ArmourCount);

  serialize<int16_t>(Unknown21);
  serialize<float>(MaxRange);
  serialize<float>(BlastRadius);
  serialize<float>(ReloadTime1);
  serialize<int16_t>(ProjectileUnitID);
  serialize<int16_t>(AccuracyPercent);
  serialize<char>(TowerMode);
  serialize<int16_t>(Delay); //TODO: missle graphic delay
  serialize<float, GRAPHICDISPLACEMENT_SIZE>(GraphicDisplacement);
  serialize<char>(Unknown23); //TODO: AoE/RoR blast level
  serialize<float>(MinRange);

  if (getGameVersion() >= genie::GV_AoK)
    serialize<float>(GarrisonRecoveryRate);

  serialize<int16_t>(AttackGraphic);
  serialize<int16_t>(DisplayedMeleeArmour);
  serialize<int16_t>(DisplayedAttack);
  serialize<float>(DisplayedRange);
  serialize<float>(ReloadTime2);
}

}

}
