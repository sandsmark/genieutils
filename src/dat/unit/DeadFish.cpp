/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml <email>
    Copyright (C) 2011 - 2014  Mikko T P

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

#include "genie/dat/unit/DeadFish.h"

namespace genie
{

namespace unit
{

DeadFish::DeadFish() : Unknown16B()
{
  WalkingGraphic.first = -1;
  WalkingGraphic.second = -1;
  RotationSpeed = 0;
  Unknown11 = 0;
  TrackingUnit = -1;
  TrackingUnitUsed = 0;
  TrackingUnitDensity = 0;
  Unknown16 = 0;
}

DeadFish::~DeadFish()
{
}

//------------------------------------------------------------------------------
void DeadFish::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

void DeadFish::serializeObject(void)
{
  serializePair<int16_t>(WalkingGraphic, (getGameVersion() < genie::GV_AoE) ? true : false);
  if (getGameVersion() >= genie::GV_AoE)
  {
    serialize<float>(RotationSpeed);
    serialize<int8_t>(Unknown11);
  }
  else // Ugly hack
  {
    serialize<int16_t>(TrackingUnit);
    serialize<int8_t>(TrackingUnitUsed);
    serialize<float>(TrackingUnitDensity);
  }
  serialize<int16_t>(TrackingUnit);
  serialize<int8_t>(TrackingUnitUsed);
  serialize<float>(TrackingUnitDensity);
  serialize<int8_t>(Unknown16);

  if (getGameVersion() >= genie::GV_AoKB) // 10.28
  {
    serialize<float, U16B_SIZE>(Unknown16B);
  }
}

}

}
