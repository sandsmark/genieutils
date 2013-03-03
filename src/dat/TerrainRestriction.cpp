/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml <email>

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
  
short TerrainRestriction::Terrain_Count_ = 0;

//------------------------------------------------------------------------------
TerrainRestriction::TerrainRestriction() : TerrainAccessible(Terrain_Count_),
                                           TerrainPassGraphics(Terrain_Count_)
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
void TerrainRestriction::setTerrainCount(short Terrain_Count)
{
  Terrain_Count_ = Terrain_Count;
}

//------------------------------------------------------------------------------
void TerrainRestriction::serializeObject(void)
{
  serialize<float>(TerrainAccessible, Terrain_Count_);
  
  if (getGameVersion() >= genie::GV_AoKA)
  {
    serializeSub<TerrainPassGraphic>(TerrainPassGraphics, Terrain_Count_);
  }
}

}
