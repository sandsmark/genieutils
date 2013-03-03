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

#ifndef GENIE_BIRD_H
#define GENIE_BIRD_H

#include "genie/file/ISerializable.h"
#include "../UnitCommand.h"

namespace genie
{

namespace unit
{

class Bird : public ISerializable
{
public:
  Bird();
  virtual ~Bird();
  virtual void setGameVersion(GameVersion gv);

  int16_t SheepConversion;//FFFF = No, 0000 = Yes
  float SearchRadius;
  float WorkRate;
  std::pair<int16_t, int16_t> DropSite;

  /// If activated unit switches villager types
  char VillagerMode;
  int16_t MoveSound;
  int16_t StopSound;
  /// Some animals have this.
  char AnimalMode;
  uint16_t CommandCount;//only in aoe/ror
  std::vector<UnitCommand> Commands;//only in aoe/ror

private:
  virtual void serializeObject(void);
};

}

}

#endif // GENIE_BIRD_H
