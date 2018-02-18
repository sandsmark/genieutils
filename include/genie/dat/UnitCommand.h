/*
    geniedat - A library for reading and writing data files of genie
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

#ifndef GENIE_UNITCOMMAND_H
#define GENIE_UNITCOMMAND_H
#include "genie/file/ISerializable.h"

namespace genie
{


class Task : public ISerializable
{
public:
  Task();
  virtual ~Task();
  virtual void setGameVersion(GameVersion gv);

  int16_t TaskType = 1;
  int16_t ID = -1;
  int8_t IsDefault = false;

  /// What type of command/action is it
  enum ActionTypes : int16_t {
      // From AGE
      None = 0,
      MoveTo = 1,
      Follow = 2,
      Garrison = 3,
      Explore = 4,
      GatherRebuild = 5,
      Graze = 6,
      Combat = 7,
      Shoot = 8,
      Attack = 9,
      Fly = 10,
      ScareHunt = 11,
      UnloadBoat = 12,
      Guard = 13,
      SiegeTowerAbility = 14,
      Escape = 20,
      Make = 21,
      Num = 100, // wat
      Build = 101,
      MakeUnit = 102,
      MakeTech = 103,
      Convert = 104,
      Heal = 105,
      Repair = 106,
      GetAutoConverted = 107,
      DiscoveryArtifact = 108,
      ShootingRangeRetreat = 109, // automatically run away when attacked or something? doesn't set any parameters except autosearch and search wait time
      Hunt = 110,
      Trade = 111,
      GenerateWonderVictory = 120,

      FarmDeselectWhenTasked = 121,
      LootGather = 122,
      Housing = 123,
      Pack = 124,
      UnpackAttack = 125,
      OffMapTrade0 = 130,
      OffMapTrade1 = 131,
      PickupUnit = 132,
      ChargeAttack = 133,
      TransformUnit = 134,
      KidnapUnit = 135,
      DepositUnit = 136,
      Shear = 149,
      HDRegeneration = 150,
      HDFeitoria = 151,
  };
  int16_t ActionType = 0;
  static const std::string &actionTypeName(const int16_t type);
  const std::string &actionTypeName() const { return actionTypeName(ActionType); }

  int16_t ClassID = -1;
  int16_t UnitID = -1;
  int16_t TerrainID = -1;
  int16_t ResourceIn = -1;
  int16_t ResourceMultiplier = -1;
  int16_t ResourceOut = -1;
  int16_t UnusedResource = -1;
  float WorkValue1 = 0;
  float WorkValue2 = 0;
  float WorkRange = 0;
  int8_t AutoSearchTargets = 0;
  float SearchWaitTime = 0;
  int8_t EnableTargeting = 0;
  int8_t CombatLevelFlag = false;
  int16_t GatherType = 0;
  int16_t WorkFlag2 = 0;

  /// Owner Type
  /// Determines what you can select as targets
  /// > 7 is same as 0 and 7
  enum TargetDiplomacies : int8_t {
      TargetAnyDiplo = 0,           ///< All objects
      TargetSelf = 1,           ///< Your objects only
      TargetNeutralsEnemies = 2, ///< Neutral and enemy objects only
      TargetGaiaOnly = 3,           ///< Gaia only
      TargetSelfAllyGaia = 4,     ///< Gaia, your and ally objects only
      TargetGaiaNeutralEnemies = 5, ///< Gaia, neutral and enemy objects only
      TargetOthers = 6,             ///< All but your objects
      TargetAnyDiplo2 = 7,          ///< All objects
  };
  int8_t TargetDiplomacy = 0;

  int8_t CarryCheck = false;
  int8_t PickForConstruction = false;

  // walking with tool but no resource
  int16_t MovingGraphicID = -1;
  // proceeding resource gathering or attack
  int16_t ProceedingGraphicID = -1;
  // actual execution or transformation graphic
  int16_t WorkingGraphicID = -1;
  // display resources in hands
  int16_t CarryingGraphicID = -1;

  int16_t ResourceGatheringSoundID = -1;
  int16_t ResourceDepositSoundID = -1;


private:
  virtual void serializeObject(void);
};

}

#endif // GENIE_UNITCOMMAND_H
