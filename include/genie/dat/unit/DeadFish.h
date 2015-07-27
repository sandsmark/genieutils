/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2014  Mikko "Tapsa" P

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

#ifndef GENIE_DEADFISH_H
#define GENIE_DEADFISH_H

#include "genie/file/ISerializable.h"

namespace genie
{

namespace unit
{

/// Attributes for Unit of type >= DeadFish.
class DeadFish : public ISerializable
{
public:
  DeadFish();
  virtual ~DeadFish();
  virtual void setGameVersion(GameVersion gv);

  std::pair<int16_t, int16_t> WalkingGraphic;
  float RotationSpeed;
  int8_t Unknown11;
  int16_t TrackingUnit;
  int8_t TrackingUnitUsed;
  float TrackingUnitDensity;
  int8_t Unknown16;

  static const unsigned short U16B_SIZE = 5;
  std::vector<float> RotationAngles;

private:
  virtual void serializeObject(void);
};

}

}

#endif // GENIE_DEADFISH_H
