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

#include "genie/dat/Terrain.h"

namespace genie
{

unsigned short Terrain::terrain_count;
//------------------------------------------------------------------------------
Terrain::Terrain() : ElevationGraphics(TILE_TYPE_COUNT),
  TerrainUnitID(TERRAIN_UNITS_SIZE), TerrainUnitDensity(TERRAIN_UNITS_SIZE),
  TerrainUnitPriority(TERRAIN_UNITS_SIZE)
{
}

//------------------------------------------------------------------------------
Terrain::~Terrain()
{
}

void Terrain::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  Borders.resize(getTerrainCount(gv), 0);
}

//------------------------------------------------------------------------------
void Terrain::setTerrainCount(unsigned short cnt)
{
  terrain_count = cnt;
}

//------------------------------------------------------------------------------
unsigned short Terrain::getTerrainCount(GameVersion gv)
{
  if (terrain_count)
    return terrain_count;
  if (gv >= genie::GV_SWGB)
    return 55;
  if (gv == genie::GV_Cysion)
    return 100;
  if (gv == genie::GV_TC)
    return 42;
    return 32;
}

//------------------------------------------------------------------------------
unsigned short Terrain::getNameSize()
{
  if (getGameVersion() >= genie::GV_SWGB)
    return 17;
  else
    return 13;
}

//------------------------------------------------------------------------------
void Terrain::serializeObject(void)
{
  serialize<int8_t>(Enabled);
  serialize<int8_t>(Random);

  serialize(Name, getNameSize());
  serialize(Name2, getNameSize());

  if (getGameVersion() >= genie::GV_AoEB)
    serialize<int32_t>(SLP);
  serialize<int32_t>(Unknown3);
  serialize<int32_t>(SoundID);

  if (getGameVersion() >= genie::GV_AoKB)
  {
    serialize<int32_t>(BlendPriority);
    serialize<int32_t>(BlendType);
  }

  serialize<uint8_t>(Colors, 3);
  serializePair<uint8_t>(CliffColors);
  serialize<int8_t>(PassableTerrain);
  serialize<int8_t>(ImpassableTerrain);

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

  serializeSub<FrameData>(ElevationGraphics, TILE_TYPE_COUNT);
  serialize<int16_t>(TerrainToDraw);
  serializePair<int16_t>(TerrainDimensions);
  if (isOperation(OP_READ))
    serialize<int16_t>(Borders, getTerrainCount(getGameVersion()));
  else
    serialize<int16_t>(Borders, Borders.size());
  serialize<int16_t>(TerrainUnitID, TERRAIN_UNITS_SIZE);
  serialize<int16_t>(TerrainUnitDensity, TERRAIN_UNITS_SIZE);
  serialize<int8_t>(TerrainUnitPriority, TERRAIN_UNITS_SIZE);
  serialize<int16_t>(NumberOfTerrainUnitsUsed);

  if (getGameVersion() < genie::GV_SWGB)
    serialize<int16_t>(Unknown1);
}

}
