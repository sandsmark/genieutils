/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2017  Mikko "Tapsa" P

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

#include "genie/dat/unit/Bird.h"

namespace genie
{

namespace unit
{

Bird::Bird() //: DeadFish()
{
}

Bird::~Bird()
{
}

//------------------------------------------------------------------------------
void Bird::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  updateGameVersion(TaskList);
}

void Bird::serializeObject(void)
{
  GameVersion gv = getGameVersion();

  serialize<int16_t>(DefaultTaskID);
  serialize<float>(SearchRadius);
  serialize<float>(WorkRate);
  serializePair<int16_t>(DropSite, (gv < GV_TEST) ? true : false);
  serialize<int8_t>(TaskSwapGroup);
  serialize<int16_t>(AttackSound);
  if (gv >= GV_AoEB)
    serialize<int16_t>(MoveSound); // 6.92
  serialize<int8_t>(RunPattern);

  if (gv < GV_AoK) // 11.24
  {
    uint16_t task_count;
    serializeSize<uint16_t>(task_count, TaskList.size());
    serializeSub<Task>(TaskList, task_count);
  }
}

}

}
