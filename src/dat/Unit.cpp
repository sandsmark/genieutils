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


#include "genie/dat/Unit.h"


namespace genie
{

//------------------------------------------------------------------------------
Unit::Unit(GameVersion gv) : Unknown9(getUnknown9Size())
{
  setGameVersion(gv);
//    Type 10+

  Type = UT_EyeCandy;
  NameLength = 9;
  ID1 = -1;
  LanguageDLLName = 5000;
  LanguageDLLCreation = 6000;
  Class = -1;
  StandingGraphic.first = -1;
  StandingGraphic.second = -1;
  DyingGraphic.first = -1;
  DyingGraphic.second = -1;
  DeathMode = 0;
  HitPoints = 1;
  LineOfSight = 2;
  GarrisonCapacity = 0;
  SizeRadius.first = 0;
  SizeRadius.second = 0;
  HPBarHeight1 = 0;
  TrainSound.first = -1;
  TrainSound.second = -1;
  DeadUnitID = -1;
  PlacementMode = 0;
  AirMode = 0;
  IconID = -1;
  HideInEditor = 0;
  Unknown1 = -1;
  Enabled = 0;
  PlacementBypassTerrain.first = -1;
  PlacementBypassTerrain.second = -1;
  PlacementTerrain.first = -1;
  PlacementTerrain.second = -1;
  EditorRadius.first = 0;
  EditorRadius.second = 0;
  BuildingMode = 0;
  VisibleInFog = 0;
  TerrainRestriction = 0;
  FlyMode = 0;
  ResourceCapacity = 0;
  ResourceDecay = 0;
  BlastType = 0;
  Unknown2 = 0;
  InteractionMode = 0;
  MinimapMode = 0;
  CommandAttribute = 0;
  Unknown3 = 0;
  Unknown3B = 0;
  LanguageDLLHelp = 105000;
  LanguageDLLHotKeyText = 155000;
  HotKey = 16000;
  Unselectable = 0;
  Unknown6 = 0;
  Unknown7 = 0;
  Unknown8 = 0;
  SelectionMask = 0;
  SelectionShapeType = 0;
  SelectionShape = 0;
  Attribute = 0;
  Civilization = 0;
  SelectionEffect = 1;
  EditorSelectionColour = 52;
  SelectionRadius.first = 0;
  SelectionRadius.second = 0;
  HPBarHeight2 = 0;
  ResourceStorages.resize(RESOURCE_STORAGE_CNT);
  SelectionSound = -1;
  DyingSound = -1;
  AttackSound = -1;
  Name = "";
  NameLength2 = 9;
  Name2 = "";
  Unitline = -1;
  MinTechLevel = -1;
  ID2 = -1;
  ID3 = -1;

//  Type 20+

  Speed = 1;
}

//------------------------------------------------------------------------------
Unit::~Unit()
{
}


//------------------------------------------------------------------------------
void Unit::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  updateGameVersion(ResourceStorages);
  updateGameVersion(DamageGraphics);

  DeadFish.setGameVersion(gv);
  Bird.setGameVersion(gv);
  Projectile.setGameVersion(gv);
  ProjectileOnly.setGameVersion(gv);
  Creatable.setGameVersion(gv);
  Building.setGameVersion(gv);
}


//------------------------------------------------------------------------------
short Unit::getUnknown9Size()
{
  return 2;
}

