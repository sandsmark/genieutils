/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2017  Mikko "Tapsa" P

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

#ifndef GENIE_CABFILE_H
#define GENIE_CABFILE_H

#include <string>
#include <iostream>
#include <unordered_map>

#include "genie/Types.h"
#include "genie/file/IFile.h"
#include "genie/file/Compressor.h"

namespace genie {

class CabFile : public IFile
{
public:
    enum CompressionTypes : uint16_t {
        Uncompressed = 0x0,
        MsZip = 0x1,
        Lzx = 0x1503
    };

    struct File {
        uint32_t size;
        uint32_t offsetInFolder;
        uint16_t folder;
        uint16_t date;
        uint16_t time;
        uint16_t attributes;
        std::string filename;
    };

    struct Block {
        uint32_t checksum;
        uint16_t compressedSize;
        uint16_t uncompressedSize;
        uint32_t uncompressedOffset;
        std::streampos streamOffset;
    };

    struct Folder {
        uint32_t dataOffset;
        uint16_t blockCount;
        uint16_t compression;
        std::vector<Block> blocks;
    };

    //----------------------------------------------------------------------------
    /// Standard constructor
    //
    CabFile();

    //----------------------------------------------------------------------------
    /// Destructor
    //
    virtual ~CabFile();

    //----------------------------------------------------------------------------
    /// Debug information will be printed to stdout if activated.
    ///
    /// @param verbose true to activate
    //
    void setVerboseMode(bool verbose);

    void readFile(std::string filename);

private:
    bool seekToHeader();
    // if true print debug messages
    bool verbose_ = false;

    Compressor compressor_;

    CabFile(const CabFile &other);
    CabFile &operator=(const CabFile &other);

    virtual void serializeObject(void);

    std::vector<Folder> m_folders;
    std::unordered_map<std::string, File> m_files;
};
}

#endif // GENIE_CABFILE_H
