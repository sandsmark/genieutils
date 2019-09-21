/*
    Mostly used for sloped terrains
    Copyright (C) 2018 - 2019 Martin Sandsmark

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

    Most of the documentation is written by aap, and hence the copyright is owned by him
*/

#pragma once

namespace genie {

/// @page Pattern
/// Different light blending patterns, you can see them at this url:\n
/// https://github.com/aap/geniedoc/blob/master/patternmasks.bmp
enum Pattern {
    FlatPattern = 0,
    BlackPattern = 1,
    DiagDownPattern = 2,
    DiagUpPattern = 3,
    HalfDownPattern = 4,
    HalfUpPattern = 5,
    HalfLeftPattern = 6,
    HalfRightPattern = 7,
    DownPattern = 8,
    UpPattern = 9,
    LeftPattern = 10,
    RightPattern = 11,

// TODO: find better names for the rest of the values
    Pattern12 = 12,
    Pattern13 = 13,
    Pattern14 = 14,
    Pattern15 = 15,
    Pattern16 = 16,
    Pattern17 = 17,
    Pattern18 = 18,
    Pattern19 = 19,
    Pattern20 = 20,
    Pattern21 = 21,
    Pattern22 = 22,
    Pattern23 = 24,
    Pattern24 = 24,
    Pattern25 = 25,
    Pattern26 = 26,
    Pattern27 = 27,
    Pattern28 = 28,
    Pattern29 = 29,
    Pattern30 = 30,
    Pattern31 = 31,
    Pattern32 = 32,
    Pattern33 = 33,
    Pattern34 = 34,
    Pattern35 = 35,
    Pattern36 = 36,
    Pattern37 = 37,
    Pattern38 = 38,
    Pattern39 = 39,
    PatternMasksCount = 40,
};

}
