/*
    Reading blkedge.dat and tileedge.dat
    Copyright (C) 2019 Martin Sandsmark

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

#pragma once

namespace genie {

enum Slope : int8_t {
    SlopeInvalid     = -1,
    SlopeFlat        = 0,
    SlopeSouthUp     = 1,
    SlopeNorthUp     = 2,
    SlopeWestUp      = 3,
    SlopeEastUp      = 4,
    SlopeSouthWestUp = 5,
    SlopeNorthWestUp = 6,
    SlopeSouthEastUp = 7,
    SlopeNorthEastUp = 8,
    SLOPE_S_UP2      = 9,    // what are these?
    SLOPE_N_UP2      = 10,
    SLOPE_W_UP2      = 11,
    SLOPE_E_UP2      = 12,
    SlopeNorthDown   = 13,
    SlopeSouthDown   = 14,
    SlopeWestDown    = 15,
    SlopeEastDown    = 16,

    SlopeCount       = 17,

    SlopeSouthWestEastUp = SlopeNorthDown,
    SlopeNorthWestEastUp = SlopeSouthDown,
    SlopeNorthSouthEastUp = SlopeWestDown,
    SlopeNorthSouthWestUp = SlopeEastDown,
};

}//namespace genie
