/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
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

#include "genie/dat/RandomMap.h"

#include "TestHelpers.h"

namespace genie {

//------------------------------------------------------------------------------
void RandomMaps::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);

    updateGameVersion(Maps);
}

bool RandomMaps::compareTo(const RandomMaps &other) const
{
    COMPARE_MEMBER(RandomMapsPtr);
    COMPARE_MEMBER_VEC(Maps);


    return true;
}

//------------------------------------------------------------------------------
void RandomMaps::serializeObject()
{
    uint32_t random_map_count{};

    if (getGameVersion() < GV_AoEB) {
        serializeSize<uint32_t>(random_map_count, OldMaps.size());

        if (isOperation(OP_READ)) {
            OldMaps.resize(random_map_count);
        }

        for (std::vector<int32_t> &sub : OldMaps) {
            serialize<int32_t>(sub, 852);
        }
    } else {
        serializeSize<uint32_t>(random_map_count, Maps.size());

        serialize<int32_t>(RandomMapsPtr);

        serialize(Maps, random_map_count);
        serialize(Maps, random_map_count);
    }
}

//------------------------------------------------------------------------------
void MapInfo::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);

    updateGameVersion(MapLands);
    updateGameVersion(MapTerrains);
    updateGameVersion(MapUnits);
    updateGameVersion(MapElevations);
}

bool MapInfo::compareTo(const MapInfo &other) const
{
    COMPARE_MEMBER(MapID);
    COMPARE_MEMBER(BorderSouthWest);
    COMPARE_MEMBER(BorderNorthWest);
    COMPARE_MEMBER(BorderNorthEast);
    COMPARE_MEMBER(BorderSouthEast);
    COMPARE_MEMBER(BorderUsage);
    COMPARE_MEMBER(WaterShape);
    COMPARE_MEMBER(BaseTerrain);
    COMPARE_MEMBER(LandCoverage);
    COMPARE_MEMBER(UnusedID);
    COMPARE_MEMBER(MapLandsPtr);
    COMPARE_MEMBER(MapTerrainsPtr);
    COMPARE_MEMBER(MapUnitsPtr);
    COMPARE_MEMBER(MapElevationsPtr);

    COMPARE_MEMBER_VEC(MapLands);
    COMPARE_MEMBER_VEC(MapTerrains);
    COMPARE_MEMBER_VEC(MapUnits);
    COMPARE_MEMBER_VEC(MapElevations);

    return true;
}

//------------------------------------------------------------------------------
void MapInfo::serializeObject(void)
{
    if (!io_all_) {
        serialize<int32_t>(MapID);
    }

    // Yes. These are read and written twice.

    serialize<int32_t>(BorderSouthWest);
    serialize<int32_t>(BorderNorthWest);
    serialize<int32_t>(BorderNorthEast);
    serialize<int32_t>(BorderSouthEast);
    serialize<int32_t>(BorderUsage);
    serialize<int32_t>(WaterShape);
    serialize<int32_t>(BaseTerrain);
    serialize<int32_t>(LandCoverage);
    serialize<int32_t>(UnusedID);

    uint32_t count = 0;
    serializeSize<uint32_t>(count, MapLands.size());
    serialize<int32_t>(MapLandsPtr);

    if (io_all_) {
        serialize(MapLands, count);
    }

    serializeSize<uint32_t>(count, MapTerrains.size());
    serialize<int32_t>(MapTerrainsPtr);

    if (io_all_) {
        serialize(MapTerrains, count);
    }

    serializeSize<uint32_t>(count, MapUnits.size());
    serialize<int32_t>(MapUnitsPtr);

    if (io_all_) {
        serialize(MapUnits, count);
    }

    serializeSize<uint32_t>(count, MapElevations.size());
    serialize<int32_t>(MapElevationsPtr);

    if (io_all_) {
        serialize(MapElevations, count);
    }

    io_all_ = !io_all_;
}

//------------------------------------------------------------------------------
void MapLand::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);
}

bool MapLand::compareTo(const MapLand &other) const
{
    COMPARE_MEMBER(Padding1);
    COMPARE_MEMBER(Padding2);
    COMPARE_MEMBER(BaseSize);
    COMPARE_MEMBER(BaseX);
    COMPARE_MEMBER(BaseY);
    COMPARE_MEMBER(Clumpiness);
    COMPARE_MEMBER(LandID);
    COMPARE_MEMBER(LandSpacing);
    COMPARE_MEMBER(StartAreaRadius);
    COMPARE_MEMBER(Terrain);
    COMPARE_MEMBER(TerrainEdgeFade);
    COMPARE_MEMBER(ByPlayerFlag);
    COMPARE_MEMBER(LandProportion);
    COMPARE_MEMBER(PlacementType);
    COMPARE_MEMBER(Zone);

    return true;
}

