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

#include "genie/dat/TerrainBorder.h"

namespace genie
{

//------------------------------------------------------------------------------
TerrainBorder::TerrainBorder() : Borders(TILE_TYPE_COUNT)
{
}

//------------------------------------------------------------------------------
TerrainBorder::~TerrainBorder()
{
}

//------------------------------------------------------------------------------
void TerrainBorder::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

//------------------------------------------------------------------------------
unsigned short TerrainBorder::getNameSize()
{
  return 13;
}

//------------------------------------------------------------------------------
void TerrainBorder::serializeObject(void)
{
  serialize<int8_t>(Enabled);
  serialize<int8_t>(Random);

  serialize(Name, getNameSize());
  serialize(Name2, getNameSize());

  if (getGameVersion() >= genie::GV_AoEB)
    serialize<int32_t>(SLP);
  serialize<int32_t>(Unknown3);
  serialize<int32_t>(SoundID);
  serialize<uint8_t>(Colors, 3);

  serialize<int8_t>(IsAnimated);
  serialize<int16_t>(AnimationFrames);
  serialize<int16_t>(PauseFames);
  serialize<float>(Interval);
  serialize<float>(PauseBetweenLoops);
  serialize<int16_t>(Frame);
  serialize<int16_t>(DrawFrame);
  serialize<float>(AnimateLast);
  serialize<int8_t>(FrameChanged);
  serialize<int8_t>(Drawn);

  for (auto &sub: Borders)
    serializeSub<FrameData>(sub, getGameVersion() == genie::GV_MIK ? 13 : 12);

  serialize<int16_t>(DrawTile);
  serialize<int16_t>(UnderlayTerrain);

  if (getGameVersion() != genie::GV_MIK)
    serialize<int16_t>(BorderStyle);
}

}
