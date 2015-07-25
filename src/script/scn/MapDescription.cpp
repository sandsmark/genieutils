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
#include "genie/script/ScnFile.h"

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
  if (getGameVersion() >= genie::GV_TC) // 1.19
  {
    serialize<int32_t>(player1CameraX);
    serialize<int32_t>(player1CameraY);
    if (getGameVersion() >= genie::GV_TC) // 1.21
      serialize<int32_t>(aiType);
  }
  // <- here actually switches the reading function in exe
  serialize<uint32_t>(width);
  serialize<uint32_t>(height);

  serializeSub<MapTile>(tiles, width * height);
}

MapTile::MapTile()
{
  terrainID = 0;
  elevation = 0;
  unused = 0;
}

MapTile::~MapTile()
{
}

void MapTile::serializeObject(void)
{
  serialize<uint8_t>(terrainID);
  serialize<uint8_t>(elevation);
  serialize<uint8_t>(unused);
}

ScnPlayerResources::ScnPlayerResources()
{
}

ScnPlayerResources::~ScnPlayerResources()
{
}

void ScnPlayerResources::serializeObject(void)
{
  serialize<float>(food);
  serialize<float>(wood);
  serialize<float>(gold);
  serialize<float>(stone);
  serialize<float>(ore);
  if (getGameVersion() < genie::GV_SWGB)
    serialize<float>(padding);
  if (getGameVersion() >= genie::GV_TC) // 1.21
    serialize<float>(popLimit);
}

ScnUnit::ScnUnit()
{
}

ScnUnit::~ScnUnit()
{
}

void ScnUnit::serializeObject(void)
{
  serialize<float>(positionX);
  serialize<float>(positionY);
  serialize<float>(positionZ);
  serialize<uint32_t>(ID);
  serialize<uint16_t>(unitID);
  serialize<uint8_t>(unknown2);
  serialize<float>(rotation);
  serialize<uint16_t>(initAnimationFrame);
  serialize<uint32_t>(garrisonedInID);
}

ScnPlayerUnits::ScnPlayerUnits()
{
}

ScnPlayerUnits::~ScnPlayerUnits()
{
}

void ScnPlayerUnits::serializeObject(void)
{
  serializeSize<uint32_t>(unitCount_, units.size());
  serializeSub<ScnUnit>(units, unitCount_);
}

MapUnits::MapUnits()
{
}

MapUnits::~MapUnits()
{
}

void MapUnits::serializeObject(void)
{
  serializeSize<uint32_t>(playerCount_, playerUnits.size());
  serializeSub<ScnPlayerResources>(playerResources, 8);
  serializeSub<ScnPlayerUnits>(playerUnits, playerCount_);
}

}
