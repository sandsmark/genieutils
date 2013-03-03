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


#ifndef GENIE_UNIT_H
#define GENIE_UNIT_H
#include "genie/file/ISerializable.h"
#include "unit/DamageGraphic.h"
#include "UnitCommand.h"
#include "unit/AttackOrArmor.h"
#include "unit/BuildingAnnex.h"
#include "unit/DeadFish.h"
#include "unit/Bird.h"
#include "unit/Projectile.h"
#include "unit/ProjectileOnly.h"
#include "unit/Creatable.h"
#include "unit/Building.h"
#include "ResourceUsage.h"

namespace genie
{

/// Available unit types
enum UnitType
{
  /// Basic units like rubble and flares.
  UT_EyeCandy = 10,

  /// With Speed but mostly flags. Purpose of speed is unknown
  UT_Flag = 20,

  /// Only one unit has this type. AOK, DOPL (id 243) same properties as UT_Flag
  UT_25 = 25,

  /// Dead and fish units. It seems to be unused in SWGB as units just explode
  /// and do not leave carcasses.
  UT_Dead_Fish = 30,

  /// Only birds in aoe and ror are of this type.
  UT_Bird = 40,

  /// Projectiles
  UT_Projectile = 60,

  /// Units that can be created or trained like Army, Villagers and Ships.
  UT_Creatable = 70,

  /// Buildings
  UT_Building = 80,

  /// Trees in aoe and ror are of this type
  UT_AoeTrees = 90
};


//------------------------------------------------------------------------------
/// Stores properties of genie units.
//
class Unit : public ISerializable
{
public:
  Unit(GameVersion gv = GV_None);
  virtual ~Unit();
  virtual void setGameVersion(GameVersion gv);

  /// See enum UnitType
  char Type;

  /// Length of the internal name
  uint16_t NameLength;

  int16_t ID1;

  /// ID of the name string stored in the language.dll file
  uint16_t LanguageDLLName;

  /// ID of the name string stored in the language.dll file
  uint16_t LanguageDLLCreation;

  /// Class that gives certain properties
  int16_t Class;

  /// Default standing graphic
  std::pair<int16_t, int16_t> StandingGraphic;

  /// Graphic shown while the units is dying.
  std::pair<int16_t, int16_t> DyingGraphic;

  /// TODO
  char DeathMode;

  /// Unit's hit points
  int16_t HitPoints;

  /// Default line of sight measured in tiles
  float LineOfSight;

  /// Maximum number of units that can be garrisonned at the same time.
  char GarrisonCapacity;

  /// Area taken by the unit. No other unit can move into this area except
  /// flying units.
  std::pair<float, float> SizeRadius;

  float HPBarHeight1;

  /// Sound played when the unit is created
  std::pair<int16_t, int16_t> TrainSound;

  /// ID of the unit to change to when this one dies.
  int16_t DeadUnitID;

  /// TODO
  char PlacementMode;

  /// TODO
  char AirMode;

  /// ID of the icon shown at the bottom when selected or when building
  int16_t IconID;

  /// If true, unit can not be placed in the editor
  char HideInEditor;

  /// TODO Unknown
  int16_t Unknown1;         // Always -1

  /// TODO
  int16_t Enabled;
  std::pair <int16_t, int16_t> PlacementBypassTerrain;
  std::pair <int16_t, int16_t> PlacementTerrain;
  std::pair <float, float> EditorRadius;
  char BuildingMode;
  char VisibleInFog;

  /// ID of terrain restrictions that are imposed on the unit.
  int16_t TerrainRestriction;

  /// Bool which determines wheter the unit can fly.
  char FlyMode;

  /// How much resources this unit is able to carry
  int16_t ResourceCapacity;

  // TODO
  float ResourceDecay;
  char BlastType;
  char Unknown2;
  char InteractionMode;

  /**
   * \brief Defines if and how a unit is shown on the minimap.
   *
   * Following values are known to work:
   *
   * - 0 does not place a dot on the mini-map. Eye candy, resources,
   *     fish traps, OUTLWs, and farms have this value
   * - 1 places a square dot on the min-map for the unit. When the unit is
   *     selected, the dot will turn white
   * - 2 places a diamond-shaped dot on the mini-map for the unit. When the unit
   *     is selected, the dot will turn white
   * - 3 places a diamond-shaped dot on the mini-map for the unit. When the unit
   *     is selected, the dot remains the player's color.
   * - 4 places a larger black spot on the minimap which does not follow the
   *     unit. Whenever you open the chat box (enter) and close it, the black
   *     spots will move to the unit on the minimap. No blinking occurs when
   *     the unit is attacked. Both allied and enemy units are visible in the
   *     black area on the minimap. CLF units have this value and a brown spot
   *     is placed instead.
   * - 5 does the same thing as “4”
   * - 6 does not place a dot on the minimap
   * - 7-9 does the same thing as “6”
   * - 10 does the same thing as “6”. Hawks, macaws, and flying dogs have
   *      this value.
   */
  char MinimapMode;

  int16_t CommandAttribute;// Page for Build button: 2 = Page 1, 10 = Page 2, ?11 = Page 3?
  int16_t Unknown3;
  int16_t Unknown3B;
  int32_t LanguageDLLHelp;
  int32_t LanguageDLLHotKeyText;
  int32_t HotKey;
  char Unselectable;
  char Unknown6;
  char Unknown7;// not in aoe/ror
  char Unknown8;// not in aoe/ror
  char SelectionMask;

  /// values for SelectionShapeType
  /// 0 farm, gate, dead bodies, town center
  /// 2 buildings, gold mine
  /// 3 berserk, flag x
  /// 5 units
  /// 10 mountain(matches selction mask)
  char SelectionShapeType;

  /// 0 square, 1+ circle
  char SelectionShape;
  char Attribute;
  /// Seems to be used only in SWGB/CC
  char Civilization;
  static short getUnknown9Size();
  std::vector<char> Unknown9; // 3 //aoe/ror 1, aok 0, tc 3

  /**
   * 1.HP Bar on, selection shadow (permanent darker outline in editor only, disappears in game)
   * 2.HP Bar on, normal outline
   * 3.HP Bar off, selection shadow
   * 4.HP Bar off, normal outline
   * Note: this doe not work with all units
   */
  char SelectionEffect;
  char EditorSelectionColour;
  std::pair <float, float> SelectionRadius;
  float HPBarHeight2;

  static const unsigned short RESOURCE_STORAGE_CNT = 3;

  typedef ResourceUsage<int16_t, float, char> ResourceStorage;

  /// Resource cost of a unit TODO (3 different types of resource costs??)
  std::vector<ResourceStorage> ResourceStorages; //3

  unsigned char DamageGraphicCount;
  std::vector<unit::DamageGraphic> DamageGraphics;

  /// Sound that is played when this unit is selected
  int16_t SelectionSound;
  int16_t DyingSound;
  int16_t AttackSound;
  std::string Name;
  uint16_t NameLength2;
  std::string Name2;

  /// MinGameVersion: SWGB
  int16_t Unitline;

  /// MinGameVersion: SWGB
  char MinTechLevel;

  int16_t ID2;
  int16_t ID3;//not in aoe/ror

//      Type 20+

  float Speed;

  unit::DeadFish DeadFish;

  unit::Bird Bird;

  unit::Projectile Projectile;

  unit::ProjectileOnly ProjectileOnly;

  unit::Creatable Creatable;

  unit::Building Building;

private:
  virtual void serializeObject(void);
};

}

#endif // GENIE_UNIT_H
