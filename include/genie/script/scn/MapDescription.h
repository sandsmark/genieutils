/*
    genieutils - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2015  Mikko "Tapsa" P
    Copyright (C) 2015  JustTesting1234

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

  virtual void serializeObject(void);
};

class ScnUnit : public ISerializable
{
public:
  ScnUnit();
  virtual ~ScnUnit();

  float positionX;
  float positionY;
  float positionZ;
  uint32_t ID;
  uint16_t unitID;
  uint8_t unknown2;
  float rotation;
  uint16_t initAnimationFrame;
  uint32_t garrisonedInID;

private:
  virtual void serializeObject(void);
};

class ScnPlayerResources : public ISerializable
{
public:
  ScnPlayerResources();
  virtual ~ScnPlayerResources();

  float food;
  float wood;
  float gold;
  float stone;
  float ore;
  float padding;
  float popLimit;

private:
  virtual void serializeObject(void);
};

class ScnPlayerUnits : public ISerializable
{
public:
  ScnPlayerUnits();
  virtual ~ScnPlayerUnits();

  std::vector<ScnUnit> units;

private:
  uint32_t unitCount_;
  virtual void serializeObject(void);
};

class MapUnits : public ISerializable
{
public:
  MapUnits();
  virtual ~MapUnits();

  std::vector<ScnPlayerResources> playerResources;
  std::vector<ScnPlayerUnits> playerUnits;

private:
  uint32_t playerCount_;
  virtual void serializeObject(void);
};

}

#endif // GENIE_MAP_DESCRIPTION_H
