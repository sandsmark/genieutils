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

class Terrain : public SharedTerrain
{
public:
    void setGameVersion(GameVersion gv) override;

    /// If set explicitly with setTerrainCount(), returns that.\n
    /// Otherwise returns defaults:
    ///  - SWGB and above: 55
    ///  - Tapsa tsts: 96
    ///  - Cysion: 100
    ///  - TC: 42
    ///  - Other: 32
    static size_t getTerrainCount(GameVersion gv);

    static void setTerrainCount(unsigned short cnt);

    /// If this is water.
    int8_t IsWater = 0;

    /// If this should not be shown in the map editor
    int8_t HideInEditor = 0;

    /// The string to show in the editor
    int32_t StringID = 0;

    /// Only in versions before SWGB
    int16_t Phantom = 0;

    uint32_t WwiseSoundID = 0; // TODO doc, new in DE
    uint32_t WwiseSoundStopID = 0; // TODO doc, new in DE;

    size_t getNameSize() override;

    /// Priority for blending over neighbouring tiles, not in AoE/RoR.
    int32_t BlendPriority = 0;

    /// The combination of this + the terrain to blend with defines which type of blending, not in aoe/ror.
    int32_t BlendType = 0;

    std::string OverlayMaskName = ""; // TODO doc, new in DE

    std::pair<uint8_t, uint8_t> CliffColors = { 0, 0 };

    /// Is this passable? (I think)
    int8_t PassableTerrain = -1;

    /// Is this impassable? (I think)
    int8_t ImpassableTerrain = -1;

    /// Tile Graphics: flat, 2 x 8 elevation, 2 x 1:1\n
    ///  Frame Count, Animations, Frame Index
    std::array<FrameData, TILE_TYPE_COUNT> ElevationGraphics;

    /// If this is an alias for another terrain that should actually be rendered
    int16_t TerrainToDraw = 0;

    /// Rows, cols
    std::pair<int16_t, int16_t> TerrainDimensions = { 0, 0 };

    /// These refer to terrain borders, which are actually used only in AoE and RoR.
    std::vector<int16_t> Borders;

    static constexpr uint8_t TERRAIN_UNITS_SIZE = 30;
    std::array<int16_t, TERRAIN_UNITS_SIZE> TerrainUnitID{};
    std::array<int16_t, TERRAIN_UNITS_SIZE> TerrainUnitDensity{};

    std::array<int16_t, TERRAIN_UNITS_SIZE> TerrainUnitMaskedDensity{}; // TODO doc, new in DE

    /// If two terrain units are to be placed on same spot, this selects which one will prevail over others.\n
    /// 1 = prevails, others don't.\n
    /// Centralize\n
    /// 0   Place randomly on the tile\n
    /// 1   Place in middle of the tile\n
    std::array<int8_t, TERRAIN_UNITS_SIZE> TerrainUnitCentering{};

    /// Less than TERRAIN_UNITS_SIZE obviously
    int16_t NumberOfTerrainUnitsUsed = 0;

    bool compareTo(const Terrain &other) const;

private:
    static unsigned short terrain_count_;

    void serializeObject() override;
};
} // namespace genie

