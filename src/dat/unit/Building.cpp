/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml <email>
    Copyright (C) 2011 - 2014  Mikko T P

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

#include "genie/dat/unit/Building.h"

namespace genie
{

namespace unit
{

Building::Building() : Annexes(), AlfaThingy()
{
  ConstructionGraphicID = -1;
  SnowGraphicID = -1;
  AdjacentMode = 0;
  IconDisabler = 0;
  DisappearsWhenBuilt = 0;
  StackUnitID = -1;
  TerrainID = -1;
  ResourceID = -1;
  ResearchID = -1;
  PlayerAlive = 0;
  HeadUnit = -1;
  TransformUnit = -1;
  UnknownSound = -1;
  ConstructionSound = -1;
  GarrisonType = 0;
  GarrisonHealRate = 0;
  Unknown35 = 0;
  PileUnit = -1;
}

Building::~Building()
{
}

//------------------------------------------------------------------------------
void Building::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

void Building::serializeObject(void)
{
  serialize<int16_t>(ConstructionGraphicID);

  if (getGameVersion() >= genie::GV_TC) // 11.53
    serialize<int16_t>(SnowGraphicID);

  serialize<int8_t>(AdjacentMode);
  serialize<int16_t>(IconDisabler);
  serialize<int8_t>(DisappearsWhenBuilt);
  serialize<int16_t>(StackUnitID);
  serialize<int16_t>(TerrainID);
  serialize<int16_t>(ResourceID); // Resource?
  serialize<int16_t>(ResearchID);

  if (getGameVersion() >= genie::GV_AoKA)
  {
    serialize<int8_t>(PlayerAlive);
    serializeSub<unit::BuildingAnnex, BUILDING_ANNEXES_SIZE>(Annexes); // 40 bytes
    serialize<int16_t>(HeadUnit); // 9.89
    serialize<int16_t>(TransformUnit);
    serialize<int16_t>(UnknownSound);
  }

  serialize<int16_t>(ConstructionSound);

  if (getGameVersion() >= genie::GV_AoKA)
  {
    serialize<int8_t>(GarrisonType);
    serialize<float>(GarrisonHealRate);
    serialize<float>(Unknown35);
	{
      serialize<int16_t>(PileUnit); // 9.06
      serialize<int8_t, LOOTABLE_RES_COUNT>(AlfaThingy); // 9.26
    }
  }
}

}

}
