/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2014  Mikko "Tapsa" P

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

#ifndef GENIE_BUILDING_H
#define GENIE_BUILDING_H

#include "genie/file/ISerializable.h"

namespace genie
{

namespace unit
{

/// A building can hold annexes like the town center.
class BuildingAnnex : public ISerializable
{
public:
  BuildingAnnex()
  {
    UnitID = -1;
    Misplacement.first = 0;
    Misplacement.second = 0;
  }
  virtual ~BuildingAnnex() {}
  virtual void setGameVersion(GameVersion gv)
  {
    ISerializable::setGameVersion(gv);
  }

  int16_t UnitID;
  std::pair <float, float> Misplacement;

private:
  virtual void serializeObject(void)
  {
    serialize<int16_t>(UnitID);
    serializePair<float>(Misplacement);
  }
};

class Building : public ISerializable
{
public:
  Building();
  virtual ~Building();
  virtual void setGameVersion(GameVersion gv);

  int16_t ConstructionGraphicID;
  int16_t SnowGraphicID; //not in aok/aoe/ror
  /// Enabler for annex units.
  int8_t AdjacentMode;
  int16_t GraphicsAngle;
  int8_t DisappearsWhenBuilt;
  int16_t StackUnitID;
  int16_t FoundationTerrainID;
  /// ES forgot to remove this when AoE was still in its alpha stages
  int16_t OldTerrainLikeID;
  int16_t ResearchID;
  int8_t Unknown33;

  static const unsigned short BUILDING_ANNEXES_SIZE = 4;
  std::vector<unit::BuildingAnnex> Annexes;
  int16_t HeadUnit;
  int16_t TransformUnit;
  int16_t UnknownSound;
  int16_t ConstructionSound;
  int8_t GarrisonType;
  float GarrisonHealRate;
  float Unknown35;
  int16_t PileUnit;

  /// Related to annexes in any way?
  /// Widespread usage in the AoK alpha 09.07.0222
  static const unsigned short LOOTABLE_RES_COUNT = 6;
  std::vector<int8_t> LootingTable;

private:
  virtual void serializeObject(void);
};

}

}

#endif // GENIE_BUILDING_H
