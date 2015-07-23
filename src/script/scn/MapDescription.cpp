/*
    genieutils - <description>
    Copyright (C) 2011 - 2013  Armin Preiml <email>
    Copyright (C) 2015  Mikko "Tapsa" P <email>

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

#include "genie/script/scn/MapDescription.h"

namespace genie
{

MapDescription::MapDescription()
{
  player1CameraX = 0;
  player1CameraY = 0;
  aiType = 0;
  width = 0;
  height = 0;
}

MapDescription::~MapDescription()
{
}

void MapDescription::serializeObject(void)
{
  serialize<int32_t>(player1CameraX);
  serialize<int32_t>(player1CameraY);
  serialize<int32_t>(aiType);
  serialize<uint32_t>(width);
  serialize<uint32_t>(height);

  serializeSub<MapTile>(tiles, width * height);
}

MapTile::MapTile()
{
  terrainId = 0;
  elevation = 0;
  unused = 0;
}

MapTile::~MapTile()
{
}

void MapTile::serializeObject(void)
{
  serialize<uint8_t>(terrainId);
  serialize<uint8_t>(elevation);
  serialize<uint8_t>(unused);
}

}
