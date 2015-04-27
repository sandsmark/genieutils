/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2015  Mikko T P

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

#include "genie/dat/Terrain.h"

namespace genie
{

//------------------------------------------------------------------------------
FrameData::FrameData()
{
  FrameCount = 0;
  AngleCount = 0;
  ShapeID = 0;
}

//------------------------------------------------------------------------------
FrameData::~FrameData()
{
}

//------------------------------------------------------------------------------
void FrameData::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

//------------------------------------------------------------------------------
void FrameData::serializeObject(void)
{
  serialize<int16_t>(FrameCount);
  serialize<int16_t>(AngleCount);
  serialize<int16_t>(ShapeID);
}

}
