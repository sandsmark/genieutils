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

#include "genie/dat/UnitLine.h"

#include "TestHelpers.h"

namespace genie {

//------------------------------------------------------------------------------
bool UnitLine::compareTo(const UnitLine &other) const
{
    COMPARE_MEMBER(ID);
    COMPARE_MEMBER(Name);
    COMPARE_MEMBER_VEC(UnitIDs);

    return true;
}

void UnitLine::serializeObject()
{
    serialize<int16_t>(ID);

<<<<<<< HEAD
    uint16_t name_len{};
    serializeSize<uint16_t>(name_len, Name, false);
    serialize(Name, name_len);
=======
  int16_t name_len;
  serializeSize<int16_t>(name_len, Name, false);
  serialize(Name, name_len);

  serializeSize<int16_t>(name_len, UnitIDs.size());
  serialize<int16_t>(UnitIDs, name_len);
}
>>>>>>> 65dd660 (More accurate signedness.)

    serializeSize<uint16_t>(name_len, UnitIDs.size());
    serialize<int16_t>(UnitIDs, name_len);
}
} // namespace genie
