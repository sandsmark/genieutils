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

#include "genie/dat/TechTree.h"

#include "TestHelpers.h"

namespace genie {

//------------------------------------------------------------------------------
void TechTree::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);

    updateGameVersion(TechTreeAges);
    updateGameVersion(BuildingConnections);
    updateGameVersion(UnitConnections);
    updateGameVersion(ResearchConnections);
}

//------------------------------------------------------------------------------
unsigned short TechTree::getCount() //GameVersion gv) // used before 10.84 (GV_AoKB)
{
    if (false) { // 10.38 - <10.84
        return 100;
    } else {
        return 40;
    }
}

bool TechTree::compareTo(const TechTree &other) const
{
    COMPARE_MEMBER(TotalUnitTechGroups);
    COMPARE_MEMBER_OBJ_VEC(TechTreeAges);
    COMPARE_MEMBER_OBJ_VEC(UnitConnections);
    COMPARE_MEMBER_OBJ_VEC(BuildingConnections);
    COMPARE_MEMBER_OBJ_VEC(ResearchConnections);

    return true;
}

//------------------------------------------------------------------------------
void TechTree::serializeObject()
{
    uint8_t age_count = 0;
    uint8_t building_count = 0;
    uint8_t research_count = 0;
    uint16_t unit_count = 0;

    serializeSize<uint8_t>(age_count, TechTreeAges.size());

    serializeSize<uint8_t>(building_count, BuildingConnections.size());

    if (getGameVersion() >= GV_SWGB) {
        serializeSize<uint16_t>(unit_count, UnitConnections.size());
    } else {
        uint8_t tbc = 0;
        serializeSize<uint8_t>(tbc, UnitConnections.size());
        unit_count = tbc;
    }

    serializeSize<uint8_t>(research_count, ResearchConnections.size());

    serialize<int32_t>(TotalUnitTechGroups); // 9.39

    serialize(TechTreeAges, age_count);
    serialize(BuildingConnections, building_count);
    serialize(UnitConnections, unit_count);
    serialize(ResearchConnections, research_count);
}

void TechTreeAge::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);

    Common.setGameVersion(gv);
    BuildingsPerZone.resize(getZoneCount());
    GroupLengthPerZone.resize(getZoneCount());
}

//------------------------------------------------------------------------------
unsigned short TechTreeAge::getZoneCount()
{
    if (getGameVersion() >= GV_SWGB) {
        return 20;
    } else if (getGameVersion() >= GV_AoKB) { // 10.38
        return 10;
    } else {
        return 3;
    }
}

bool TechTreeAge::compareTo(const TechTreeAge &other) const
{
    COMPARE_MEMBER(ID);
    COMPARE_MEMBER(LineMode);
    COMPARE_MEMBER(MaxAgeLength);
    COMPARE_MEMBER(NumBuildingLevels);
    COMPARE_MEMBER(Status);
    COMPARE_MEMBER_VEC(Buildings);
    COMPARE_MEMBER_VEC(Techs);
    COMPARE_MEMBER_VEC(Units);
    COMPARE_MEMBER_VEC(BuildingsPerZone);
    COMPARE_MEMBER_VEC(GroupLengthPerZone);

    return Common.compareTo(other.Common);
}

//------------------------------------------------------------------------------
void TechTreeAge::serializeObject()
{
    serialize<int32_t>(ID);
    serialize<int8_t>(Status);

    // All of these need rework. Used + actual? Fix actual!
    uint8_t count{};

    if (getGameVersion() < GV_AoKB) { // < 10.84
        serialize<uint8_t>(count);
        serialize<int32_t>(Buildings, TechTree::getCount());

        serialize<uint8_t>(count);
        serialize<int32_t>(Units, TechTree::getCount());

        serialize<uint8_t>(count);
        serialize<int32_t>(Techs, TechTree::getCount());
    } else {
        serializeSize<uint8_t>(count, Buildings.size());
        serialize<int32_t>(Buildings, count);

        serializeSize<uint8_t>(count, Units.size());
        serialize<int32_t>(Units, count);

        serializeSize<uint8_t>(count, Techs.size());
        serialize<int32_t>(Techs, count);
    }

    serialize<ISerializable>(Common);

    // 9.39
    {
        serialize<int8_t>(NumBuildingLevels);
        serialize<int8_t>(BuildingsPerZone, getZoneCount());
        serialize<int8_t>(GroupLengthPerZone, getZoneCount());
        serialize<int8_t>(MaxAgeLength);
        serialize<int32_t>(LineMode); // 9.51
    }
}

//------------------------------------------------------------------------------
BuildingConnection::BuildingConnection() :
    UnitsTechsTotal(AGES),
    UnitsTechsFirst(AGES)
{
}

void BuildingConnection::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);
    Common.setGameVersion(gv);
}

bool BuildingConnection::compareTo(const BuildingConnection &other) const
{
    COMPARE_MEMBER(EnablingResearch);
    COMPARE_MEMBER(ID);
    COMPARE_MEMBER(LineMode);
    COMPARE_MEMBER(LocationInAge);
    COMPARE_MEMBER(Status);
    COMPARE_MEMBER_VEC(Buildings);
    COMPARE_MEMBER_VEC(Techs);
    COMPARE_MEMBER_VEC(Units);
    COMPARE_MEMBER_VEC(UnitsTechsFirst);
    COMPARE_MEMBER_VEC(UnitsTechsTotal);

    return Common.compareTo(other.Common);
}

