/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2015  Mikko "Tapsa" P

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

#ifndef GENIE_TERRAINBORDER_H
#define GENIE_TERRAINBORDER_H
#include "genie/file/ISerializable.h"
#include "genie/dat/TerrainCommon.h"

namespace genie
{

/// Seems to be unused in GV > RoR
class TerrainBorder : public SharedTerrain
{
public:
  TerrainBorder();
  virtual ~TerrainBorder();
  virtual void setGameVersion(GameVersion gv);

  unsigned short getNameSize(void);

  std::vector<FrameData> Frames;

  int16_t DrawTile; // always 0
  int16_t UnderlayTerrain;
  int16_t BorderStyle;

private:
  virtual void serializeObject(void);
};

}

#endif // GENIE_TERRAINBORDER_H
