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

#include "genie/dat/TechTree.h"

namespace genie
{

//------------------------------------------------------------------------------
TechTree::TechTree()
{
}

//------------------------------------------------------------------------------
TechTree::~TechTree()
{
}

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
unsigned short TechTree::getCount()//GameVersion gv) // used before 10.84 (GV_AoKB)
{
  if (false) // 10.38 - <10.84
    return 100;
  else
    return 40;
}

//------------------------------------------------------------------------------
void TechTree::serializeObject(void)
{
  uint8_t age_count;
  uint8_t building_count;
  uint8_t research_count;
  uint16_t unit_count;

  serializeSize<uint8_t>(age_count, TechTreeAges.size());

  serializeSize<uint8_t>(building_count, BuildingConnections.size());

  if (getGameVersion() >= GV_SWGB)
   serializeSize<uint16_t>(unit_count, UnitConnections.size());
  else
  {
    uint8_t tbc = unit_count;
    serializeSize<uint8_t>(tbc, UnitConnections.size());
    unit_count = tbc;
  }

  serializeSize<uint8_t>(research_count, ResearchConnections.size());

  serialize<int32_t>(TotalUnitTechGroups); // 9.39

  serializeSub<TechTreeAge>(TechTreeAges, age_count);
  serializeSub<BuildingConnection>(BuildingConnections, building_count);
  serializeSub<UnitConnection>(UnitConnections, unit_count);
  serializeSub<ResearchConnection>(ResearchConnections, research_count);
}

//------------------------------------------------------------------------------
TechTreeAge::TechTreeAge()
{
}

//------------------------------------------------------------------------------
TechTreeAge::~TechTreeAge()
{
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
  if (getGameVersion() >= GV_SWGB)
    return 20;
  else if (getGameVersion() >= GV_AoKB) // 10.38
    return 10;
  else
    return 3;
}

//------------------------------------------------------------------------------
void TechTreeAge::serializeObject(void)
{
  serialize<int32_t>(ID);
  serialize<int8_t>(Status);

  // All of these need rework. Used + actual? Fix actual!
  uint8_t count;
  if (getGameVersion() < GV_AoKB)// < 10.84
  {
    serialize<uint8_t>(count);
    serialize<int32_t>(Buildings, TechTree::getCount());

    serialize<uint8_t>(count);
    serialize<int32_t>(Units, TechTree::getCount());

    serialize<uint8_t>(count);
    serialize<int32_t>(Techs, TechTree::getCount());
  }
  else
  {
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
BuildingConnection::BuildingConnection() : UnitsTechsTotal(AGES),
  UnitsTechsFirst(AGES)
{
}

//------------------------------------------------------------------------------
BuildingConnection::~BuildingConnection()
{
}

void BuildingConnection::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
  Common.setGameVersion(gv);
}

//------------------------------------------------------------------------------
void BuildingConnection::serializeObject(void)
{
  serialize<int32_t>(ID);
  serialize<int8_t>(Status);

  uint8_t count;
  if (getGameVersion() < GV_AoKB)// < 10.84
  {
    serialize<uint8_t>(count);
    serialize<int32_t>(Buildings, TechTree::getCount());

    serialize<uint8_t>(count);
    serialize<int32_t>(Units, TechTree::getCount());

    serialize<uint8_t>(count);
    serialize<int32_t>(Techs, TechTree::getCount());
  }
  else
  {
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

//------------------------------------------------------------------------------
UnitConnection::UnitConnection()
{
}

//------------------------------------------------------------------------------
UnitConnection::~UnitConnection()
{
}

void UnitConnection::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
  Common.setGameVersion(gv);
}

//------------------------------------------------------------------------------
void UnitConnection::serializeObject(void)
{
  serialize<int32_t>(ID);
  serialize<int8_t>(Status);
  serialize<int32_t>(UpperBuilding);

  serialize<ISerializable>(Common);

  // 9.39
  {
    serialize<int32_t>(VerticalLine);

    uint8_t count;
    if (getGameVersion() < GV_AoKB)// < 10.84
    {
      serialize<uint8_t>(count);
      serialize<int32_t>(Units, TechTree::getCount());
    }
    else
    {
      serializeSize<uint8_t>(count, Units.size());
      serialize<int32_t>(Units, count);
    }

    serialize<int32_t>(LocationInAge); // 9.46
    serialize<int32_t>(RequiredResearch); // 9.5
    serialize<int32_t>(LineMode); // 9.51
    serialize<int32_t>(EnablingResearch); // 9.91
  }
}

//------------------------------------------------------------------------------
ResearchConnection::ResearchConnection()
{
}

//------------------------------------------------------------------------------
ResearchConnection::~ResearchConnection()
{
}

void ResearchConnection::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
  Common.setGameVersion(gv);
}

//------------------------------------------------------------------------------
void ResearchConnection::serializeObject(void)
{
  serialize<int32_t>(ID);
  serialize<int8_t>(Status);
  serialize<int32_t>(UpperBuilding);

  uint8_t count;
  if (getGameVersion() < GV_AoKB)// < 10.84
  {
    serialize<uint8_t>(count);
    serialize<int32_t>(Buildings, TechTree::getCount());

    serialize<uint8_t>(count);
    serialize<int32_t>(Units, TechTree::getCount());

    serialize<uint8_t>(count);
    serialize<int32_t>(Techs, TechTree::getCount());
  }
  else
  {
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

}
