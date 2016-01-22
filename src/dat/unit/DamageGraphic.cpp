/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2016  Mikko "Tapsa" P

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

#include "genie/dat/unit/DamageGraphic.h"

namespace genie
{

namespace unit
{

DamageGraphic::DamageGraphic()
{
  GraphicID = -1;
  DamagePercent = 0;
  OldApplyMode = 0;
  ApplyMode = 0;
}

DamageGraphic::~DamageGraphic()
{
}

//------------------------------------------------------------------------------
void DamageGraphic::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

void DamageGraphic::serializeObject(void)
{
  serialize<int16_t>(GraphicID);
  serialize<int8_t>(DamagePercent);
  serialize<int8_t>(OldApplyMode);
  serialize<int8_t>(ApplyMode);
}

}

}
