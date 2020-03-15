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

#ifndef GENIE_BINAFILE_H
#define GENIE_BINAFILE_H

#include "genie/file/ISerializable.h"
#include "PalFile.h"
#include "UIFile.h"
#include "genie/script/ScnFile.h"

namespace genie {

class Logger;

typedef std::shared_ptr<uint8_t[]> BmpFilePtr;

//------------------------------------------------------------------------------
/// Class for loading "bina" files of a drs file.
//
class BinaFile : public ISerializable
{
public:
    BinaFile(uint32_t size);

    std::shared_ptr<PalFile> readPalFile(std::istream *istr);
    UIFilePtr readUIFile(std::istream *istr);
    BmpFilePtr readBmpFile(std::istream *istr);
    std::string readScriptFile(std::istream *istr);
    ScnFilePtr readScnFile(std::istream *istr);

    std::string filetype(std::istream *istr);

    uint32_t size() const { return m_size; }

private:
    static Logger &log;

    void serializeObject() override;
    uint32_t m_size;
};

typedef std::shared_ptr<BinaFile> BinaFilePtr;
} // namespace genie

#endif // GENIE_BINAFILE_H
