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
#include <vector>
#include "genie/file/ISerializable.h"
#include "TerrainPassGraphic.h"

namespace genie {

class TerrainRestriction : public ISerializable
{
public:
    TerrainRestriction();
    void setGameVersion(GameVersion gv) override;

    /// Accessibility and Damage Multiplier
    /// See unit -> terrain defense bonus\nPass-ability:
    ///  0  Not passable
    /// > 0  Passable
    /// Build-ability:
    /// <= 0.05  You cannot build on it.
    /// > 0.05  You can build on it.
    /// Damage Multiplier:
    /// 0 Damage multiplier is 1.
    ///  > 0  Damage multipler is as specified.
    /// Star Wars: < 1  Damage multiplier is 1.
    std::vector<float> PassableBuildableDmgMultiplier;

    std::vector<TerrainPassGraphic> TerrainPassGraphics;

    static void setTerrainCount(unsigned short cnt);

    bool compareTo(const TerrainRestriction &other) const;

private:
    static unsigned short terrain_count_;

    void serializeObject() override;
};
} // namespace genie
