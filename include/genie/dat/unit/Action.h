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

#include <genie/file/ISerializable.h>
#include <genie/dat/UnitCommand.h>

namespace genie {

namespace unit {

class Action : public ISerializable
{
public:
    void setGameVersion(GameVersion gv) override;

    /// Unit task ID executed when idle.
    /// To get the unit auto-converted to enemy,\nuse unit command 107, which sheep and monument have.
    int16_t DefaultTaskID = -1;


    float SearchRadius = 0;
    float WorkRate = 0;

    /// Giving to a villager drop site to cart-like unit
    /// can allow you to have mobile resource-gatherers,\nsimilar to those in Age of Mythology.
    std::pair<int16_t, int16_t> DropSite = { -1, -1 };

    /// If activated unit switches villager types
    /// When tasking the unit, it will transform into another unit,
    /// if the action is not found in this unit, but exists on another unit,
    /// that uses the same task swap group.
    /// Changes according to task
    /// 1   Male villager
    /// 2   Female villager
    /// 3+ Free slots
    int8_t TaskSwapGroup = 0;

    int16_t AttackSound = -1;
    int16_t MoveSound = -1;

    /// Some animals have this.
    enum RunPatterns : int8_t {
        RunStraight = 0,
        RunRandom = 1
    };
    int8_t RunPattern = 0;

    std::vector<Task> TaskList; //only in aoe/ror

protected:
    void serializeObject(void) override;
};

} // namespace unit
} // namespace genie
