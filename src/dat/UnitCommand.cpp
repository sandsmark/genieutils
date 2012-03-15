/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011  Armin Preiml <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "genie/dat/UnitCommand.h"

namespace genie
{
  
//------------------------------------------------------------------------------
UnitCommand::UnitCommand()
{
  One = 0;
  ID = -1;
  Unknown1 = 0;
  Type = 7;
  ClassID = -1;
  UnitID = -1;
  Unknown2 = -1;
  ResourceIn = -1;
  SubType = -1;
  ResourceOut = -1;
  Unknown3 = -1;
  Unknown4 = 0;
  ExecutionRadius = 0;
  Unknown5 = 0;
  Unknown6 = 1;
  Unknown7 = 3;
  Unknown8 = 1;
  Unknown9 = 1;
  Unknown10 = 1;
  Unknown11 = 0;
  Unknown12 = 0;
  Unknown13 = 5;
  Unknown14 = 9;
}

//------------------------------------------------------------------------------
UnitCommand::~UnitCommand()
{
}

//------------------------------------------------------------------------------
short UnitCommand::getGraphicsSize(void)
{
  return 6;
}

//------------------------------------------------------------------------------
void UnitCommand::serializeObject(void )
{
  serialize<int16_t>(One);
  serialize<int16_t>(ID);
  serialize<char>(Unknown1); 
  serialize<int16_t>(Type);
  serialize<int16_t>(ClassID);
  serialize<int16_t>(UnitID);
  serialize<int16_t>(Unknown2);
  serialize<int16_t>(ResourceIn);
  serialize<int16_t>(SubType);
  serialize<int16_t>(ResourceOut);
  serialize<int16_t>(Unknown3);
  serialize<float>(Unknown4);
  serialize<float>(ExecutionRadius);
  serialize<float>(Unknown5);
  serialize<char>(Unknown6);
  serialize<float>(Unknown7);
  serialize<char>(Unknown8);
  serialize<char>(Unknown9);
  serialize<char>(Unknown10);
  serialize<char>(Unknown11);
  serialize<int16_t>(Unknown12);
  serialize<int16_t>(Unknown13);
  serialize<char>(Unknown14);
  serialize<int16_t>(Graphics, getGraphicsSize());
}


}