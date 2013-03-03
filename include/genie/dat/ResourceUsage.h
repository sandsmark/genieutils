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


#ifndef GENIE_RESOURCEUSAGE_H
#define GENIE_RESOURCEUSAGE_H
#include "genie/file/ISerializable.h"

namespace genie
{

//------------------------------------------------------------------------------
/// Template class describing quantities of Resources.
/// It is a template class because different objects need different datatypes
/// for ResourceUsage members.
//
template<typename T, typename A, typename E>
class ResourceUsage : public ISerializable
{

public:
  ResourceUsage()
  {
    Type = -1;
    Amount = 0;
    Enabled = 0;
  }
  
  virtual ~ResourceUsage() {}
 
  /// Resource ID TODO add resource ids
  T Type;
  
  /// Amount of the resource available/required/used
  A Amount;
  
  /// Bool that determines wheter it is used or not
  E Enabled;
    
private:
  virtual void serializeObject(void)
  {
    serialize<T>(Type);
    serialize<A>(Amount);
    serialize<E>(Enabled);
  }
 
};

}

#endif // GENIE_RESOURCEUSAGE_H
