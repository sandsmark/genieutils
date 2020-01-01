/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2014 - 2019  Mikko "Tapsa" P

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

#include "genie/dat/TerrainBlock.h"

#include "TestHelpers.h"

namespace genie {

//------------------------------------------------------------------------------
void TerrainBlock::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);

    Terrains.resize(Terrain::getTerrainCount(gv));
    updateGameVersion(Terrains);
    updateGameVersion(TerrainBorders);

    SomeBytes.resize(getBytesSize());
    SomeInt32.resize(getSomethingSize());
}

//------------------------------------------------------------------------------
unsigned short TerrainBlock::getBytesSize(void)
{
    GameVersion gv = getGameVersion();

    if (gv >= GV_SWGB) {
        return 25;
    }

    if (gv >= GV_AoKA) {
        return 21;
    }

    if (gv >= GV_AoKE3) {
        return 17;
    }

    if (gv >= GV_AoEB) {
        return 2; // Padding
    }

    return 0;
}

//------------------------------------------------------------------------------
unsigned short TerrainBlock::getSomethingSize(void)
{
    GameVersion gv = getGameVersion();

    if (gv >= GV_AoK) {
        return 157;
    }

    if (gv >= GV_AoKB) {
        return 84;
    }

    if (gv >= GV_AoKE3) {
        return 6;
    }

    if (gv >= GV_AoEB) {
        return 5; // 5 pointers
    }

    return 65;
}

bool TerrainBlock::compareTo(const TerrainBlock &other) const
{
    COMPARE_MEMBER(MapMaxX);
    COMPARE_MEMBER(MapMaxXplus1);
    COMPARE_MEMBER(MapMaxY);
    COMPARE_MEMBER(MapMaxYplus1);
    COMPARE_MEMBER(MapMinX);
    COMPARE_MEMBER(MapMinY);
    COMPARE_MEMBER(BlockBegCol);
    COMPARE_MEMBER(BlockBegRow);
    COMPARE_MEMBER(BlockEndCol);
    COMPARE_MEMBER(BlockEndRow);
    COMPARE_MEMBER(CurCol);
    COMPARE_MEMBER(CurRow);
    COMPARE_MEMBER(ElevHeight);
    COMPARE_MEMBER(MaxTerrain);
    COMPARE_MEMBER(PaddingTS);
    COMPARE_MEMBER(TileHalfHeight);
    COMPARE_MEMBER(TileHalfWidth);
    COMPARE_MEMBER(TileHeight);
    COMPARE_MEMBER(TileWidth);
    COMPARE_MEMBER(MapHeight);
    COMPARE_MEMBER(MapPointer);
    COMPARE_MEMBER(MapRowOffset);
    COMPARE_MEMBER(MapWidth);
    COMPARE_MEMBER(SearchMapPtr);
    COMPARE_MEMBER(SearchMapRowsPtr);
    COMPARE_MEMBER(VirtualFunctionPtr);
    COMPARE_MEMBER(WorldHeight);
    COMPARE_MEMBER(WorldWidth);
    COMPARE_MEMBER(AnyFrameChange);
    COMPARE_MEMBER(FogFlag);
    COMPARE_MEMBER(MapVisibleFlag);
    COMPARE_MEMBER(BordersUsed);
    COMPARE_MEMBER(RemovedBlocksUsed);
    COMPARE_MEMBER(TerrainsUsed2);
    COMPARE_MEMBER_VEC(Terrains);
    COMPARE_MEMBER_VEC(TerrainBorders);
    COMPARE_MEMBER_VEC(TileSizes);
    COMPARE_MEMBER(AoEAlphaUnknown); // These are very similar to borders
    COMPARE_MEMBER(SomeInt32);
    COMPARE_MEMBER(SomeBytes);

    return true;
}

