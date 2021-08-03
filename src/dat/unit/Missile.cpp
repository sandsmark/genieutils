/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2021  Mikko "Tapsa" P

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

#include "genie/dat/unit/Missile.h"

#include "../TestHelpers.h"

namespace genie::unit {

bool Missile::compareTo(const Missile &other) const
{
<<<<<<< HEAD:src/dat/unit/Missile.cpp
    COMPARE_MEMBER(ProjectileArc);
    COMPARE_MEMBER(AreaEffectSpecials);
    COMPARE_MEMBER(HitMode);
    COMPARE_MEMBER(ProjectileType);
    COMPARE_MEMBER(SmartMode);
    COMPARE_MEMBER(VanishMode);

    return true;
=======
  serialize<uint8_t>(ProjectileType);
  serialize<uint8_t>(SmartMode);
  serialize<uint8_t>(HitMode);
  serialize<uint8_t>(VanishMode);
  serialize<uint8_t>(AreaEffectSpecials);
  serialize<float>(ProjectileArc);
>>>>>>> 65dd660 (More accurate signedness.):src/dat/unit/Projectile.cpp
}

} // namespace genie
