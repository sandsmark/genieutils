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
  DefaultArmor = 1000;
  Unknown21 = -1;
  MaxRange = 0;
  BlastRadius = 0;
  ReloadTime1 = 0;
  ProjectileUnitID = -1;
  AccuracyPercent = 0;
  TowerMode = 0;
  Delay = 0;
  BlastLevel = 0;
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

//------------------------------------------------------------------------------
void Projectile::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  updateGameVersion(Attacks);
  updateGameVersion(Armours);
}

void Projectile::serializeObject(void)
{
  if (getGameVersion() < genie::GV_TC)
  {
    uint8_t defarmor_byte = DefaultArmor;
    serialize<uint8_t>(defarmor_byte);
    DefaultArmor = defarmor_byte;
  }
  else
  {
    serialize<int16_t>(DefaultArmor);
  }

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
  serialize<int8_t>(TowerMode);
  serialize<int16_t>(Delay); //TODO: missle graphic delay
  serialize<float, GRAPHICDISPLACEMENT_SIZE>(GraphicDisplacement);
  serialize<int8_t>(BlastLevel);
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
