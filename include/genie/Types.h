/*
    genieutils - <description>
    Copyright (C) 2013  Armin Preiml <email>

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

#ifndef GENIE_TYPES_H
#define GENIE_TYPES_H

namespace genie
{

enum GameVersion
{
  GV_None = 0, //Game version not set
  GV_AoE,
  GV_RoR,
  GV_AoKA,
  GV_AoK,
  GV_TC,
  GV_SWGB,
  GV_CC
};

}

#endif //GENIE_TYPES_H
