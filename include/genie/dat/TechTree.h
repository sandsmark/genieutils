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

namespace genie {

class TechTreeAge;
class BuildingConnection;
class UnitConnection;
class ResearchConnection;

class TechTree : public ISerializable
{
public:
    void setGameVersion(GameVersion gv) override;

    /// Number of total unit/tech groups
    int32_t TotalUnitTechGroups = 1; //1

    std::vector<TechTreeAge> TechTreeAges;

    std::vector<BuildingConnection> BuildingConnections;

    std::vector<UnitConnection> UnitConnections;

    std::vector<ResearchConnection> ResearchConnections;

    static unsigned short getCount(); //GameVersion gv);

    bool compareTo(const TechTree &other) const;

private:
    void serializeObject() override;
};

namespace techtree {

class Common : public ISerializable
{
public:
    void setGameVersion(GameVersion gv) override
    {
        ISerializable::setGameVersion(gv);
        UnitResearch.resize(getSlots());
        Mode.resize(getSlots());
    }

    int32_t SlotsUsed = 0;

    /// Connection lines when selected
    std::vector<int32_t> UnitResearch;

    /// 0 Age/Tech-level, 1 Building, 2 Unit, 3 Tech.
    std::vector<int32_t> Mode;

    unsigned short getSlots()
    {
        if (getGameVersion() >= genie::GV_SWGB) {
            return 20;
        } else if (getGameVersion() >= genie::GV_AoKB) {
            return 10;
        } else {
            return 5;
        }
    }

    bool compareTo(const Common &other) const;

private:
    void serializeObject() override // 84 bytes, 164 in SWGB
    {
        serialize<int32_t>(SlotsUsed);
        serialize<int32_t>(UnitResearch, getSlots());
        serialize<int32_t>(Mode, getSlots());
    }
};
} // namespace techtree

/// Contains all items of an age in techtree
class TechTreeAge : public ISerializable
{
public:
    void setGameVersion(GameVersion gv) override;

    int32_t ID = -1; //Age ID
    int8_t Status = 2; //Always 2

    std::vector<int32_t> Buildings;
    std::vector<int32_t> Units;
    std::vector<int32_t> Techs;

    techtree::Common Common;
    int8_t NumBuildingLevels = 0;
    std::vector<int8_t> BuildingsPerZone;
    std::vector<int8_t> GroupLengthPerZone;
    int8_t MaxAgeLength = 0;
    int32_t LineMode = 0;

    unsigned short getZoneCount();

    bool compareTo(const TechTreeAge &other) const;

private:
    void serializeObject() override;
};

class BuildingConnection : public ISerializable
{
public:
    BuildingConnection();
    void setGameVersion(GameVersion gv) override;

    int32_t ID = -1;

    /// 0   Nothing
    /// 1   None
    /// 2   Available to player
    /// 3   Not available to player
    /// 4   Researching or constructing or creating
    /// 5   Technology researched or building built
    /// 6   End
    int8_t Status = 2; //always 2

    std::vector<int32_t> Buildings;
    std::vector<int32_t> Units;
    std::vector<int32_t> Techs;

    techtree::Common Common;

    static const unsigned short AGES = 5;

    /// 0   Hidden
    /// 1   First
    /// 2   Second
    int8_t LocationInAge = 0;
    std::vector<int8_t> UnitsTechsTotal;

    std::vector<int8_t> UnitsTechsFirst;

    /// 0   Research: First age
    /// 1   Age
    /// 2   Unit: First in vertical line
    /// 3   Unit: Not first
    /// 4   Research
    /// 5   Building: One or more connections
    /// 6   Building: No connections
    int32_t LineMode = 0;

    /// Makes available. Used by buildings, which need a research to be available.
    int32_t EnablingResearch = 0;

    bool compareTo(const BuildingConnection &other) const;

private:
    void serializeObject() override;
};

class UnitConnection : public ISerializable
{
public:
    void setGameVersion(GameVersion gv) override;

    int32_t ID = -1;
    int8_t Status = 2; //always 2
    int32_t UpperBuilding = -1;

    techtree::Common Common;

    int32_t VerticalLine = -1;

    std::vector<int32_t> Units;

    /// 1 First, 2 Second.
    int32_t LocationInAge = 0;
    /// Upgrades unit. Used by units, which aren't first in upgrade line.
    int32_t RequiredResearch = -1;
    /// 2 First in vertical line. 3 Not first.
    int32_t LineMode = 0;
    /// Makes available. Used by units, which need a research to be available.
    int32_t EnablingResearch = -1;

    bool compareTo(const UnitConnection &other) const;

private:
    void serializeObject() override;
};

class ResearchConnection : public ISerializable
{
public:
    void setGameVersion(GameVersion gv) override;

    int32_t ID = -1;
    int8_t Status = 2; //always 2
    int32_t UpperBuilding = -1;

    std::vector<int32_t> Buildings;
    std::vector<int32_t> Units;
    std::vector<int32_t> Techs;

    techtree::Common Common;

    int32_t VerticalLine = 0;
    /// 0 Hidden, 1 First, 2 Second.
    int32_t LocationInAge = 0;
    /// 0 First Age. Others.
    int32_t LineMode = 0;

    bool compareTo(const ResearchConnection &other) const;

private:
    void serializeObject() override;
};
} // namespace genie

