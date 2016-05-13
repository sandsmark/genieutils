/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2016  Mikko "Tapsa" P

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
}

PlayerColour::~PlayerColour()
{
}

//------------------------------------------------------------------------------
void PlayerColour::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

void PlayerColour::serializeObject(void)
{
  if (getGameVersion() < genie::GV_AoKE3)
    serialize(Name, NAME_SIZE);


  if (getGameVersion() < genie::GV_AoKE3)
  {
    int16_t ID_short = ID;
    serialize<int16_t>(ID_short);
    ID = ID_short;
    int16_t unk_short = Unknown1;
    serialize<int16_t>(unk_short);
    Unknown1 = unk_short;
    uint8_t col_byte = Colour;
    serialize<uint8_t>(col_byte);
    Colour = col_byte;
    uint8_t unk_byte = Unknown2;
    serialize<uint8_t>(unk_byte);
    Unknown2 = unk_byte;
  }
  else
  {
    serialize<int32_t>(ID);
    serialize<int32_t>(Palette);
    serialize<int32_t>(Colour);
    serialize<int32_t>(Unknown1);
    serialize<int32_t>(Unknown2);
    serialize<int32_t>(MinimapColour);
    serialize<int32_t>(Unknown3);
    serialize<int32_t>(Unknown4);
    serialize<int32_t>(StatisticsText);
  }
}

}
