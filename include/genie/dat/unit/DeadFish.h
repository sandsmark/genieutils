/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2013  Armin Preiml <email>

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
  DeadFish(GameVersion gv = GV_None);
  virtual ~DeadFish();
  virtual void setGameVersion(GameVersion gv);
  
  std::pair<int16_t, int16_t> WalkingGraphic;
  float RotationSpeed;
  char Unknown11;
  int16_t TrackingUnit;
  char TrackingUnitUsed;
  float TrackingUnitDensity;
  float Unknown12;//not in aoe/ror
  
  short getUnknown16Size();
  std::vector<char> Unknown16; //17 //4 int16_t, float, 3 int16_t aoe/ror 1 char
  
private:
  virtual void serializeObject(void);
};

}

}

#endif // GENIE_DEADFISH_H
