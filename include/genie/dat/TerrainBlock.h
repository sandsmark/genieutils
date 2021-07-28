/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2014 - 2015  Mikko "Tapsa" P

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
#include "genie/dat/Terrain.h"
#include "genie/dat/TerrainBorder.h"

namespace genie {

class TileSize : public ISerializable
{
public:
    void setGameVersion(GameVersion gv) override;

    int16_t Width = 0;
    int16_t Height = 0;
    int16_t DeltaY = 0;

    bool compareTo(const TileSize &other) const;

private:
    void serializeObject() override;
};


/// Most here seems to be unused in AoK at least, so I haven't bothered documenting it.
///
/// Size in bytes:
///  - AoE Trial Beta  36264
///  - AoE & RoR       36304
///  - AoK A & B       36340
///  - AoK             37208
///  - TC              42208
///  - SWGB & CC       49640
class TerrainBlock : public ISerializable
{
public:
    void setGameVersion(GameVersion gv) override;

    /// Unused, because the original Genie wrote their entire class structure to disk, including vptrs
    int32_t VirtualFunctionPtr = 0;

    /// Unused, same as above
    int32_t MapPointer = 0;

    int32_t MapWidth = 0;
    int32_t MapHeight = 0;

    int32_t WorldWidth = 0;
    int32_t WorldHeight = 0;
    int16_t PaddingTS = 0;

    std::array<TileSize, SharedTerrain::TILE_TYPE_COUNT> TileSizes;
    std::vector<Terrain> Terrains;

    /// These are very similar to borders
    /// Only in AoE1 alpha, hardcoded to (16 * 1888) / 2 in genieutils:w
    std::vector<int16_t> AoEAlphaUnknown;

    /// TerrainBorders seem to be unused (are empty) in GV > AoK Alpha
    std::array<TerrainBorder, 16> TerrainBorders;

    int32_t MapRowOffset = 0;
    float MapMinX = 0.f;
    float MapMinY = 0.f;
    float MapMaxX = 0.f;
    float MapMaxY = 0.f;
    float MapMaxXplus1 = 0.f;
    float MapMaxYplus1 = 0.f;

    uint16_t TerrainsUsed2 = 0;
    uint16_t RemovedBlocksUsed = 0;
    uint16_t BordersUsed = 0;
    int16_t MaxTerrain = 0;
    int16_t TileWidth = 0;
    int16_t TileHeight = 0;
    int16_t TileHalfHeight = 0;
    int16_t TileHalfWidth = 0;
    int16_t ElevHeight = 0;
    int16_t CurRow = 0;
    int16_t CurCol = 0;
    int16_t BlockBegRow = 0;
    int16_t BlockEndRow = 0;
    int16_t BlockBegCol = 0;
    int16_t BlockEndCol = 0;

    int32_t SearchMapPtr = 0;
    int32_t SearchMapRowsPtr = 0;
    int8_t AnyFrameChange = 0;

    int8_t MapVisibleFlag = 0;
    int8_t FogFlag = 0;

    unsigned short getBytesSize();
    std::vector<int8_t> SomeBytes;
    unsigned short getSomethingSize();
    std::vector<int32_t> SomeInt32;

    bool compareTo(const TerrainBlock &other) const;

private:
    void serializeObject() override;
};
} // namespace genie

