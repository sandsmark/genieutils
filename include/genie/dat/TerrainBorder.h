/*
    geniedat - A library for reading and writing data files of genie
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

#pragma once
#include "genie/file/ISerializable.h"
#include "genie/dat/TerrainCommon.h"

namespace genie {

/// Seems to be unused in GV > RoR
class TerrainBorder : public SharedTerrain
{
public:
    TerrainBorder();

    size_t getNameSize() override;

    /// Information about how to render all borders
    std::array<std::vector<FrameData>, TILE_TYPE_COUNT> Borders{};

    /// always 0
    int16_t DrawTerrain = 0;

    /// Used for terrain passability check
    int16_t UnderlayTerrain = -1;

    /// This has something to do with all 19 tile types. Mickey's DAT had this removed and instead added one border shape to each 19 tile types.
    int16_t BorderStyle = 0;

    bool compareTo(const TerrainBorder &other) const;

private:
    void serializeObject() override;
};
} // namespace genie

