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

#include "genie/dat/Graphic.h"
#include <string.h>

#include <iostream>

namespace genie
{

Graphic::Graphic() : Coordinates(), CstrName(0), CstrName2(0)
{
  Name = "";
  Name2 = "";
  SLP = -1;
  Unknown1 = 0;
  Unknown2 = 0;
  Layer = 0;
  Unknown3 = 0;
  Unknown4 = 0;
  Replay = 0;
  SoundID = -1;
  AttackSoundUsed = 0;
  FrameCount = 0;
  AngleCount = 0;
  Unknown13 = 0;
  FrameRate = 0;
  ReplayDelay = 0;
  SequenceType = 0;
  ID = -1;
  Type = 0;
}

Graphic::Graphic(const Graphic& other) : ISerializable(other),
    Coordinates(), CstrName(0), CstrName2(0)
{
  *this = other;
}

Graphic::~Graphic()
{
  delete [] CstrName;
  delete [] CstrName2;
}

//------------------------------------------------------------------------------
void Graphic::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  updateGameVersion(Deltas);
  updateGameVersion(AttackSounds);
}

Graphic &Graphic::operator=(const Graphic &other)
{
  try
  {
    arraycpy<char>(&CstrName, other.CstrName, NAME_SIZE);
    arraycpy<char>(&CstrName2, other.CstrName2, NAME_SIZE2);
  }
  catch (std::bad_alloc &e)
  {
    delete [] CstrName;
    delete [] CstrName2;
    throw;
  }

  Name = other.Name;
  Name2 = other.Name2;

  SLP = other.SLP;
  Unknown1 = other.Unknown1;
  Unknown2 = other.Unknown2;
  Layer = other.Layer;
  Unknown3 = other.Unknown3;
  Unknown4 = other.Unknown4;
  Replay = other.Replay;
  Coordinates = other.Coordinates;
  DeltaCount = other.DeltaCount;
  SoundID = other.SoundID;
  AttackSoundUsed = other.AttackSoundUsed;
  FrameCount = other.FrameCount;
  AngleCount = other.AngleCount;
  Unknown13 = other.Unknown13;
  FrameRate = other.FrameRate;
  ReplayDelay = other.ReplayDelay;
  SequenceType = other.SequenceType;
  ID = other.ID;
  Type = other.Type;

  Deltas = other.Deltas;
  AttackSounds = other.AttackSounds;

  return *this;
}

unsigned short Graphic::getNameSize()
{
  if (getGameVersion() <= genie::GV_TC)
    return NAME_SIZE;
  else
    return NAME_SIZE_SWGB;
}

unsigned short Graphic::getName2Size()
{
  if (getGameVersion() <= genie::GV_TC)
    return NAME_SIZE2;
  else
    return NAME_SIZE_SWGB;
}

void Graphic::serializeObject(void)
{
  /*
   * Workaround for Name strings, because in RoR, SWGB and TC there are
   * Unknown values after the \0 on some strings. In RoR the size should be
   * ok, but it may not be right in >= SWGB
   */
  if (isOperation(OP_WRITE))
  {
    if (CstrName == 0 || Name.compare(CstrName) != 0)
      serialize<std::string>(Name, getNameSize());
    else
      serialize<char>(&CstrName, getNameSize());

    if (CstrName2 == 0 || Name2.compare(CstrName2) != 0)
      serialize<std::string>(Name2, getName2Size());
    else
      serialize<char>(&CstrName2, getName2Size());
  }
  else
  {
    serialize<char>(&CstrName, getNameSize());
    serialize<char>(&CstrName2, getName2Size());

    Name = std::string(CstrName);
    Name2 = std::string(CstrName2);
  }

  serialize<int32_t>(SLP);
  serialize<char>(Unknown1);
  serialize<char>(Unknown2); /// TODO: priority?
  serialize<char>(Layer);
  serialize<char>(Unknown3);
  serialize<char>(Unknown4);
  serialize<char>(Replay);

  serialize<int16_t, COORDINATES_SIZE>(Coordinates);

  serializeSize<uint16_t>(DeltaCount, Deltas.size());
  serialize<int16_t>(SoundID);
  serialize<char>(AttackSoundUsed);
  serialize<uint16_t>(FrameCount);
  serialize<uint16_t>(AngleCount);
  serialize<float>(Unknown13);
  serialize<float>(FrameRate);
  serialize<float>(ReplayDelay);
  serialize<char>(SequenceType);
  serialize<int16_t>(ID);

  if (getGameVersion() >= genie::GV_AoK)
    serialize<int16_t>(Type);
  else
  {
    char tmp = Type;
    serialize<char>(tmp);
    Type = tmp;
  }

  serializeSub<GraphicDelta>(Deltas, DeltaCount);

  if (AttackSoundUsed != 0)
  {
    if (isOperation(OP_WRITE) && AttackSounds.size() > AngleCount)
      std::cerr << "Warning: There'are more GraphicAttackSounds than angles!"
                << std::endl;

    serializeSub<GraphicAttackSound>(AttackSounds, AngleCount);
  }

}

}
