/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2013  Armin Preiml <email>

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


#include "genie/dat/PlayerColour.h"
#include <string.h>

namespace genie
{

PlayerColour::PlayerColour()
{
  ID = -1;
  Palette = 0;
  Colour = 0;
  Unknown1 = 0;
  Unknown2 = 0;
  MinimapColour = 0;
  Unknown3 = 0;
  Unknown4 = 0;
  Unknown5 = 0;
}

PlayerColour::~PlayerColour()
{
}

void PlayerColour::serializeObject(void )
{
  if (getGameVersion() < genie::GV_AoK)
    serialize<std::string>(Name, NAME_LEN);
  
  serialize<int32_t>(ID);
  
  if (getGameVersion() < genie::GV_AoK)
  {
    short col_short = Colour;
    serialize<int16_t>(col_short);
    Colour = col_short;
  }
  else
  {
    serialize<int32_t>(Palette);
    serialize<int32_t>(Colour);
    serialize<int32_t>(Unknown1);
    serialize<int32_t>(Unknown2);
    serialize<int32_t>(MinimapColour);
    serialize<int32_t>(Unknown3);
    serialize<int32_t>(Unknown4);
    serialize<int32_t>(Unknown5);
  }
}


}
