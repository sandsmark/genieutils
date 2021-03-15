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

#include "genie/dat/PlayerColour.h"

#include "TestHelpers.h"

#include <string.h>

namespace genie {

bool PlayerColour::compareTo(const PlayerColour &other) const
{
    COMPARE_MEMBER(ID);
    COMPARE_MEMBER(PaletteBase);
    COMPARE_MEMBER(PlayerColorBase);
    COMPARE_MEMBER(UnitOutlineColor);
    COMPARE_MEMBER(UnitSelectionColor1);
    COMPARE_MEMBER(UnitSelectionColor2);
    COMPARE_MEMBER(MinimapColor2);
    COMPARE_MEMBER(MinimapColor3);
    COMPARE_MEMBER(StatisticsText);
    COMPARE_MEMBER(Name);
    COMPARE_MEMBER(ResourceID);
    COMPARE_MEMBER(Type);

    return true;
}

void PlayerColour::serializeObject()
{
    if (getGameVersion() < GV_AoKE3) {
        serialize(Name, NAME_SIZE);

        serialize<int16_t>(IdS16);

        serialize<int16_t>(ResourceID);

        serialize<uint8_t>(PaletteBaseU8);

        serialize<uint8_t>(Type);

        ID = IdS16;
        PaletteBase = PaletteBaseU8;
    } else {
        serialize<int32_t>(ID);
        serialize<int32_t>(PlayerColorBase);
        serialize<int32_t>(UnitOutlineColor);
        serialize<int32_t>(UnitSelectionColor1);
        serialize<int32_t>(UnitSelectionColor2);
        serialize<int32_t>(PaletteBase);
        serialize<int32_t>(MinimapColor2);
        serialize<int32_t>(MinimapColor3);
        serialize<int32_t>(StatisticsText);
    }
}
} // namespace genie
