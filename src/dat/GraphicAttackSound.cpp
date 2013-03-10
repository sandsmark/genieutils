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

#include "genie/dat/GraphicAttackSound.h"
#include <string.h>

namespace genie
{

GraphicAttackSound::GraphicAttackSound()
{
  SoundDelay = -1;
  SoundID = -1;
  SoundDelay2 = -1;
  SoundID2 = -1;
  SoundDelay3 = -1;
  SoundID3 = -1;
}

GraphicAttackSound::~GraphicAttackSound()
{
}

//------------------------------------------------------------------------------
void GraphicAttackSound::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

void GraphicAttackSound::serializeObject(void)
{
  serialize<int16_t>(SoundDelay);
  serialize<int16_t>(SoundID);
  serialize<int16_t>(SoundDelay2);
  serialize<int16_t>(SoundID2);
  serialize<int16_t>(SoundDelay3);
  serialize<int16_t>(SoundID3);
}

}
