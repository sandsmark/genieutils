/*
    genieutils - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
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

#include "genie/file/IFile.h"

namespace genie {

//------------------------------------------------------------------------------
IFile::~IFile()
{
    fileIn_.close();
}

//------------------------------------------------------------------------------
void IFile::freelock()
{
    fileIn_.close();
}

//------------------------------------------------------------------------------
void IFile::setFileName(const std::string &fileName)
{
    fileName_ = fileName;
}

//------------------------------------------------------------------------------
const char *IFile::getFileName(void) const
{
    return fileName_.c_str();
}

//------------------------------------------------------------------------------
void IFile::load()
{
    if (fileName_.empty()) {
        throw std::ios_base::failure("Load: File name not set");
    }

    load(fileName_.c_str());
}

//------------------------------------------------------------------------------
void IFile::load(const std::string &fileName)
{
    freelock();

    fileName_ = fileName;

    fileIn_.open(fileName, std::ios::binary | std::ios::in);

    if (fileIn_.fail()) {
        fileIn_.close();
        std::string errnoString(strerror(errno));
        throw std::ios_base::failure("Can't read file \"" + fileName_ + "\": " + errnoString);
    } else {
        readObject(fileIn_);
        loaded_ = true;
    }
}

//------------------------------------------------------------------------------
void IFile::save()
{
    if (fileName_.empty()) {
        throw std::ios_base::failure("Save: File name not set");
    }

    saveAs(fileName_.c_str());
}

//------------------------------------------------------------------------------
void IFile::saveAs(const char *fileName)
{
    std::ofstream file;

    file.open(fileName, std::ofstream::binary);

    if (file.fail()) {
        file.close();
        throw std::ios_base::failure("Can't write to file: \"" + std::string(fileName) + "\"");
    } else {
        writeObject(file);
    }

    file.close();
}

//------------------------------------------------------------------------------
void IFile::unload()
{
}
} // namespace genie
