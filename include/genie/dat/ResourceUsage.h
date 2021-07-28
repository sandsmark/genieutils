/*
    geniedat - A library for reading and writing data files of genie
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

#pragma once
#include "genie/file/ISerializable.h"

#include "ResourceType.h"

namespace genie {

//------------------------------------------------------------------------------
/// Template class describing quantities of Resources.
/// It is a template class because different objects need different datatypes
/// for ResourceUsage members.
//
template <typename A, typename E>
class Resource : public ISerializable
{
public:
    void setGameVersion(GameVersion gv) override
    {
        ISerializable::setGameVersion(gv);
    }

    int16_t Type = -1;

    /// Amount of the resource available/required/used
    A Amount = 0;

    /// Bool that determines whether it is paid or only needed.
    E Paid = 0;

    bool compareTo (const Resource<A, E> &other) const {
        if (Type != other.Type) {
            std::cerr << "Type mismatch" << std::endl;
            return false;
        }
        if (Amount != other.Amount) {
            std::cerr << "Amount mismatch" << std::endl;
            return false;
        }
        if (Paid != other.Paid) {
            std::cerr << "Paid mismatch" << std::endl;
            return false;
        }
        return true;
    }

private:
    void serializeObject() override
    {
        serialize<int16_t>(Type);
        serialize<A>(Amount);
        serialize<E>(Paid);
    }
};
} // namespace genie

