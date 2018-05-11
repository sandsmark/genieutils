/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2018 Martin Sandsmark

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

#include "genie/file/CabFile.h"

#include <fstream>
#include <vector>

#include "genie/Types.h"

namespace genie {

//------------------------------------------------------------------------------
CabFile::CabFile() :
    compressor_(this)
{
}

//------------------------------------------------------------------------------
CabFile::~CabFile()
{
    unload();
}

//------------------------------------------------------------------------------
void CabFile::setVerboseMode(bool verbose)
{
    verbose_ = verbose;
}

bool CabFile::seekToHeader()
{
    static const char header[] = { 'M', 'S', 'C', 'F',
                                    0,   0,   0,   0 };

    std::istream *istr = getIStream();
    if (!istr) {
        std::cerr << "no input file set" << std::endl;
        return false;
    }

    std::streampos position = istr->tellg();
    char buffer[sizeof(header)];
    std::cout << sizeof(buffer) << " " << sizeof(header) << std::endl;
    while (istr->good() && !istr->eof()) {
        istr->read(buffer, sizeof(buffer));
        if (strncmp(header, buffer, sizeof(buffer)) == 0) {
            if (verbose_) {
                std::cout << "Found header at " << std::hex << position << std::dec << std::endl;
            }

            istr->seekg(position);

            return true;
        }

        position += std::streampos(1);
        istr->seekg(position);
    }

    return false;
}

//------------------------------------------------------------------------------
void CabFile::serializeObject(void)
{
    if (!seekToHeader()) {
        std::cerr << "Failed to find header" << std::endl;
        return;
    }

    std::istream *istr = getIStream();

    std::streampos startPos = istr->tellg();
    istr->seekg(startPos + std::streampos(8));

    const uint64_t byteCount = read<uint64_t>();
    if (verbose_) {
        std::cout << byteCount << " bytes" << std::endl;
    }

    const uint64_t headerSize = read<uint64_t>();
    if (verbose_) {
        std::cout << "files start at " << headerSize << std::endl;
    }

    const uint8_t versionMajor = read<uint8_t>();
    const uint8_t versionMinor = read<uint8_t>();
    if (verbose_) {
        std::cout << "version " << int(versionMajor) << "." << int(versionMinor) << std::endl;
    }
    if (versionMajor != 3 || versionMinor != 1) {
        std::cerr << "Invalid version" << std::endl;
        return;
    }

    const uint16_t folderCount = read<uint16_t>();
    if (verbose_) {
        std::cout << folderCount << " folders" << std::endl;
    }
    const uint16_t fileCount = read<uint16_t>();
    if (verbose_) {
        std::cout << fileCount << " files" << std::endl;
    }

    const uint16_t flags = read<uint16_t>();
    if (verbose_) {
        std::cout << "flags: " << std::hex << flags << std::dec << std::endl;
    }
    if (flags != 0x0) {
        std::cerr << "not implemented" << std::endl;
        return;
    }

    const uint16_t setId = read<uint16_t>();
    if (verbose_) {
        std::cout << "set id: " << std::hex << setId << std::dec << std::endl;
    }
    const uint16_t setNumber = read<uint16_t>();
    if (verbose_) {
        std::cout << "set number: " << setNumber << std::endl;
    }


    for (uint16_t i=0; i<folderCount; i++) {
        const uint32_t dataOffset = read<uint32_t>();
        if (verbose_) {
            std::cout << "data offset: " << dataOffset << std::endl;
        }
        const uint16_t blockCount = read<uint16_t>();
        if (verbose_) {
            std::cout << "block count: " << blockCount << std::endl;
        }
        const uint16_t compression = read<uint16_t>();
        if (verbose_) {
            std::cout << "compression: 0x" << std::hex << compression << std::dec << std::endl;
        }
    }
    for (uint16_t i=0; i<fileCount; i++) {
        const uint32_t size = read<uint32_t>();
        if (verbose_) {
            std::cout << "file size: " << size << std::endl;
        }

        const uint32_t offsetInFolder = read<uint32_t>();
        if (verbose_) {
            std::cout << "offset in folder: " << offsetInFolder << std::endl;
        }
        const uint16_t folder = read<uint16_t>();
        if (verbose_) {
            std::cout << "folder: " << folder << std::endl;
        }

        const uint16_t date = read<uint16_t>();
        if (verbose_) {
            std::cout << "date: " << std::hex << date << std::dec << std::endl;
        }
        const uint16_t time = read<uint16_t>();
        if (verbose_) {
            std::cout << "time: " << std::hex << time << std::dec << std::endl;
        }
        const uint16_t attributes = read<uint16_t>();
        if (verbose_) {
            std::cout << "attributes: " << std::hex << attributes << std::dec << std::endl;
        }

        std::string filename;
        while (!istr->eof()) {
            filename += istr->peek();
            if (istr->get() == 0x0) {
                break;
            }

        }
        if (verbose_) {
            std::cout << "filename: " << filename << std::endl;
        }

    }

}

//------------------------------------------------------------------------------
void CabFile::unload()
{
}
}
