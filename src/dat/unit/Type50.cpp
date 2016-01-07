/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2016  Mikko "Tapsa" P

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

#include "genie/dat/unit/Type50.h"

namespace genie
{

namespace unit
{

Type50::Type50() : GraphicDisplacement(GRAPHICDISPLACEMENT_SIZE)
{
  DefaultArmor = 1000;
  TerRestrictionForDmgMultiplying = -1;
  MaxRange = 0;
  BlastWidth = 0;
  ReloadTime = 0;
  ProjectileUnitID = -1;
  AccuracyPercent = 0;
  TowerMode = 0;
  FrameDelay = 0;
  BlastAttackLevel = 0;
  MinRange = 0;
  AccuracyDispersion = 0;
  AttackGraphic = -1;
  DisplayedMeleeArmour = 0;
  DisplayedAttack = 0;
  DisplayedRange = 0;
  DisplayedReloadTime = 0;
}

Type50::~Type50()
{
}

//------------------------------------------------------------------------------
void Type50::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  updateGameVersion(Attacks);
  updateGameVersion(Armours);
}

void Type50::serializeObject(void)
{
  if (getGameVersion() < genie::GV_TC) // 11.52
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

  serialize<int16_t>(TerRestrictionForDmgMultiplying);
  serialize<float>(MaxRange);
  serialize<float>(BlastWidth);
  serialize<float>(ReloadTime);
  serialize<int16_t>(ProjectileUnitID);
  serialize<int16_t>(AccuracyPercent);
  serialize<int8_t>(TowerMode);
  serialize<int16_t>(FrameDelay);
  serialize<float>(GraphicDisplacement, GRAPHICDISPLACEMENT_SIZE);
  serialize<int8_t>(BlastAttackLevel);
  serialize<float>(MinRange);

  if (getGameVersion() >= genie::GV_AoKB) // 10.36
    serialize<float>(AccuracyDispersion);

  serialize<int16_t>(AttackGraphic);
  if (getGameVersion() >= genie::GV_AoEB) // 7.01
  {
    serialize<int16_t>(DisplayedMeleeArmour);
    serialize<int16_t>(DisplayedAttack);
    serialize<float>(DisplayedRange);
    serialize<float>(DisplayedReloadTime);
  }
}

}

}
