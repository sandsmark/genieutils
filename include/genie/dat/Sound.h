/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2018  Mikko "Tapsa" P

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

#include <vector>
#include "SoundItem.h"

namespace genie {

/// A sound "type", set of similar sounds that can be chosen randomly for the same event
class Sound : public ISerializable
{
public:
    void setGameVersion(GameVersion gv) override;

    /// Unique sound ID
    int16_t ID = -1;

    /// Delay before playing sound
    int16_t PlayDelay = 0;

    /// How long to cache sound in memory
    int32_t CacheTime = 300000; // 5 minutes

    /// Sum of all SoundItem probabilities
    int16_t TotalProbability = 100;

    /// List of actual sounds that can be chosen at random
    std::vector<SoundItem> Items;

    bool compareTo(const Sound &other) const;

private:
    void serializeObject(void) override;
};
} // namespace genie

