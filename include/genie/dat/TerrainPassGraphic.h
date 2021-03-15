/*
    geniedat - A library for reading and writing data files of genie
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

#pragma once
#include "genie/file/ISerializable.h"

namespace genie {

class TerrainPassGraphic : public ISerializable
{
public:
    int32_t ExitTileSpriteID = -1;
    int32_t EnterTileSpriteID = -1;
    int32_t WalkTileSpriteID = -1;

    union {
        int32_t WalkSpriteRate = 0;
        float WalkSpriteRateF;
    };

    bool compareTo(const TerrainPassGraphic &other) const;

private:
    void serializeObject() override;
};
} // namespace genie

