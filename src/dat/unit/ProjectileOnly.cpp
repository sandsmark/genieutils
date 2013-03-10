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

#include "genie/dat/unit/ProjectileOnly.h"

namespace genie
{

namespace unit
{

ProjectileOnly::ProjectileOnly()
{
  StretchMode = 0;
  CompensationMode = 0;
  DropAnimationMode = 0;
  PenetrationMode = 0;
  Unknown24 = 0;
  ProjectileArc = 0;
}

ProjectileOnly::~ProjectileOnly()
{
}

//------------------------------------------------------------------------------
void ProjectileOnly::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

void ProjectileOnly::serializeObject(void)
{
  serialize<char>(StretchMode);
  serialize<char>(CompensationMode);
  serialize<char>(DropAnimationMode);
  serialize<char>(PenetrationMode);
  serialize<char>(Unknown24);
  serialize<float>(ProjectileArc);
}

}

}
