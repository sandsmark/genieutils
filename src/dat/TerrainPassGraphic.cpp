/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2013  Mikko "Tapsa" P

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

#include "genie/dat/TerrainPassGraphic.h"

#include <iostream>
#include <string.h>

namespace genie
{

//------------------------------------------------------------------------------
TerrainPassGraphic::TerrainPassGraphic()
{
  Buildable = 0;
  GraphicIDs.first = -1;
  GraphicIDs.second = -1;
  ReplicationAmount = 0;
}

//------------------------------------------------------------------------------
TerrainPassGraphic::~TerrainPassGraphic()
{
}

//------------------------------------------------------------------------------
void TerrainPassGraphic::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

//------------------------------------------------------------------------------
void TerrainPassGraphic::serializeObject()
{
  serialize<int32_t>(Buildable);
  serializePair<int32_t>(GraphicIDs);
  if (getGameVersion() < genie::GV_SWGB)
  {
    int32_t replicationAmount = ReplicationAmount;
    serialize<int32_t>(replicationAmount);
    ReplicationAmount = replicationAmount;
  }
  else
  {
    serialize<float>(ReplicationAmount);
  }
}

}
