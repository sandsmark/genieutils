/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml <email>
    Copyright (C) 2011 - 2014  Mikko T P

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
  Unknown2 = -1;
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
unsigned short TechTree::getCount() // used before 10.84 (GV_AoKB)
{
  if (false) // 10.38 - 10.84
    return 100;
  else
    return 40;
}

//------------------------------------------------------------------------------
void TechTree::serializeObject(void)
{
  serializeSize<uint8_t>(age_count_, TechTreeAges.size());

  serializeSize<uint8_t>(total_building_count_, BuildingConnections.size());

  if (getGameVersion() >= genie::GV_SWGB)
   serializeSize<uint16_t>(total_unit_count_, UnitConnections.size());
  else
  {
    uint8_t tbc = total_unit_count_;
    serializeSize<uint8_t>(tbc, UnitConnections.size());
    total_unit_count_ = tbc;
  }

  serializeSize<uint8_t>(total_research_count_, ResearchConnections.size());

  serialize<int32_t>(Unknown2); // 9.39

  serializeSub<TechTreeAge>(TechTreeAges, age_count_);
  serializeSub<BuildingConnection>(BuildingConnections, total_building_count_);
  serializeSub<UnitConnection>(UnitConnections, total_unit_count_);
  serializeSub<ResearchConnection>(ResearchConnections, total_research_count_);
}

//------------------------------------------------------------------------------
TechTreeAge::TechTreeAge()
{
  ID = 0;
  Unknown2 = 2;
  SlotsUsed = 0;
  Unknown6 = 0;
  LineMode = 0;
}

//------------------------------------------------------------------------------
TechTreeAge::~TechTreeAge()
{
}

void TechTreeAge::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  Common.setGameVersion(gv);
  Unknown4.resize(getU4Size());
  Unknown5.resize(getU4Size());
}

//------------------------------------------------------------------------------
unsigned short TechTreeAge::getU4Size()
{
  if (getGameVersion() >= genie::GV_SWGB)
    return 20;
  else if (getGameVersion() >= genie::GV_AoKB) // 10.38
    return 10;
  else
    return 3;
}

//------------------------------------------------------------------------------
void TechTreeAge::serializeObject(void)
{
  serialize<int32_t>(ID);
  serialize<int8_t>(Unknown2);

  if (getGameVersion() < genie::GV_AoKB)// < 10.84
  {
    serialize<uint8_t>(building_count_);
    serialize<int32_t>(Buildings, TechTree::getCount());

    serialize<uint8_t>(unit_count_);
    serialize<int32_t>(Units, TechTree::getCount());

    serialize<uint8_t>(research_count_);
    serialize<int32_t>(Researches, TechTree::getCount());
  }
  else
  {
    serializeSize<uint8_t>(building_count_, Buildings.size());
    serialize<int32_t>(Buildings, building_count_);

    serializeSize<uint8_t>(unit_count_, Units.size());
    serialize<int32_t>(Units, unit_count_);

    serializeSize<uint8_t>(research_count_, Researches.size());
    serialize<int32_t>(Researches, research_count_);
  }

  serialize<ISerializable>(Common);

  // 9.39
  {
    serialize<int8_t>(SlotsUsed);
    serialize<int8_t>(Unknown4, getU4Size());
    serialize<int8_t>(Unknown5, getU4Size());
    serialize<int8_t>(Unknown6);
    serialize<int32_t>(LineMode); // 9.51
  }
}

//------------------------------------------------------------------------------
BuildingConnection::BuildingConnection() : UnitsTechsTotal(), UnitsTechsFirst()
{
  ID = 0;
  Unknown1 = 2;
  LocationInAge = 0;
  LineMode = 0;
  EnablingResearch = -1;
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
  serialize<int8_t>(Unknown1);

  if (getGameVersion() < genie::GV_AoKB)// < 10.84
  {
    serialize<uint8_t>(building_count_);
    serialize<int32_t>(Buildings, TechTree::getCount());

    serialize<uint8_t>(unit_count_);
    serialize<int32_t>(Units, TechTree::getCount());

    serialize<uint8_t>(research_count_);
    serialize<int32_t>(Researches, TechTree::getCount());
  }
  else
  {
    serializeSize<uint8_t>(building_count_, Buildings.size());
    serialize<int32_t>(Buildings, building_count_);

    serializeSize<uint8_t>(unit_count_, Units.size());
    serialize<int32_t>(Units, unit_count_);

    serializeSize<uint8_t>(research_count_, Researches.size());
    serialize<int32_t>(Researches, research_count_);
  }

  serialize<ISerializable>(Common);

  // 9.39
  {
    serialize<int8_t>(LocationInAge);
    serialize<int8_t, AGES>(UnitsTechsTotal);
    serialize<int8_t, AGES>(UnitsTechsFirst); // 9.42
    serialize<int32_t>(LineMode); // 9.51
    serialize<int32_t>(EnablingResearch); // 9.91
  }
}

//------------------------------------------------------------------------------
UnitConnection::UnitConnection()
{
  ID = 0;
  Unknown1 = 2;
  UpperBuilding = -1;
  VerticalLine = 0;
  LocationInAge = 0;
  RequiredResearch = -1;
  LineMode = 0;
  EnablingResearch = -1;
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
  serialize<int8_t>(Unknown1);
  serialize<int32_t>(UpperBuilding);

  serialize<ISerializable>(Common);

  // 9.39
  {
    serialize<int32_t>(VerticalLine);

    if (getGameVersion() < genie::GV_AoKB)// < 10.84
    {
      serialize<uint8_t>(unit_count_);
      serialize<int32_t>(Units, TechTree::getCount());
    }
    else
    {
      serializeSize<uint8_t>(unit_count_, Units.size());
      serialize<int32_t>(Units, unit_count_);
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
  ID = 0;
  Unknown1 = 2;
  UpperBuilding = -1;
  VerticalLine = 0;
  LocationInAge = 0;
  LineMode = 0;
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
  serialize<int8_t>(Unknown1);
  serialize<int32_t>(UpperBuilding);

  if (getGameVersion() < genie::GV_AoKB)// < 10.84
  {
    serialize<uint8_t>(building_count_);
    serialize<int32_t>(Buildings, TechTree::getCount());

    serialize<uint8_t>(unit_count_);
    serialize<int32_t>(Units, TechTree::getCount());

    serialize<uint8_t>(research_count_);
    serialize<int32_t>(Researches, TechTree::getCount());
  }
  else
  {
    serializeSize<uint8_t>(building_count_, Buildings.size());
    serialize<int32_t>(Buildings, building_count_);

    serializeSize<uint8_t>(unit_count_, Units.size());
    serialize<int32_t>(Units, unit_count_);

    serializeSize<uint8_t>(research_count_, Researches.size());
    serialize<int32_t>(Researches, research_count_);
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
