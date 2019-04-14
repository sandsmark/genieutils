/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Armin Preiml

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

#include "genie/resource/PalFile.h"

#include <iostream>
#include <stdexcept>

#include <string_view>

#include "genie/util/Logger.h"

namespace genie {

const PalFile PalFile::null;

using std::string;

Logger &PalFile::log = Logger::getLogger("genie.PalFile");

//------------------------------------------------------------------------------
PalFile::~PalFile()
{
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
const std::vector<Color> &PalFile::getColors() const
{
    return colors_;
}

//------------------------------------------------------------------------------
size_t PalFile::size() const
{
    return colors_.size();
}

//------------------------------------------------------------------------------
void PalFile::serializeObject()
{
    if (isOperation(OP_READ)) {
        std::istream *istr = getIStream();

        std::string header(fileHeader.size(), 0);

        istr->read(header.data(), header.size());

        if (header != fileHeader) {
            log.error("Not a color palette!");
            //TODO: Exception
            return;
        }

        std::string versionHeader(fileVersionHeader.size(), 0);
        istr->read(versionHeader.data(), versionHeader.size());

        if (versionHeader != fileVersionHeader) {
            log.warn("Invalid file version %", versionHeader);
        }

        *istr >> num_colors_;

        for (uint32_t i = 0; i < num_colors_; i++) {
            uint32_t r, g, b;
            *istr >> r >> g >> b;
            colors_.push_back(Color(r, g, b));
        }
    } else {
        std::ostream *ostr = getOStream();

        static const std::string lineEnding("\r\n"); // windows line ending

        *ostr << fileHeader;
        *ostr << fileVersionHeader;

        if (colors_.size() > 256)
            log.error("Too much colors (>256)");

        *ostr << colors_.size() << lineEnding;

        for (uint32_t i = 0; i < colors_.size(); i++) {
            *ostr << (int)colors_[i].r << " ";
            *ostr << (int)colors_[i].g << " ";
            *ostr << (int)colors_[i].b << lineEnding;
        }
    }
}

//------------------------------------------------------------------------------
size_t PalFile::numOfChars(uint8_t number)
{
    if (number < 10)
        return 1;

    if (number < 100)
        return 2;

    return 3;
}

//------------------------------------------------------------------------------
size_t PalFile::objectSize()
{
    size_t size = 0;

    constexpr size_t lineEndingSize = std::string_view("\r\n").size();

    size += fileHeader.size();
    size += fileVersionHeader.size();

    if (colors_.size() > 256)
        log.error("Too much colors (>256)");

    size += numOfChars(colors_.size()) + lineEndingSize;

    for (uint32_t i = 0; i < colors_.size(); i++) {
        size += numOfChars(colors_[i].r);
        size += numOfChars(colors_[i].g);
        size += numOfChars(colors_[i].b);

        size += 2; // spaces
        size += lineEndingSize;
    }

    return size;
}

bool PalFile::isValid() const
{
    return !colors_.empty();
}

}