//------------------------------------------------------------------------------
void MapLand::serializeObject(void)
{
    serialize<int32_t>(LandID);
    serialize<int32_t>(Terrain); //int8_t
    serialize<int32_t>(LandSpacing);
    serialize<int32_t>(BaseSize);
    serialize<int8_t>(Zone);
    serialize<int8_t>(PlacementType);
    serialize<int16_t>(Padding1);
    serialize<int32_t>(BaseX);
    serialize<int32_t>(BaseY);
    serialize<int8_t>(LandProportion);
    serialize<int8_t>(ByPlayerFlag);
    serialize<int16_t>(Padding2);
    serialize<int32_t>(StartAreaRadius);
    serialize<int32_t>(TerrainEdgeFade);
    serialize<int32_t>(Clumpiness);
}

//------------------------------------------------------------------------------
void MapTerrain::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);
}

bool MapTerrain::compareTo(const MapTerrain &other) const
{
    COMPARE_MEMBER(Proportion);
    COMPARE_MEMBER(Terrain);
    COMPARE_MEMBER(ClumpCount);
    COMPARE_MEMBER(EdgeSpacing);
    COMPARE_MEMBER(PlacementTerrain);
    COMPARE_MEMBER(Clumpiness);

    return true;
}

//------------------------------------------------------------------------------
void MapTerrain::serializeObject(void)
{
    serialize<int32_t>(Proportion);
    serialize<int32_t>(Terrain);
    serialize<int32_t>(ClumpCount);
    serialize<int32_t>(EdgeSpacing);
    serialize<int32_t>(PlacementTerrain);
    serialize<int32_t>(Clumpiness);
}

//------------------------------------------------------------------------------
void MapUnit::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);
}

bool MapUnit::compareTo(const MapUnit &other) const
{
    COMPARE_MEMBER(Padding1);
    COMPARE_MEMBER(Fluctuation);
    COMPARE_MEMBER(GroupArea);
    COMPARE_MEMBER(GroupsPerPlayer);
    COMPARE_MEMBER(HostTerrain);
    COMPARE_MEMBER(MaxDistanceToPlayers);
    COMPARE_MEMBER(MinDistanceToPlayers);
    COMPARE_MEMBER(ObjectsPerGroup);
    COMPARE_MEMBER(PlayerID);
    COMPARE_MEMBER(SetPlaceForAllPlayers);
    COMPARE_MEMBER(Unit);
    COMPARE_MEMBER(GroupPlacing);
    COMPARE_MEMBER(ScaleFlag);

    return true;
}

//------------------------------------------------------------------------------
void MapUnit::serializeObject(void)
{
    serialize<int32_t>(Unit);
    serialize<int32_t>(HostTerrain);
    serialize<int8_t>(GroupPlacing);
    serialize<int8_t>(ScaleFlag);
    serialize<int16_t>(Padding1);
    serialize<int32_t>(ObjectsPerGroup);
    serialize<int32_t>(Fluctuation);
    serialize<int32_t>(GroupsPerPlayer);
    serialize<int32_t>(GroupArea);
    serialize<int32_t>(PlayerID);
    serialize<int32_t>(SetPlaceForAllPlayers);
    serialize<int32_t>(MinDistanceToPlayers);
    serialize<int32_t>(MaxDistanceToPlayers);
}

//------------------------------------------------------------------------------
void MapElevation::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);
}

bool MapElevation::compareTo(const MapElevation &other) const
{
    COMPARE_MEMBER(Proportion);
    COMPARE_MEMBER(Terrain);
    COMPARE_MEMBER(ClumpCount);
    COMPARE_MEMBER(BaseTerrain);
    COMPARE_MEMBER(BaseElevation);
    COMPARE_MEMBER(TileSpacing);

    return true;
}

//------------------------------------------------------------------------------
void MapElevation::serializeObject(void)
{
    serialize<int32_t>(Proportion);
    serialize<int32_t>(Terrain);
    serialize<int32_t>(ClumpCount);
    serialize<int32_t>(BaseTerrain);
    serialize<int32_t>(BaseElevation);
    serialize<int32_t>(TileSpacing);
}
} // namespace genie
