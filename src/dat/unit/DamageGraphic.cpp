/*
    genie/dat - A library for reading and writing data files of genie
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


#include "genie/dat/unit/DamageGraphic.h"

namespace genie
{
  
namespace unit
{

DamageGraphic::DamageGraphic()
{
  GraphicID = -1;
  DamagePercent = 0;
  Unknown1 = 0;
  Unknown2 = 0;
}

DamageGraphic::~DamageGraphic()
{

}

void DamageGraphic::serializeObject(void )
{
  serialize<int16_t>(GraphicID);
  serialize<char>(DamagePercent);
  serialize<char>(Unknown1);
  serialize<char>(Unknown2);
}

}

}
