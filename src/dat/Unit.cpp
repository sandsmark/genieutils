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

#include "genie/dat/Unit.h"

namespace genie
{

//------------------------------------------------------------------------------
Unit::Unit() : ResourceStorages(3)
{
}

//------------------------------------------------------------------------------
Unit::~Unit()
{
}

//------------------------------------------------------------------------------
void Unit::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  updateGameVersion(DamageGraphics);
  DeadFish.setGameVersion(gv);
  Bird.setGameVersion(gv);
  Type50.setGameVersion(gv);
  Projectile.setGameVersion(gv);
  Creatable.setGameVersion(gv);
  Building.setGameVersion(gv);
}

//------------------------------------------------------------------------------
void Unit::serializeObject(void)
{
  //Type 10+
  if (getGameVersion() < genie::GV_AoEB && isOperation(OP_WRITE)) Type /= 10;
  serialize<int8_t>(Type); // 7 = 70 in AoE alphas etc
  if (getGameVersion() < genie::GV_AoEB && isOperation(OP_READ)) Type *= 10;

  serializeSize<uint16_t>(NameLength, Name);
  serialize<int16_t>(ID1);        //TODO: Check
  serialize<uint16_t>(LanguageDLLName);
  if (getGameVersion() >= genie::GV_MATT)
    serialize<uint16_t>(LanguageDLLCreation);
  serialize<int16_t>(Class);
  serializePair<int16_t>(StandingGraphic, (getGameVersion() >= genie::GV_AoKB) ? false : true);
  serializePair<int16_t>(DyingGraphic);
  serialize<int8_t>(DeathMode);
  serialize<int16_t>(HitPoints);
  serialize<float>(LineOfSight);
  serialize<int8_t>(GarrisonCapacity);
  serialize<float>(CollisionSize.x);
  serialize<float>(CollisionSize.y);
  serialize<float>(CollisionSize.z);
  serializePair<int16_t>(TrainSound, (getGameVersion() >= genie::GV_AoKA) ? false : true);
  serialize<int16_t>(DeadUnitID);
  serialize<int8_t>(PlacementMode);
  serialize<int8_t>(AirMode);
  serialize<int16_t>(IconID);
  serialize<int8_t>(HideInEditor);
  serialize<int16_t>(Unknown1);
  serialize<int8_t>(Enabled);

  if (getGameVersion() >= genie::GV_AoK) // 11.48
    serialize<int8_t>(Disabled);

  if (getGameVersion() >= genie::GV_MIK)
    serializePair<int16_t>(PlacementSideTerrain);
  serializePair<int16_t>(PlacementTerrain); // Before AoE, this also contains side terrain.
  serializePair<float>(ClearanceSize);
  serialize<int8_t>(HillMode);
  serialize<int8_t>(VisibleInFog);
  serialize<int16_t>(TerrainRestriction);
  serialize<int8_t>(FlyMode);
  serialize<int16_t>(ResourceCapacity);
  serialize<float>(ResourceDecay);
  serialize<int8_t>(BlastDefenseLevel);
  serialize<int8_t>(SubType);
  serialize<int8_t>(InteractionMode);
  serialize<int8_t>(MinimapMode);
  serialize<int8_t>(CommandAttribute);
  serialize<float>(Unknown3A);
  serialize<uint8_t>(MinimapColor);
  if (getGameVersion() >= genie::GV_AoEB)
  {
    serialize<int32_t>(LanguageDLLHelp);
    serialize<int32_t>(LanguageDLLHotKeyText);
    serialize<int32_t>(HotKey);
    serialize<int8_t>(Unselectable);
    serialize<int8_t>(Unknown6);
    serialize<int8_t>(Unknown7);
    serialize<int8_t>(Unknown8);

    if (getGameVersion() >= genie::GV_AoKA)
    {
      serialize<int8_t>(SelectionMask);
      serialize<int8_t>(SelectionShapeType);
      serialize<int8_t>(SelectionShape);
      if (getGameVersion() >= genie::GV_TC)
      {
        serialize<uint8_t>(Attribute);
        serialize<int8_t>(Civilization);
        serialize<int16_t>(Nothing);
      }
    }

    serialize<int8_t>(SelectionEffect);
    serialize<uint8_t>(EditorSelectionColour);
    serialize<float>(SelectionShapeSize.x);
    serialize<float>(SelectionShapeSize.y);
    serialize<float>(SelectionShapeSize.z);
  }

  serializeSub<ResourceStorage>(ResourceStorages, 3);

  serializeSize<uint8_t>(DamageGraphicCount, DamageGraphics.size());
  serializeSub<unit::DamageGraphic>(DamageGraphics, DamageGraphicCount);

  serialize<int16_t>(SelectionSound);
  serialize<int16_t>(DyingSound);
  serialize<int8_t>(AttackMode);
  serialize<int8_t>(Unknown10);

  serialize(Name, NameLength);

  if (getGameVersion() >= genie::GV_SWGB)
  {
    serializeSize<uint16_t>(NameLength2, Name2);
    serialize(Name2, NameLength2);

    serialize<int16_t>(Unitline);
    serialize<int8_t>(MinTechLevel);
  }

  if (getGameVersion() >= genie::GV_AoE)
    serialize<int16_t>(ID2);

  if (getGameVersion() >= genie::GV_AoKA)
    serialize<int16_t>(ID3);

  if (Type == genie::UT_AoeTrees)
    return;

  if (Type >= genie::UT_Flag)
    serialize<float>(Speed);
  else
    return;

  if (Type >= genie::UT_Dead_Fish)
    serialize<ISerializable>(DeadFish);

  if (Type >= genie::UT_Bird)
    serialize<ISerializable>(Bird);

  if (Type >= genie::UT_Unknown)
    serialize<ISerializable>(Type50);

  if (Type == genie::UT_Projectile)
    serialize<ISerializable>(Projectile);

  if (Type >= genie::UT_Creatable)
    serialize<ISerializable>(Creatable);

  if (Type == genie::UT_Building)
    serialize<ISerializable>(Building);
}

}
