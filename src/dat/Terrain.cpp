/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml <email>
    Copyright (C) 2011 - 2015  Mikko T P

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

int Terrain::customTerrainAmount;
//------------------------------------------------------------------------------
Terrain::Terrain() : Colors(), CliffColors(),
  TerrainUnitID(), TerrainUnitDensity(), TerrainUnitPriority()
{
  Unknown1 = 0;
  Enabled = 0;
  Random = 0;
  Name = "";
  Name2 = "";
  SLP = -1;
  Unknown3 = 0;
  SoundID = -1;
  BlendPriority = -1;
  BlendType = -1;
  PassableTerrain = -1;
  ImpassableTerrain = -1;
  TerrainToDraw = 0;
  NumberOfTerrainUnitsUsed = 0;
  TerrainDimensions.first = TerrainDimensions.second = -1;
}

//------------------------------------------------------------------------------
Terrain::~Terrain()
{
}

void Terrain::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  ElevationGraphics.resize(ELEVATION_GRAPHICS_SIZE);
  Borders.resize(getTerrainsSize(gv), 0);
}

//------------------------------------------------------------------------------
unsigned short Terrain::getTerrainsSize(GameVersion gv)
{
  if (gv >= genie::GV_SWGB)
    return 55;
  if (gv == genie::GV_Cysion)
    return Terrain::customTerrainAmount;//47; // 200 pls
  if (gv >= genie::GV_TC)
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

  serialize<std::string>(Name, getNameSize());
  serialize<std::string>(Name2, getNameSize());

  if (getGameVersion() >= genie::GV_AoEB)
    serialize<int32_t>(SLP);
  serialize<int32_t>(Unknown3);
  serialize<int32_t>(SoundID);

  if (getGameVersion() >= genie::GV_AoKB)
  {
    serialize<int32_t>(BlendPriority);
    serialize<int32_t>(BlendType);
  }

  serialize<uint8_t, 3>(Colors);
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

  serializeSub<FrameData>(ElevationGraphics, ELEVATION_GRAPHICS_SIZE);
  serialize<int16_t>(TerrainToDraw);
  serializePair<int16_t>(TerrainDimensions);
  if (isOperation(OP_READ))
    serialize<int16_t>(Borders, getTerrainsSize(getGameVersion()));
  else
    serialize<int16_t>(Borders, Borders.size());
  serialize<int16_t, TERRAIN_UNITS_SIZE>(TerrainUnitID);
  serialize<int16_t, TERRAIN_UNITS_SIZE>(TerrainUnitDensity);
  serialize<int8_t, TERRAIN_UNITS_SIZE>(TerrainUnitPriority);
  serialize<int16_t>(NumberOfTerrainUnitsUsed);

  if (getGameVersion() < genie::GV_SWGB)
    serialize<int16_t>(Unknown1);
}

//------------------------------------------------------------------------------
FrameData::FrameData()
{
  FrameCount = 0;
  AngleCount = 0;
  ShapeID = 0;
}

//------------------------------------------------------------------------------
FrameData::~FrameData()
{
}

//------------------------------------------------------------------------------
void FrameData::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

//------------------------------------------------------------------------------
void FrameData::serializeObject(void)
{
  serialize<int16_t>(FrameCount);
  serialize<int16_t>(AngleCount);
  serialize<int16_t>(ShapeID);
}

}
