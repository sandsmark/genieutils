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

#include "genie/dat/unit/Projectile.h"

namespace genie
{

namespace unit
{

Projectile::Projectile() //: Type50()
{
}

Projectile::~Projectile()
{
}

//------------------------------------------------------------------------------
void Projectile::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

void Projectile::serializeObject(void)
{
  serialize<int8_t>(StretchMode);
  serialize<int8_t>(SmartMode);
  serialize<int8_t>(DropAnimationMode);
  serialize<int8_t>(PenetrationMode);
  serialize<int8_t>(Unknown24);
  serialize<float>(ProjectileArc);
}

}

}
