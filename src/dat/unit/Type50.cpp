/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2017  Mikko "Tapsa" P

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

Type50::Type50() //: Bird()
{
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
  GameVersion gv = getGameVersion();
  if (gv < GV_TC) // 11.52
  {
    uint8_t defarmor_byte = BaseArmor;
    serialize<uint8_t>(defarmor_byte);
    BaseArmor = defarmor_byte;
  }
  else
  {
    serialize<int16_t>(BaseArmor);
  }

  uint16_t attack_count;
  serializeSize<uint16_t>(attack_count, Attacks.size());
  serializeSub<unit::AttackOrArmor>(Attacks, attack_count);

  serializeSize<uint16_t>(attack_count, Armours.size());
  serializeSub<unit::AttackOrArmor>(Armours, attack_count);

  serialize<int16_t>(DefenseTerrainBonus);
  serialize<float>(MaxRange);
  serialize<float>(BlastWidth);
  serialize<float>(ReloadTime);
  serialize<int16_t>(ProjectileUnitID);
  serialize<int16_t>(AccuracyPercent);
  serialize<int8_t>(BreakOffCombat); // Not used anymore
  serialize<int16_t>(FrameDelay);
  serialize<float>(GraphicDisplacement, 3);
  serialize<int8_t>(BlastAttackLevel);
  serialize<float>(MinRange);

  if (gv >= GV_AoKB) // 10.36
    serialize<float>(AccuracyDispersion);

  serialize<int16_t>(AttackGraphic);
  if (gv >= GV_AoEB) // 7.01
  {
    serialize<int16_t>(DisplayedMeleeArmour);
    serialize<int16_t>(DisplayedAttack);
    serialize<float>(DisplayedRange);
    serialize<float>(DisplayedReloadTime);
  }
}

}

}
