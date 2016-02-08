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

#include "genie/dat/unit/Building.h"

namespace genie
{

namespace unit
{

Building::Building() : //Creatable(),
  Annexes(BUILDING_ANNEXES_SIZE),
  LootingTable(LOOTABLE_RES_COUNT)
{
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
  serialize<int16_t>(GraphicsAngle);
  serialize<int8_t>(DisappearsWhenBuilt);
  serialize<int16_t>(StackUnitID);
  serialize<int16_t>(FoundationTerrainID);
  serialize<int16_t>(OldTerrainLikeID); // Resource?
  serialize<int16_t>(ResearchID);

  if (getGameVersion() >= genie::GV_AoKA)
  {
    serialize<int8_t>(Unknown33);
    serializeSub<unit::BuildingAnnex>(Annexes, BUILDING_ANNEXES_SIZE); // 40 bytes
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
      serialize<int8_t>(LootingTable, LOOTABLE_RES_COUNT); // 9.26
    }
  }
}

}

}
