/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2014  Mikko "Tapsa" P

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

Graphic::Graphic() : Coordinates(COORDINATES_SIZE)
{
  Name = "";
  Name2 = "";
  SLP = -1;
  Unknown1 = 0;
  Unknown2 = 0;
  Layer = 0;
  PlayerColor = -1;
  Rainbow = -1;
  Replay = 0;
  SoundID = -1;
  AttackSoundUsed = 0;
  FrameCount = 0;
  AngleCount = 0;
  NewSpeed = 0;
  FrameRate = 0;
  ReplayDelay = 0;
  SequenceType = 0;
  ID = -1;
  MirroringMode = 0;
  Unknown3 = 0;
}

Graphic::~Graphic()
{
}

//------------------------------------------------------------------------------
void Graphic::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  updateGameVersion(Deltas);
  updateGameVersion(AttackSounds);
}

unsigned short Graphic::getNameSize()
{
  if (getGameVersion() < genie::GV_SWGB)
    return NAME_SIZE;
  else
    return NAME_SIZE_SWGB;
}

unsigned short Graphic::getName2Size()
{
  if (getGameVersion() < genie::GV_SWGB)
    return NAME_SIZE2;
  else
    return NAME_SIZE_SWGB;
}

void Graphic::serializeObject(void)
{
  serialize<std::string>(Name, getNameSize());
  serialize<std::string>(Name2, getName2Size());

  if (getGameVersion() < genie::GV_TEST)
  {
    int16_t slp = SLP;
    serialize<int16_t>(slp);
    SLP = slp;
  }
  else
  {
    serialize<int32_t>(SLP);
  }
  serialize<int8_t>(Unknown1);
  serialize<int8_t>(Unknown2); /// TODO: priority?
  serialize<int8_t>(Layer);
  serialize<int8_t>(PlayerColor);
  serialize<int8_t>(Rainbow); // 2nd half of player color
  serialize<int8_t>(Replay);

  serialize<int16_t>(Coordinates, COORDINATES_SIZE);

  serializeSize<uint16_t>(DeltaCount, Deltas.size());
  serialize<int16_t>(SoundID);
  serialize<int8_t>(AttackSoundUsed);
  serialize<uint16_t>(FrameCount);
  serialize<uint16_t>(AngleCount);
  serialize<float>(NewSpeed);
  serialize<float>(FrameRate);
  serialize<float>(ReplayDelay);
  serialize<int8_t>(SequenceType);
  serialize<int16_t>(ID);
  serialize<int8_t>(MirroringMode);

  if (getGameVersion() >= genie::GV_AoKB) // 10.72
    serialize<int8_t>(Unknown3); // Maybe sprite editor thing?

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
