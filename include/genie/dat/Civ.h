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

#ifndef GENIE_CIV_H
#define GENIE_CIV_H
#include "genie/file/ISerializable.h"
#include "Unit.h"

namespace genie
{

/// Class holding information about a civilization
class Civ : public ISerializable
{
public:
  Civ(GameVersion gv = GV_None);
  virtual ~Civ();
  virtual void setGameVersion(GameVersion gv);

  /// Always one
  char One;

  /// Returns size for both names
  static unsigned short getNameSize();

  /// Internal name
  std::string Name;

  /// MinGameVersion: SWGB
  std::string Name2;

  /// ID of the technology tree for a civ.
  int16_t TechTreeID;

  /// ID of the team bonus
  int16_t TeamBonusID; // not in aoe/ror

  /// Resources can easily added and removed using this vector.
  std::vector<float> Resources;

  char GraphicSet;

  std::vector<int32_t> UnitPointers;

  /// Units defined for this civ.
  std::vector<Unit> Units;

  static const unsigned short SWUNKNOWN_SIZE = 4;
  std::array<int16_t, SWUNKNOWN_SIZE> SUnknown1; // Unknown in >=SWGB (cnt=4)

private:
  /// Number of resources defined for this civilization
  uint16_t ResourceCount;
  uint16_t UnitCount;

  virtual void serializeObject(void);
};

}

#endif // GENIE_CIV_H
