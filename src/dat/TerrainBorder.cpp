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

#include "genie/dat/TerrainBorder.h"

namespace genie
{

//------------------------------------------------------------------------------
TerrainBorder::TerrainBorder()
{
}

//------------------------------------------------------------------------------
TerrainBorder::~TerrainBorder()
{
}

//------------------------------------------------------------------------------
uint32_t TerrainBorder::getNameSize()
{
  return 13;
}

//------------------------------------------------------------------------------
void TerrainBorder::serializeObject(void)
{
  serialize<int16_t>(Unknown1);
  serialize<int16_t>(Enabled);

  serialize<std::string>(Name, getNameSize());
  serialize<std::string>(Name2, getNameSize());

  serialize<int32_t>(SLP);
  serialize<float>(Unknown3);
  serialize<int32_t>(SoundID); // -1

  serialize<unsigned char, 3>(Colors);
  serialize<char, 5>(Unknown5);
  serialize<float>(Unknown6);

  serializeSub<TBFrameData, FRAMES_CNT>(Frames);

  serialize<int16_t>(FrameCount);
  serialize<int16_t>(AngleCount);
  serialize<int16_t>(TerrainID);
}

//------------------------------------------------------------------------------
TBFrameData::TBFrameData()
{
}

//------------------------------------------------------------------------------
TBFrameData::~TBFrameData()
{
}

//------------------------------------------------------------------------------
void TBFrameData::serializeObject(void)
{
  serialize<int16_t>(FrameID);
  serialize<int16_t>(Flag1);
  serialize<int16_t>(Flag2);
}

}
