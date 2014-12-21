/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2014  Mikko T P

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

#ifndef GENIE_TERRAINBLOCK_H
#define GENIE_TERRAINBLOCK_H
#include "genie/file/ISerializable.h"
#include "genie/dat/Terrain.h"
#include "genie/dat/TerrainBorder.h"

namespace genie
{

class TerrainBlock : public ISerializable
{
public:
  TerrainBlock();
  virtual ~TerrainBlock();
  virtual void setGameVersion(GameVersion gv);

  static const unsigned short TERRAIN_HEADER_SIZE = 70;
  std::array<int16_t, TERRAIN_HEADER_SIZE> GraphicsRendering;
  std::vector<Terrain> Terrains;

  std::vector<TerrainBorder> TerrainBorders;

  unsigned short getZeroSpaceSize(void);
  std::vector<int16_t> ZeroSpace;

  uint16_t NumberOfTerrainsUsed2;

  unsigned short getRenderingSize(void);
  std::vector<int16_t> Rendering;
  unsigned short getSomethingSize(void);
  std::vector<int32_t> Something;

private:
  virtual void serializeObject(void);
};

}

#endif // GENIE_DATFILE_H
