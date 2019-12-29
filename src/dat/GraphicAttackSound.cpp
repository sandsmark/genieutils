/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2019  Mikko "Tapsa" P

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

#include "genie/dat/GraphicAttackSound.h"

#include "TestHelpers.h"

#include <string.h>

namespace genie {

bool GraphicAngleSound::compareTo(const GraphicAngleSound &other) const
{
    COMPARE_MEMBER(FrameNum);
    COMPARE_MEMBER(FrameNum2);
    COMPARE_MEMBER(FrameNum3);
    COMPARE_MEMBER(SoundID);
    COMPARE_MEMBER(SoundID2);
    COMPARE_MEMBER(SoundID3);
    COMPARE_MEMBER(WwiseSoundID);
    COMPARE_MEMBER(WwiseSoundID2);
    COMPARE_MEMBER(WwiseSoundID3);

    return true;
}

void GraphicAngleSound::serializeObject(void)
{
    GameVersion gv = getGameVersion();

    serialize<int16_t>(FrameNum);
    serialize<int16_t>(SoundID);
    if (gv >= GV_C4 && gv <= GV_LatestDE2) {
        serialize<uint32_t>(WwiseSoundID);
    }

    serialize<int16_t>(FrameNum2);
    if (gv >= GV_C4 && gv <= GV_LatestDE2) {
        serialize<uint32_t>(WwiseSoundID2);
    }
    serialize<int16_t>(SoundID2);

    serialize<int16_t>(FrameNum3);
    if (gv >= GV_C4 && gv <= GV_LatestDE2) {
        serialize<uint32_t>(WwiseSoundID3);
    }
    serialize<int16_t>(SoundID3);
}
} // namespace genie
