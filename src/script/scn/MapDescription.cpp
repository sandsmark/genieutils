/*
    genieutils - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2015 - 2016  Mikko "Tapsa" P
    Copyright (C) 2015  JustTesting1234

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

#include "genie/script/scn/MapDescription.h"
#include "genie/script/ScnFile.h"

namespace genie {

void ScnMap::serializeObject(void)
{
    serialize<uint32_t>(width);
    serialize<uint32_t>(height);

    if (width >= 46340) {
        throw std::out_of_range("Width (" + std::to_string(width) + ") out of range");
    }
    if (height >= 46340) {
        throw std::out_of_range("Height (" + std::to_string(height) + ") out of range");
    }

    (void)0;
    serialize(tiles, size_t(width) * size_t(height));
}

void MapTile::serializeObject(void)
{
    serialize<uint8_t>(terrainID);
    serialize<uint8_t>(elevation);
    serialize<uint8_t>(unused);
}
} // namespace genie
