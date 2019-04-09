/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Armin Preiml
    Copyright (C) 2015 - 2016  Mikko "Tapsa" P

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

#include "genie/resource/EdgeFiles.h"

#include <stdexcept>
#include <chrono>
#include <cmath>

namespace genie {

template<>
Logger &TileEdgeFile::log = Logger::getLogger("genie.EdgeFiles");

template<>
Logger &BlkEdgeFile::log = Logger::getLogger("genie.EdgeFiles");

const VisibilityMask VisibilityMask::null;

}// namespace genie
