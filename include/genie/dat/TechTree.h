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

  int32_t Unknown2; //1

  std::vector<TechTreeAge> TechTreeAges;

  std::vector<BuildingConnection> BuildingConnections;

  std::vector<UnitConnection> UnitConnections;

  std::vector<ResearchConnection> ResearchConnections;

private:
  uint8_t age_count_;
  uint8_t total_building_count_;
  uint16_t total_unit_count_; // int16_t in >= swgb
  uint8_t total_research_count_;

  virtual void serializeObject(void);

};

namespace techtree
{

class Common : public ISerializable
{
public:
  Common()
  {
    RequiredResearches = 0;
    Age = 0;
    UnitResearch1 = 0;
    UnitResearch2 = 0;
    Mode1 = 0;
    Mode2 = 0;
  }

  virtual ~Common() {}

  virtual void setGameVersion(GameVersion gv)
  {
    ISerializable::setGameVersion(gv);
    Unknown1.resize(getU1Size());
    Unknown2.resize(getU2Size());
  }

  /// Minimum amount of researches that need to be researched for this to be available.
  int32_t RequiredResearches;

  int32_t Age;
  /// Connection lines when selected
  int32_t UnitResearch1;
  int32_t UnitResearch2;
  std::vector<int32_t> Unknown1;

  /// 0 Nothing, 1 Building, 2 Unit, 3 Research.
  int32_t Mode1;
  int32_t Mode2;
  std::vector<int32_t> Unknown2;

  unsigned short getU1Size()
  {
    if (getGameVersion() >= genie::GV_SWGB)
      return 18;
    else
      return 8;
  }

  unsigned short getU2Size()
  {
    if (getGameVersion() >= genie::GV_SWGB)
      return 17;
    else
      return 7;
  }

  std::vector<int32_t> Unknown2a;

private:
  virtual void serializeObject(void) // 84 bytes, 164 in SWGB
  {
	serialize<int32_t>(RequiredResearches);
	serialize<int32_t>(Age);
	serialize<int32_t>(UnitResearch1);
	serialize<int32_t>(UnitResearch2);
	serialize<int32_t>(Unknown1, getU1Size()); // 8 tai 18
	serialize<int32_t>(Mode1); // LineMode
	serialize<int32_t>(Mode2);
	serialize<int32_t>(Unknown2, getU2Size()); // 7 tai 17
  }
};

}

/// Contains all items of an age in techtree
class TechTreeAge : public ISerializable
{
public:
  TechTreeAge();
  virtual ~TechTreeAge();
  virtual void setGameVersion(GameVersion gv);

  int32_t ID; //Age ID?
  int8_t Unknown2; //Always 2

  std::vector<int32_t> Buildings;
  std::vector<int32_t> Units;
  std::vector<int32_t> Researches;

  techtree::Common Common;
  int8_t Unknown3;
  std::vector<int8_t> Unknown4;
  std::vector<int8_t> Unknown5;
  int8_t Unknown6;
  int32_t Unknown7;

  unsigned short getU4Size();

private:
  uint8_t building_count_;
  uint8_t unit_count_;
  uint8_t research_count_;

  virtual void serializeObject(void);
};

class BuildingConnection : public ISerializable
{
public:
  BuildingConnection();
  virtual ~BuildingConnection();
  virtual void setGameVersion(GameVersion gv);

  int32_t ID;
  int8_t Unknown1; //always 2

  std::vector<int32_t> Buildings;
  std::vector<int32_t> Units;
  std::vector<int32_t> Researches;

  techtree::Common Common;

  static const unsigned short AGES = 5;
  int8_t Unknown2;
  std::array<int8_t, AGES> UnitsTechsTotal;
  std::array<int8_t, AGES> UnitsTechsFirst;

  /// 5 One or more connections, 6 No connections.
  int32_t Connections;
  /// Makes available. Used by buildings, which need a research to be available.
  int32_t EnablingResearch;

private:
  uint8_t building_count_;
  uint8_t unit_count_;
  uint8_t research_count_;

  virtual void serializeObject(void);
};

class UnitConnection : public ISerializable
{
public:
  UnitConnection();
  virtual ~UnitConnection();
  virtual void setGameVersion(GameVersion gv);

  int32_t ID;
  int8_t Unknown1; //always 2
  int32_t UpperBuilding;

  techtree::Common Common;

  int32_t VerticalLine;

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
  uint8_t unit_count_;

  virtual void serializeObject(void);

};

class ResearchConnection : public ISerializable
{
public:
  ResearchConnection();
  virtual ~ResearchConnection();
  virtual void setGameVersion(GameVersion gv);

  int32_t ID;
  int8_t Unknown1; //always 2
  int32_t UpperBuilding;

  std::vector<int32_t> Buildings;
  std::vector<int32_t> Units;
  std::vector<int32_t> Researches;

  techtree::Common Common;

  int32_t VerticalLine;
  /// 0 Hidden, 1 First, 2 Second.
  int32_t LocationInAge;
  /// 0 First Age. Others.
  int32_t FirstAgeMode;

private:
  uint8_t building_count_;
  uint8_t unit_count_;
  uint8_t research_count_;

  virtual void serializeObject(void);
};

}

#endif // GENIE_TECHTREE_H
