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

#ifndef GENIE_PALFILE_H
#define GENIE_PALFILE_H

#include <string>
#include <vector>
#include <stdint.h>
#include <memory>

#include "genie/resource/Color.h"
#include "genie/file/IFile.h"

namespace genie {

class Logger;
class Color;

//------------------------------------------------------------------------------
/// Class for parsing aoe color palettes.
//
class PalFile : public IFile
{
    static constexpr std::string_view fileHeader = "JASC-PAL\r\n";
    static constexpr std::string_view fileVersionHeader = "0100\r\n";

public:
    // For passing by reference
    static const PalFile null;

    //----------------------------------------------------------------------------
    /// Returns a color at given index.
    ///
    /// @param index index of color in palette
    /// @return color object
    //
    const inline Color &operator[](uint16_t index) const
    {
        return colors_[index];
    }

    const std::vector<Color> &getColors() const;


    //----------------------------------------------------------------------------
    /// Number of colors stored in this palette.
    ///
    /// @return size
    //
    size_t size() const;

    //----------------------------------------------------------------------------
    /// TODO: Somethings wrong...
    //
    size_t objectSize() override;

    bool isValid() const;

    std::vector<Color> colors_;

private:
    static Logger &log;


    uint32_t num_colors_ = 0;

    std::string type_;
    std::string unknown_;

    void serializeObject() override;

    //----------------------------------------------------------------------------
    /// Get number of characters of a number serialized as a string.
    //
    size_t numOfChars(uint8_t number);
};

} // namespace genie

#endif // GENIE_PALFILE_H
