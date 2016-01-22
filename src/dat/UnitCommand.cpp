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

#include "genie/dat/UnitCommand.h"

namespace genie
{

//------------------------------------------------------------------------------
UnitCommand::UnitCommand()
{
  Enabled = 1;
  ID = -1;
  Unknown1 = 0;
  Type = 7;
  ClassID = -1;
  UnitID = -1;
  TerrainID = -1;
  ResourceIn = -1;
  ResourceProductivityMultiplier = -1;
  ResourceOut = -1;
  Resource = -1;
  Quantity = 0;
  ExecutionRadius = 0;
  ExtraRange = 0;
  Unknown4 = 1;
  Unknown5 = 3;
  SelectionEnabler = 1;
  Unknown7 = 1;
  PlunderSource = 1;
  Unknown9 = 0;
  SelectionMode = 5;
  RightClickMode = 0;
  Unknown12 = 0;
  ToolGraphicID = -1;
  ProceedingGraphicID = -1;
  ActionGraphicID = -1;
  CarryingGraphicID = -1;
  ExecutionSoundID = -1;
  ResourceDepositSoundID = -1;
}

//------------------------------------------------------------------------------
UnitCommand::~UnitCommand()
{
}

//------------------------------------------------------------------------------
void UnitCommand::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);
}

//------------------------------------------------------------------------------
void UnitCommand::serializeObject(void)
{
  serialize<int16_t>(Enabled);
  serialize<int16_t>(ID);
  serialize<int8_t>(Unknown1);
  serialize<int16_t>(Type);
  serialize<int16_t>(ClassID);
  serialize<int16_t>(UnitID);
  serialize<int16_t>(TerrainID);
  serialize<int16_t>(ResourceIn);
  serialize<int16_t>(ResourceProductivityMultiplier);
  serialize<int16_t>(ResourceOut);
  serialize<int16_t>(Resource);
  serialize<float>(Quantity);
  serialize<float>(ExecutionRadius);
  serialize<float>(ExtraRange);
  serialize<int8_t>(Unknown4);
  serialize<float>(Unknown5);
  serialize<int8_t>(SelectionEnabler);
  serialize<int8_t>(Unknown7);
  serialize<int16_t>(PlunderSource);
  serialize<int16_t>(Unknown9);
  serialize<int8_t>(SelectionMode);
  serialize<int8_t>(RightClickMode);
  serialize<int8_t>(Unknown12);
  serialize<int16_t>(ToolGraphicID);
  serialize<int16_t>(ProceedingGraphicID);
  serialize<int16_t>(ActionGraphicID);
  serialize<int16_t>(CarryingGraphicID);
  serialize<int16_t>(ExecutionSoundID);
  serialize<int16_t>(ResourceDepositSoundID);
}

}
