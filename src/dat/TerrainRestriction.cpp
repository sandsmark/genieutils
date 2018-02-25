/*
    genie/dat - A library for reading and writing data files of genie
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

#include "genie/dat/TerrainRestriction.h"

#include <iostream>

namespace genie
{

unsigned short TerrainRestriction::terrain_count_ = 0;

//------------------------------------------------------------------------------
TerrainRestriction::TerrainRestriction() :
  PassableBuildableDmgMultiplier(terrain_count_),
  TerrainPassGraphics(terrain_count_)
{
}

//------------------------------------------------------------------------------
TerrainRestriction::~TerrainRestriction()
{
}

//------------------------------------------------------------------------------
void TerrainRestriction::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  updateGameVersion(TerrainPassGraphics);
}

//------------------------------------------------------------------------------
void TerrainRestriction::setTerrainCount(unsigned short cnt)
{
  terrain_count_ = cnt;
}

//------------------------------------------------------------------------------
void TerrainRestriction::serializeObject(void)
{
  serialize<float>(PassableBuildableDmgMultiplier, terrain_count_);

  GameVersion gv = getGameVersion();
  if (gv >= GV_AoKA || (gv >= GV_T4 && gv <= GV_LatestTap))
  {
    serializeSub<TerrainPassGraphic>(TerrainPassGraphics, terrain_count_);
  }
}

}
