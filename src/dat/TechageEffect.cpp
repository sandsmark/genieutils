/*
    genie/dat - A library for reading and writing data files of genie
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

#include "genie/dat/TechageEffect.h"

namespace genie
{

//------------------------------------------------------------------------------
TechageEffect::TechageEffect()
{
  Type = -1;
  A = -1;
  B = -1;
  C = -1;
  D = 0;
}

//------------------------------------------------------------------------------
TechageEffect::~TechageEffect()
{
}

//------------------------------------------------------------------------------
void TechageEffect::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

//------------------------------------------------------------------------------
void TechageEffect::serializeObject(void)
{
  serialize<int8_t>(Type);
  serialize<int16_t>(A);  //unit (TODO)
  serialize<int16_t>(B);  //class
  serialize<int16_t>(C);  //attribute
  serialize<float>(D);  //amount
}

}
