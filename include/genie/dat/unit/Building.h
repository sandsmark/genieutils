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

#ifndef GENIE_BUILDING_H
#define GENIE_BUILDING_H

#include "genie/file/ISerializable.h"
#include "BuildingAnnex.h"

namespace genie
{

namespace unit
{

class Building : public ISerializable
{
public:
  Building();
  virtual ~Building();
  virtual void setGameVersion(GameVersion gv);

  int16_t ConstructionGraphicID;
  int16_t SnowGraphicID; //not in aok/aoe/ror
  /// Enabler for annex units.
  int16_t AdjacentMode;
  int8_t IconDisabler;
  int8_t Unknown31b;
  int16_t StackUnitID;
  int16_t TerrainID;
  int16_t Unknown32;
  int16_t ResearchID;
  int8_t Unknown33;

  static const unsigned short BUILDING_ANNEXES_SIZE = 4;
  std::array<unit::BuildingAnnex, BUILDING_ANNEXES_SIZE> Annexes;
  int16_t HeadUnit;
  int16_t TransformUnit;
  int16_t Unknown34;
  int16_t ConstructionSound;
  int8_t GarrisonType;
  float GarrisonHealRate;
  int32_t Unknown35;

  /// Related to annexes in any way?
  /// Widespread usage in the AoK alpha 09.07.0222
  static const unsigned short ALFATHING_SIZE = 8;
  std::array<int8_t, ALFATHING_SIZE> AlfaThingy;

private:
  virtual void serializeObject(void);
};

}

}

#endif // GENIE_BUILDING_H