//------------------------------------------------------------------------------
void Unit::serializeObject(void)
{
  //Type 10+
  serialize<char>(Type);

  serializeSize<uint16_t>(NameLength, Name);
  serialize<int16_t>(ID1);        //TODO: Check
  serialize<uint16_t>(LanguageDLLName);
  serialize<uint16_t>(LanguageDLLCreation);
  serialize<int16_t>(Class);

  if (getGameVersion() >= genie::GV_AoK)
    serialize<int16_t>(StandingGraphic);
  else
    serialize<int16_t>(StandingGraphic, true);

  serialize<int16_t>(DyingGraphic);
  serialize<char>(DeathMode);
  serialize<int16_t>(HitPoints);
  serialize<float>(LineOfSight);
  serialize<char>(GarrisonCapacity);
  serialize<float>(SizeRadius);

  serialize<float>(HPBarHeight1);

  serialize<int16_t>(TrainSound, (getGameVersion() >= genie::GV_AoK) ? false : true);
  serialize<int16_t>(DeadUnitID);
  serialize<char>(PlacementMode);
  serialize<char>(AirMode);
  serialize<int16_t>(IconID);
  serialize<char>(HideInEditor);
  serialize<int16_t>(Unknown1);

  if (getGameVersion() >= genie::GV_AoK)
    serialize<int16_t>(Enabled);
  else
  {
    char enabled = Enabled;
    serialize<char>(enabled);
    Enabled = enabled;
  }

  serialize<int16_t>(PlacementBypassTerrain);
  serialize<int16_t>(PlacementTerrain);
  serialize<float>(EditorRadius);
  serialize<char>(BuildingMode);
  serialize<char>(VisibleInFog);
  serialize<int16_t>(TerrainRestriction);
  serialize<char>(FlyMode);
  serialize<int16_t>(ResourceCapacity);
  serialize<float>(ResourceDecay);
  serialize<char>(BlastType); //TODO: AoE/ROR: [0]:blast_type?
  serialize<char>(Unknown2);
  serialize<char>(InteractionMode);
  serialize<char>(MinimapMode);
  serialize<int16_t>(CommandAttribute);
  serialize<int16_t>(Unknown3);
  serialize<int16_t>(Unknown3B);
  serialize<int32_t>(LanguageDLLHelp);
  serialize<int32_t>(LanguageDLLHotKeyText);
  serialize<int32_t>(HotKey);
  serialize<char>(Unselectable);
  serialize<char>(Unknown6);

  if (getGameVersion() >= genie::GV_AoK)
  {
    serialize<char>(Unknown7);
    serialize<char>(Unknown8);
  }

  serialize<char>(SelectionMask);

  if (getGameVersion() >= genie::GV_AoK)
    serialize<char>(SelectionShapeType);

  serialize<char>(SelectionShape);

  if (getGameVersion() >= genie::GV_TC)
  {
    serialize<char>(Attribute);
    serialize<char>(Civilization);
    serialize<char>(Unknown9, getUnknown9Size());
  }

  serialize<char>(SelectionEffect);
  serialize<char>(EditorSelectionColour);
  serialize<float>(SelectionRadius);
  serialize<float>(HPBarHeight2);

  serializeSub<ResourceStorage>(ResourceStorages, 3);

  serializeSize<unsigned char>(DamageGraphicCount, DamageGraphics.size());
  serializeSub<unit::DamageGraphic>(DamageGraphics, DamageGraphicCount);

  serialize<int16_t>(SelectionSound);
  serialize<int16_t>(DyingSound);
  serialize<int16_t>(AttackSound);

  serialize<std::string>(Name, NameLength);

  if (getGameVersion() >= genie::GV_SWGB)
  {
    //serializeSize<uint16_t>(NameLength2, Name2.size());
    serializeSize<uint16_t>(NameLength2, Name2);
    serialize<std::string>(Name2, NameLength2);

    serialize<int16_t>(Unitline);
    serialize<char>(MinTechLevel);
  }

  serialize<int16_t>(ID2);

  if (getGameVersion() >= genie::GV_AoK)
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

  if (Type >= genie::UT_Projectile)
    serialize<ISerializable>(Projectile);

  if (Type == genie::UT_Projectile)
    serialize<ISerializable>(ProjectileOnly);

  if (Type >= genie::UT_Creatable)
    serialize<ISerializable>(Creatable);

  if (Type >= genie::UT_Building)
    serialize<ISerializable>(Building);
}


}

