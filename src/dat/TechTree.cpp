/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml <email>
    Copyright (C) 2011 - 2013  Mikko T P

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

  updateGameVersion(BuildingConnections);
  updateGameVersion(UnitConnections);
  updateGameVersion(ResearchConnections);
}

//------------------------------------------------------------------------------
void TechTree::serializeObject(void)
{
  serializeSize<unsigned char>(age_count_, TechTreeAges.size());

  serializeSize<unsigned char>(total_building_count_, BuildingConnections.size());

  if (getGameVersion() >= genie::GV_SWGB)
   serializeSize<uint16_t>(total_unit_count_, UnitConnections.size());
  else
  {
    unsigned char tbc = total_unit_count_;
    serializeSize<unsigned char>(tbc, UnitConnections.size());
    total_unit_count_ = tbc;
  }

  serializeSize<unsigned char>(total_research_count_, ResearchConnections.size());

  serializeSub<TechTreeAge>(TechTreeAges, age_count_);

  serialize<int32_t>(Unknown2);

  serializeSub<BuildingConnection>(BuildingConnections, total_building_count_);
  serializeSub<UnitConnection>(UnitConnections, total_unit_count_);
  serializeSub<ResearchConnection>(ResearchConnections, total_research_count_);
}

//------------------------------------------------------------------------------
TechTreeAge::TechTreeAge(GameVersion gv) : Zeroes(0)
{
  setGameVersion(gv);
  Unknown1 = 0;
  ID = 0;
  Unknown2 = 2;
  Unknown3 = 0;
  Unknown4 = 0;
}

//------------------------------------------------------------------------------
TechTreeAge::~TechTreeAge()
{
}

void TechTreeAge::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  Zeroes.resize(getZeroesSize());
}

//------------------------------------------------------------------------------
unsigned short TechTreeAge::getZeroesSize()
{
  if (getGameVersion() >= genie::GV_SWGB)
    return 99;
  else
    return 49;
}

//------------------------------------------------------------------------------
void TechTreeAge::serializeObject(void)
{
  serialize<int32_t>(Unknown1);
  serialize<int32_t>(ID);
  serialize<char>(Unknown2);

  serializeSize<unsigned char>(building_count_, Buildings.size());
  serialize<int32_t>(Buildings, building_count_);

  serializeSize<unsigned char>(unit_count_, Units.size());
  serialize<int32_t>(Units, unit_count_);

  serializeSize<unsigned char>(research_count_, Researches.size());
  serialize<int32_t>(Researches, research_count_);

  serialize<int32_t>(Unknown3);
  serialize<int32_t>(Unknown4);

  serialize<int16_t>(Zeroes, getZeroesSize());
}

//------------------------------------------------------------------------------
BuildingConnection::BuildingConnection(GameVersion gv) : Unknown2a(0),
Unknown2b(0), Unknown3(getUnknown3Size())
{
  setGameVersion(gv);
  ID = 0;
  Unknown1 = 2;
  RequiredResearches = 0;
  Age = 0;
  UnitOrResearch1 = 0;
  UnitOrResearch2 = 0;
  Mode1 = 0;
  Mode2 = 0;
  Connections = 0;
  EnablingResearch = -1;
}

//------------------------------------------------------------------------------
BuildingConnection::~BuildingConnection()
{
}

void BuildingConnection::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  Unknown2a.resize(getUnknown2aSize());
  Unknown2b.resize(getUnknown2bSize());
}

//------------------------------------------------------------------------------
unsigned short BuildingConnection::getUnknown2aSize()
{
  if (getGameVersion() >= genie::GV_SWGB)
    return 18;
  else
    return 8;
}

//------------------------------------------------------------------------------
unsigned short BuildingConnection::getUnknown2bSize()
{
  if (getGameVersion() >= genie::GV_SWGB)
    return 17;
  else
    return 7;
}

//------------------------------------------------------------------------------
void BuildingConnection::serializeObject(void)
{
  serialize<int32_t>(ID);
  serialize<char>(Unknown1);

  serializeSize<unsigned char>(building_count_, Buildings.size());
  serialize<int32_t>(Buildings, building_count_);

  serializeSize<unsigned char>(unit_count_, Units.size());
  serialize<int32_t>(Units, unit_count_);

  serializeSize<unsigned char>(research_count_, Researches.size());
  serialize<int32_t>(Researches, research_count_);

  serialize<int32_t>(RequiredResearches);
  serialize<int32_t>(Age);
  serialize<int32_t>(UnitOrResearch1);
  serialize<int32_t>(UnitOrResearch2);

  serialize<int32_t>(Unknown2a, getUnknown2aSize());

  serialize<int32_t>(Mode1);
  serialize<int32_t>(Mode2);

  serialize<int32_t>(Unknown2b, getUnknown2bSize());

  serialize<char>(Unknown3, getUnknown3Size());

  serialize<int32_t>(Connections);
  serialize<int32_t>(EnablingResearch);
}

