/*
    geniedat - A library for reading and writing data files of genie
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

#ifndef GENIE_TERRAINBORDER_H
#define GENIE_TERRAINBORDER_H
#include "genie/file/ISerializable.h"

namespace genie
{

class TBFrameData : public ISerializable
{
public:
  TBFrameData();
  virtual ~TBFrameData();
  virtual void setGameVersion(GameVersion gv);

  int16_t FrameID;
  int16_t Flag1;
  int16_t Flag2;

private:
  virtual void serializeObject(void);

};

/// Seems to be unused in GV > RoR
class TerrainBorder : public ISerializable
{
public:
  TerrainBorder();
  virtual ~TerrainBorder();
  virtual void setGameVersion(GameVersion gv);

  int16_t Unknown1;
  int16_t Enabled;

  static uint32_t getNameSize();

  /// Internal long name
  std::string Name;

  /// Internal SLP name
  std::string Name2;

  /// ID of the SLP Resource in BORDER.DRS
  int32_t SLP;
  float Unknown3; // 0
  int32_t SoundID; // -1

  std::array<uint8_t, 3> Colors;
  std::array<int8_t, 5> Unknown5;
  float Unknown6;

  static const unsigned short FRAMES_CNT = 230;
  std::vector<TBFrameData> Frames;

  int16_t FrameCount; //?
  int16_t AngleCount; // always 0
  int16_t TerrainID;

private:
  virtual void serializeObject(void);
};

}

#endif // GENIE_TERRAINBORDER_H