//------------------------------------------------------------------------------
/// Size in bytes
/// AoE Trial Beta  36264
/// AoE & RoR       36304
/// AoK A & B       36340
/// AoK             37208
/// TC              42208
/// SWGB & CC       49640
void TerrainBlock::serializeObject(void)
{
    GameVersion gv = getGameVersion();

    serialize<int32_t>(VirtualFunctionPtr);// __vfptr
    serialize<int32_t>(MapPointer);
    serialize<int32_t>(MapWidth);
    serialize<int32_t>(MapHeight);
    serialize<int32_t>(WorldWidth);
    serialize<int32_t>(WorldHeight);

    serialize<TileSize, SharedTerrain::TILE_TYPE_COUNT>(TileSizes);

    if (gv >= GV_AoE) {
        serialize<int16_t>(PaddingTS); // Padding for TileSizes (32-bit aligned)
    }

    if (Terrains.size() != Terrain::getTerrainCount(gv)) {
        std::cerr << "Unexpected terrain count, " << Terrains.size() << " for this game type " << Terrain::getTerrainCount(gv) << " is expected" << std::endl;
    }

    serialize(Terrains, Terrain::getTerrainCount(gv));

    std::cout << "Terrains: " << Terrains.size() << std::endl;

    if (gv < GV_AoEB) {
        serialize<int16_t>(AoEAlphaUnknown, (16 * 1888) / 2);
    }

    if (gv < GV_C9 || gv > GV_LatestDE2) {
        // TerrainBorders seem to be unused (are empty) in GV > AoK Alpha
        serialize<TerrainBorder, 16>(TerrainBorders);

        // Probably filled after loading map in game.
        serialize<int32_t>(MapRowOffset);
    }

    if (gv >= GV_AoKA) {
        serialize<float>(MapMinX);
        serialize<float>(MapMinY);
        serialize<float>(MapMaxX);
        serialize<float>(MapMaxY);

        if (gv >= GV_AoK) {
            serialize<float>(MapMaxXplus1);
            serialize<float>(MapMaxYplus1);
        }
    }

    serialize<uint16_t>(TerrainsUsed2);

    if (gv < GV_AoEB) {
        serialize<uint16_t>(RemovedBlocksUsed);
    }

    serialize<uint16_t>(BordersUsed);
    serialize<int16_t>(MaxTerrain);
    serialize<int16_t>(TileWidth);
    serialize<int16_t>(TileHeight);
    serialize<int16_t>(TileHalfHeight);
    serialize<int16_t>(TileHalfWidth);
    serialize<int16_t>(ElevHeight);
    serialize<int16_t>(CurRow);
    serialize<int16_t>(CurCol);
    serialize<int16_t>(BlockBegRow);
    serialize<int16_t>(BlockEndRow);
    serialize<int16_t>(BlockBegCol);
    serialize<int16_t>(BlockEndCol);

    if (gv >= GV_AoKE3) {
        serialize<int32_t>(SearchMapPtr);
        serialize<int32_t>(SearchMapRowsPtr);
        serialize<int8_t>(AnyFrameChange);
    } else {
        // Padding fix
        int32_t any = AnyFrameChange;
        serialize<int32_t>(any);
        AnyFrameChange = any;

        serialize<int32_t>(SearchMapPtr);
        serialize<int32_t>(SearchMapRowsPtr);
    }

    serialize<int8_t>(MapVisibleFlag);
    serialize<int8_t>(FogFlag); // Always 1

    if (gv < GV_C9 || gv > GV_LatestDE2) {
        //From here on data is filled in game anyway.
        //There are two 32-bit pointers random map and game world, rest should be all 0.
        serialize<int8_t>(SomeBytes, getBytesSize());

        // Few pointers and small numbers.
        serialize<int32_t>(SomeInt32, getSomethingSize());
    }
}

//------------------------------------------------------------------------------
void TileSize::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);
}

bool TileSize::compareTo(const TileSize &other) const
{
    COMPARE_MEMBER(Width);
    COMPARE_MEMBER(Height);
    COMPARE_MEMBER(DeltaY);

    return true;
}

//------------------------------------------------------------------------------
void TileSize::serializeObject(void)
{
    serialize<int16_t>(Width);
    serialize<int16_t>(Height);

    if (getGameVersion() >= GV_AoE) {
        serialize<int16_t>(DeltaY);
    }
}
} // namespace genie