//------------------------------------------------------------------------------
UnitConnection::UnitConnection(GameVersion gv) : Unknown2a(0), Unknown2b(0)
{
  setGameVersion(gv);
  ID = 0;
  Unknown1 = 2;
  UpperBuilding = -1;
  RequiredResearches = 0;
  Age = 0;
  UnitOrResearch1 = 0;
  UnitOrResearch2 = 0;
  Mode1 = 0;
  Mode2 = 0;
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

  Unknown2a.resize(getUnknown2aSize());
  Unknown2b.resize(getUnknown2bSize());
}

//------------------------------------------------------------------------------
unsigned short UnitConnection::getUnknown2aSize()
{
  if (getGameVersion() >= genie::GV_SWGB)
    return 18;
  else
    return 8;
}

//------------------------------------------------------------------------------
unsigned short UnitConnection::getUnknown2bSize()
{
  if (getGameVersion() >= genie::GV_SWGB)
    return 17;
  else
    return 7;
}

//------------------------------------------------------------------------------
void UnitConnection::serializeObject(void)
{
  serialize<int32_t>(ID);
  serialize<char>(Unknown1);
  serialize<int32_t>(UpperBuilding);
  serialize<int32_t>(RequiredResearches);
  serialize<int32_t>(Age);
  serialize<int32_t>(UnitOrResearch1);
  serialize<int32_t>(UnitOrResearch2);

  serialize<int32_t>(Unknown2a, getUnknown2aSize());

  serialize<int32_t>(Mode1);
  serialize<int32_t>(Mode2);

  serialize<int32_t>(Unknown2b, getUnknown2bSize());

  serialize<int32_t>(VerticalLine);

  serializeSize<unsigned char>(unit_count_, Units.size());
  serialize<int32_t>(Units, unit_count_);

  serialize<int32_t>(LocationInAge);
  serialize<int32_t>(RequiredResearch);
  serialize<int32_t>(LineMode);

  serialize<int32_t>(EnablingResearch);
}

//------------------------------------------------------------------------------
ResearchConnection::ResearchConnection(GameVersion gv) : Unknown2a(0), Unknown2b(0)
{
  setGameVersion(gv);
  ID = 0;
  Unknown1 = 2;
  UpperBuilding = -1;
  RequiredResearches = 0;
  Age = 0;
  UpperResearch = -1;
  LineMode = 0;
  VerticalLine = 0;
  LocationInAge = 0;
  Unknown9 = 0;
}

//------------------------------------------------------------------------------
ResearchConnection::~ResearchConnection()
{
}

void ResearchConnection::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  Unknown2a.resize(getUnknown2aSize());
  Unknown2b.resize(getUnknown2bSize());
}

//------------------------------------------------------------------------------
unsigned short ResearchConnection::getUnknown2aSize()
{
  if (getGameVersion() >= genie::GV_SWGB)
    return 19;
  else
    return 9;
}

//------------------------------------------------------------------------------
unsigned short ResearchConnection::getUnknown2bSize()
{
  if (getGameVersion() >= genie::GV_SWGB)
    return 18;
  else
    return 8;
}

//------------------------------------------------------------------------------
void ResearchConnection::serializeObject(void)
{
  serialize<int32_t>(ID);
  serialize<char>(Unknown1);
  serialize<int32_t>(UpperBuilding);

  serializeSize<unsigned char>(building_count_, Buildings.size());
  serialize<int32_t>(Buildings, building_count_);

  serializeSize<unsigned char>(unit_count_, Units.size());
  serialize<int32_t>(Units, unit_count_);

  serializeSize<unsigned char>(research_count_, Researches.size());
  serialize<int32_t>(Researches, research_count_);

  serialize<int32_t>(RequiredResearches);
  serialize<int32_t>(Age);
  serialize<int32_t>(UpperResearch);

  serialize<int32_t>(Unknown2a, getUnknown2aSize());

  serialize<int32_t>(LineMode);

  serialize<int32_t>(Unknown2b, getUnknown2bSize());

  serialize<int32_t>(VerticalLine);
  serialize<int32_t>(LocationInAge);
  serialize<int32_t>(Unknown9);
}

}
