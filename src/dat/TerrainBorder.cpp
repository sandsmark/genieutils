/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2017  Mikko "Tapsa" P

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

#include "genie/dat/TerrainBorder.h"

#include "TestHelpers.h"

namespace genie {

//------------------------------------------------------------------------------
TerrainBorder::TerrainBorder()
{
}

//------------------------------------------------------------------------------
size_t TerrainBorder::getNameSize()
{
    return 13;
}

bool TerrainBorder::compareTo(const TerrainBorder &other) const
{
    COMPARE_MEMBER(BorderStyle);
    COMPARE_MEMBER(DrawTerrain);
    COMPARE_MEMBER(UnderlayTerrain);
    COMPARE_MEMBER_VEC_VEC(Borders);

    return true;
}

//------------------------------------------------------------------------------
void TerrainBorder::serializeObject()
{
    GameVersion gv = getGameVersion();

    serialize<int8_t>(Enabled);
    serialize<int8_t>(Random);

    serialize(Name, getNameSize());
    serialize(Name2, getNameSize());

    if (gv >= GV_AoEB) {
        serialize<int32_t>(SLP);
    }

    serialize<int32_t>(ShapePtr);
    serialize<int32_t>(SoundID);
    serialize<uint8_t, 3>(Colors);

    serialize<int8_t>(IsAnimated);
    serialize<int16_t>(AnimationFrames);
    serialize<int16_t>(PauseFames);
    serialize<float>(Interval);
    serialize<float>(PauseBetweenLoops);
    serialize<int16_t>(Frame);
    serialize<int16_t>(DrawFrame);
    serialize<float>(AnimateLast);
    serialize<int8_t>(FrameChanged);
    serialize<int8_t>(Drawn);

    const size_t bordersSize = gv == GV_MIK ? 13 : 12;
    for (std::vector<FrameData> &sub : Borders) {
        if (isOperation(OP_WRITE) && sub.size() != bordersSize) {
            if (sub.size() > bordersSize) {
                std::cerr << "Too many borders (" << sub.size() << "), max: " << bordersSize;
            }
            sub.resize(bordersSize);
        }
        serialize<FrameData>(sub, bordersSize);
    }

    serialize<int16_t>(DrawTerrain);
    serialize<int16_t>(UnderlayTerrain);

    if (gv != GV_MIK) {
        serialize<int16_t>(BorderStyle);
    }
}
} // namespace genie
