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

  unsigned short getTerrainHeaderSize(void);
  std::vector<int16_t> GraphicsRendering;
  std::vector<Terrain> Terrains;

  std::vector<int16_t> AoEAlphaUnknown; // These are very similar to borders
  std::vector<TerrainBorder> TerrainBorders;

  unsigned short getZeroSpaceSize(void);
  std::vector<int16_t> ZeroSpace;

  uint16_t TerrainsUsed2;
  uint16_t RemovedBlocksUsed;
  uint16_t TerrainBordersUsed;

  unsigned short getCivDataSize(void);
  std::vector<int16_t> CivData;
  unsigned short getBytesSize(void);
  std::vector<int8_t> SomeBytes;
  unsigned short getSomethingSize(void);
  std::vector<int32_t> SomeInt32;

private:
  virtual void serializeObject(void);
};

}

#endif // GENIE_DATFILE_H