//------------------------------------------------------------------------------
void BuildingConnection::serializeObject()
{
    serialize<int32_t>(ID);
    serialize<int8_t>(Status);

    uint8_t count{};

    if (getGameVersion() < GV_AoKB) { // < 10.84
        serialize<uint8_t>(count);
        serialize<int32_t>(Buildings, TechTree::getCount());

        serialize<uint8_t>(count);
        serialize<int32_t>(Units, TechTree::getCount());

        serialize<uint8_t>(count);
        serialize<int32_t>(Techs, TechTree::getCount());
    } else {
        serializeSize<uint8_t>(count, Buildings.size());
        serialize<int32_t>(Buildings, count);

        serializeSize<uint8_t>(count, Units.size());
        serialize<int32_t>(Units, count);

        serializeSize<uint8_t>(count, Techs.size());
        serialize<int32_t>(Techs, count);
    }

    serialize<ISerializable>(Common);

    // 9.39
    {
        serialize<int8_t>(LocationInAge);
        serialize<int8_t>(UnitsTechsTotal, AGES);
        serialize<int8_t>(UnitsTechsFirst, AGES); // 9.42
        serialize<int32_t>(LineMode); // 9.51
        serialize<int32_t>(EnablingResearch); // 9.91
    }
}

void UnitConnection::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);
    Common.setGameVersion(gv);
}

bool UnitConnection::compareTo(const UnitConnection &other) const
{
    COMPARE_MEMBER(EnablingResearch);
    COMPARE_MEMBER(ID);
    COMPARE_MEMBER(LineMode);
    COMPARE_MEMBER(LocationInAge);
    COMPARE_MEMBER(RequiredResearch);
    COMPARE_MEMBER(UpperBuilding);
    COMPARE_MEMBER(VerticalLine);
    COMPARE_MEMBER(Status);
    COMPARE_MEMBER_VEC(Units);

    return Common.compareTo(other.Common);
}

//------------------------------------------------------------------------------
void UnitConnection::serializeObject()
{
    serialize<int32_t>(ID);
    serialize<int8_t>(Status);
    serialize<int32_t>(UpperBuilding);

    serialize<ISerializable>(Common);

    // 9.39
    {
        serialize<int32_t>(VerticalLine);

        uint8_t count{};

        if (getGameVersion() < GV_AoKB) { // < 10.84
            serialize<uint8_t>(count);
            serialize<int32_t>(Units, TechTree::getCount());
        } else {
            serializeSize<uint8_t>(count, Units.size());
            serialize<int32_t>(Units, count);
        }

        serialize<int32_t>(LocationInAge); // 9.46
        serialize<int32_t>(RequiredResearch); // 9.5
        serialize<int32_t>(LineMode); // 9.51
        serialize<int32_t>(EnablingResearch); // 9.91
    }
}

void ResearchConnection::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);
    Common.setGameVersion(gv);
}

bool ResearchConnection::compareTo(const ResearchConnection &other) const
{
    COMPARE_MEMBER(ID);
    COMPARE_MEMBER(LineMode);
    COMPARE_MEMBER(LocationInAge);
    COMPARE_MEMBER(UpperBuilding);
    COMPARE_MEMBER(VerticalLine);
    COMPARE_MEMBER(Status);
    COMPARE_MEMBER_VEC(Buildings);
    COMPARE_MEMBER_VEC(Techs);
    COMPARE_MEMBER_VEC(Units);

    return Common.compareTo(other.Common);
}

//------------------------------------------------------------------------------
void ResearchConnection::serializeObject()
{
    serialize<int32_t>(ID);
    serialize<int8_t>(Status);
    serialize<int32_t>(UpperBuilding);

    uint8_t count{};

    if (getGameVersion() < GV_AoKB) { // < 10.84
        serialize<uint8_t>(count);
        serialize<int32_t>(Buildings, TechTree::getCount());

        serialize<uint8_t>(count);
        serialize<int32_t>(Units, TechTree::getCount());

        serialize<uint8_t>(count);
        serialize<int32_t>(Techs, TechTree::getCount());
    } else {
        serializeSize<uint8_t>(count, Buildings.size());
        serialize<int32_t>(Buildings, count);

        serializeSize<uint8_t>(count, Units.size());
        serialize<int32_t>(Units, count);

        serializeSize<uint8_t>(count, Techs.size());
        serialize<int32_t>(Techs, count);
    }

    serialize<ISerializable>(Common);

    // 9.39
    {
        serialize<int32_t>(VerticalLine);
        serialize<int32_t>(LocationInAge); // 9.46
        serialize<int32_t>(LineMode); // 9.51
    }
}

bool techtree::Common::compareTo(const techtree::Common &other) const
{
    COMPARE_MEMBER(SlotsUsed);
    COMPARE_MEMBER_VEC(UnitResearch);
    COMPARE_MEMBER_VEC(Mode);

    return true;
}

} // namespace genie
