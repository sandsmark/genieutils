/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml <email>
    Copyright (C) 2011 - 2013  Mikko T P

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

#ifndef GENIE_UNITCOMMAND_H
#define GENIE_UNITCOMMAND_H
#include "genie/file/ISerializable.h"

namespace genie
{

class UnitCommand : public ISerializable
{
public:
  UnitCommand();
  virtual ~UnitCommand();
  virtual void setGameVersion(GameVersion gv);

  int16_t One;
  int16_t ID;
  int8_t Unknown1;
  int16_t Type;
  int16_t ClassID;
  int16_t UnitID;
  int16_t TerrainID;
  int16_t ResourceIn;
  int16_t ResourceProductivityMultiplier;
  int16_t ResourceOut;
  int16_t Resource;
  float WorkRateMultiplier;
  float ExecutionRadius;
  float ExtraRange;
  int8_t Unknown4;
  float Unknown5;
  int8_t SelectionEnabler;
  int8_t Unknown7;
  int16_t Unknown8;
  int16_t Unknown9;
  int8_t SelectionMode;
  int8_t Unknown11;
  int8_t Unknown12;

  static const unsigned short GRAPHICS_SIZE = 6;
  std::array<int16_t, GRAPHICS_SIZE> Graphics;

private:
  virtual void serializeObject(void);
};

}

#endif // GENIE_UNITCOMMAND_H
