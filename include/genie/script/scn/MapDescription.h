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

#ifndef GENIE_MAP_DESCRIPTION_H
#define GENIE_MAP_DESCRIPTION_H

#include "genie/file/ISerializable.h"
#include <stdint.h>

namespace genie
{

class MapTile : public ISerializable
{
public:
  MapTile();
  virtual ~MapTile();

  uint8_t terrainID;
  uint8_t elevation;

  /// always 0
  uint8_t unused;

private:
  virtual void serializeObject(void);
};

/// Naming it MapDescription because it may be used elsewhere
class MapDescription : public ISerializable
{
public:
  MapDescription();
  virtual ~MapDescription();

  int32_t player1CameraX;
  int32_t player1CameraY;

  /// GV >= TC TODO enum
  int32_t aiType;

  /// AoK caps at 256
  uint32_t width;

  /// AoK caps at 256
  uint32_t height;

  std::vector<MapTile> tiles;

private:
  uint32_t separator_;

  virtual void serializeObject(void);
};

}

#endif // GENIE_MAP_DESCRIPTION_H
