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

#include "genie/dat/SoundItem.h"
#include <string.h>

namespace genie
{

SoundItem::SoundItem()
{
  ResourceID = -1;
  Probability = 100;
  Civ = -1;
  Unknown1 = -1;
}

SoundItem::~SoundItem()
{
}

unsigned short SoundItem::getFileNameSize()
{
  if (getGameVersion() <= genie::GV_TC)
    return 13;
  else
    return 27;
}

void SoundItem::serializeObject(void)
{
  serialize<std::string>(FileName, getFileNameSize());

  serialize<int32_t>(ResourceID);
  serialize<int16_t>(Probability);

  if (getGameVersion() >= genie::GV_AoKA)
  {
    serialize<int16_t>(Civ);
    serialize<int16_t>(Unknown1);
  }
}

}
