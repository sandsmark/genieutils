/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2013  Armin Preiml <email>

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
  
  int16_t ConstructionGraphicID;
  int16_t SnowGraphicID; //not in aok/aoe/ror
  /// Enabler for annex units.
  int16_t AdjacentMode;
  char Unknown31;
  char Unknown31b;
  int16_t StackUnitID;
  int16_t TerrainID;
  int16_t Unknown32;
  int16_t ResearchID;
  char Unknown33;
  
  static const uint16_t BUILDING_ANNEXES_CNT = 4;
  
  std::vector<unit::BuildingAnnex> Annexes; // 4
  int16_t HeadUnit;
  int16_t TransformUnit;
  int16_t Unknown34;
  int16_t ConstructionSound;
  char GarrisonType;
  float GarrisonHealRate;
  int32_t Unknown35;
  int16_t Unknown36;
  
  static const uint16_t UNKNOWN37_SIZE = 6;
  std::vector<char> Unknown37; // 6
  
private:  
  virtual void serializeObject(void);
};

}

}

#endif // GENIE_BUILDING_H
