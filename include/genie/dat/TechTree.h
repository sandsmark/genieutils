/*
    geniedat - A library for reading and writing data files of genie
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

#ifndef GENIE_TECHTREE_H
#define GENIE_TECHTREE_H
#include "genie/file/ISerializable.h"

namespace genie
{

class TechTreeAge;
class BuildingConnection;
class UnitConnection;
class ResearchConnection;

class TechTree : public ISerializable
{
public:
  TechTree();
  virtual ~TechTree();
  virtual void setGameVersion(GameVersion gv);

  std::vector<TechTreeAge> TechTreeAges;

  int32_t Unknown2; //1

  std::vector<BuildingConnection> BuildingConnections;

  std::vector<UnitConnection> UnitConnections;

  std::vector<ResearchConnection> ResearchConnections;

private:
  unsigned char age_count_;
  unsigned char total_building_count_;
  uint16_t total_unit_count_; // int16_t in >= swgb
  unsigned char total_research_count_;

  virtual void serializeObject(void);

};

/// Contains all items of an age in techtree
class TechTreeAge : public ISerializable
{
public:
  TechTreeAge(GameVersion gv = GV_None);
  virtual ~TechTreeAge();
  virtual void setGameVersion(GameVersion gv);

  int32_t Unknown1;
  int32_t ID; //Age ID?
  char Unknown2; //Always 2

  std::vector<int32_t> Buildings;
  std::vector<int32_t> Units;
  std::vector<int32_t> Researches;

  int32_t Unknown3; //Always 1
  int32_t Unknown4; //Second Age ID?

  unsigned short getZeroesSize();

  std::vector<int16_t> Zeroes;
private:
  unsigned char building_count_;
  unsigned char unit_count_;
  unsigned char research_count_;

  virtual void serializeObject(void);
};

class BuildingConnection : public ISerializable
{
public:
  BuildingConnection(GameVersion gv = GV_None);
  virtual ~BuildingConnection();
  virtual void setGameVersion(GameVersion gv);

  int32_t ID;
  char Unknown1; //always 2

  std::vector<int32_t> Buildings;

  std::vector<int32_t> Units;

  std::vector<int32_t> Researches;

  /// Minimum amount of researches that need to be researched for this to be available.
  int32_t RequiredResearches;
  int32_t Age;
  int32_t UnitOrResearch1;
  int32_t UnitOrResearch2;

  unsigned short getUnknown2aSize();
  std::vector<int32_t> Unknown2a;

  /// 0 Nothing, 1 Building, 2 Unit, 3 Research.
  int32_t Mode1;
  int32_t Mode2;

  unsigned short getUnknown2bSize();
  std::vector<int32_t> Unknown2b;

  const uint32_t getUnknown3Size() { return 11; }
  std::vector<char> Unknown3;

  /// 5 One or more connections, 6 No connections.
  int32_t Connections;
  /// Makes available. Used by buildings, which need a research to be available.
  int32_t EnablingResearch;

private:

  unsigned char building_count_;
  unsigned char unit_count_;
  unsigned char research_count_;

  virtual void serializeObject(void);
};

class UnitConnection : public ISerializable
{
public:
  UnitConnection(GameVersion gv = GV_None);
  virtual ~UnitConnection();
  virtual void setGameVersion(GameVersion gv);

  int32_t ID;
  char Unknown1; //always 2
  int32_t UpperBuilding;

  /// Minimum amount of researches that need to be researched for this to be available.
  int32_t RequiredResearches;
  int32_t Age;
  int32_t UnitOrResearch1; // unit or research
  int32_t UnitOrResearch2; // unit or research

  unsigned short getUnknown2aSize();
  std::vector<int32_t> Unknown2a;

  /// 0 Nothing, 1 Building, 2 Unit, 3 Research.
  int32_t Mode1;
  int32_t Mode2;

  unsigned short getUnknown2bSize();
  std::vector<int32_t> Unknown2b;

  int32_t VerticalLine;

  //char *connected unit count*
  std::vector<int32_t> Units;

  /// 1 First, 2 Second.
  int32_t LocationInAge;
  /// Upgrades unit. Used by units, which aren't first in upgrade line.
  int32_t RequiredResearch;
  /// 2 First in vertical line. 3 Not first.
  int32_t LineMode;
  /// Makes available. Used by units, which need a research to be available.
  int32_t EnablingResearch;

private:
  unsigned char unit_count_;

  virtual void serializeObject(void);

};

class ResearchConnection : public ISerializable
{
public:
  ResearchConnection(GameVersion gv = GV_None);
  virtual ~ResearchConnection();
  virtual void setGameVersion(GameVersion gv);

  int32_t ID;
  char Unknown1; //always 2
  int32_t UpperBuilding;

  std::vector<int32_t> Buildings;

  std::vector<int32_t> Units;

  std::vector<int32_t> Researches;

  /// Minimum amount of researches that need to be researched for this to be available.
  int32_t RequiredResearches;
  int32_t Age;
  int32_t UpperResearch;

  unsigned short getUnknown2aSize();
  std::vector<int32_t> Unknown2a;

  /// 0 Independent/new in its line. 3 Depends on a previous research in its line.
  int32_t LineMode;

  unsigned short getUnknown2bSize();
  std::vector<int32_t> Unknown2b;

  int32_t VerticalLine;
  /// 0 Hidden, 1 First, 2 Second.
  int32_t LocationInAge;
  /// 0 First Age. Others.
  int32_t Unknown9;

private:
  unsigned char building_count_;
  unsigned char unit_count_;
  unsigned char research_count_;

  virtual void serializeObject(void);
};

}

#endif // GENIE_TECHTREE_H
