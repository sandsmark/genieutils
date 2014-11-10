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
  Unknown33 = 0;
  HeadUnit = -1;
  TransformUnit = -1;
  UnknownUnit = -1;
  ConstructionSound = -1;
  GarrisonType = 0;
  GarrisonHealRate = 0;
  Unknown35 = 0;
  Unknown36 = -1;
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

  if (getGameVersion() >= genie::GV_TC)
    serialize<int16_t>(SnowGraphicID);

  serialize<int8_t>(AdjacentMode);
  serialize<int16_t>(IconDisabler);
  serialize<int8_t>(DisappearsWhenBuilt);
  serialize<int16_t>(StackUnitID);
  serialize<int16_t>(TerrainID);
  serialize<int16_t>(ResourceID); // Unit?
  serialize<int16_t>(ResearchID);

  if (getGameVersion() >= genie::GV_AoKA)
  {
    serialize<int8_t>(Unknown33);
    serializeSub<unit::BuildingAnnex, BUILDING_ANNEXES_SIZE>(Annexes);
    serialize<int16_t>(HeadUnit);
    serialize<int16_t>(TransformUnit);
    serialize<int16_t>(UnknownUnit); // Unit?
  }

  serialize<int16_t>(ConstructionSound);

  if (getGameVersion() >= genie::GV_AoKA)
  {
    serialize<int8_t>(GarrisonType);
    serialize<float>(GarrisonHealRate);
    serialize<int32_t>(Unknown35);
    serialize<int16_t>(Unknown36);
    serialize<int8_t, ALFATHING_SIZE>(AlfaThingy);
  }
}

}

}
