/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml <email>

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


#ifndef GENIE_PROJECTILEONLY_H
#define GENIE_PROJECTILEONLY_H

#include "genie/file/ISerializable.h"

namespace genie
{
  
namespace unit
{

class ProjectileOnly : public ISerializable
{

public:
  ProjectileOnly();
  virtual ~ProjectileOnly();
  
  char StretchMode;
  char CompensationMode;
  char DropAnimationMode;
  
  /// Affects the graphics so that they pass through the target instead of stopping
  /// 1 allows the projectile to pass through, a value of 0 stops the projectile.
  /// Only affects graphic not pass through damage.
  char PenetrationMode;
  
  char Unknown24;
  
  /// Determines the arc a projectile follows.
  /// Enter a non-negative value.
  /// The higher the value, the higher the arc will be that the projectile travels. 
  /// The graphics do not change, so a scorpion bolt will still always point forwards, 
  /// even if it has a very high arc.
  float ProjectileArc;
  
private:  
  virtual void serializeObject(void);
};

}

}

#endif // GENIE_PROJECTILEONLY_H
