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

#include "lzx.h"

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

void CabFile::readFile(std::string filename)
{
    for (size_t i=0; i<filename.size(); i++) {
        if (filename[i] == '/') {
            filename[i] = '\\';
        }
    }

    if (m_files.count(filename) == 0) {
        std::cerr << filename << " is not in the archive" << std::endl;
        return;
    }
    const File &file = m_files[filename];
    if (file.folder > m_folders.size()) {
        std::cerr << "Invalid folder " << file.folder;
        return;
    }

    const Folder &folder = m_folders[file.folder];
//    uint16_t compressedSize = 0;
    switch(folder.compression) {
    case Uncompressed:
        std::cout << "Uncompressed" << std::endl;
//        compressedSize = file.size;
        break;
    case MsZip:
        std::cerr << "MsZip is not implemented" << std::endl;
        return;
    case Lzx:
        std::cout << "lzx compression" << std::endl;
        break;
    default:
        std::cout << "unknown compression " << std::hex << folder.compression << std::dec << std::endl;
        return;
    }

    const uint32_t fileEnd = file.offsetInFolder + file.size;
//    std::cout <
//    std::string contents;
//    contents.resize(file.size);

    std::istream *istr = getIStream();

    const int windowSize = (folder.compression >> 8) & 0x1f;

    for (size_t i=0; i<m_folders[file.folder].blocks.size(); i++) {
        const Block &block = folder.blocks[i];
        if (fileEnd < block.uncompressedOffset) {
            continue;
        }
        if (file.offsetInFolder > block.uncompressedOffset + block.uncompressedSize) {
            continue;
        }

//        std::streampos blockpos = getInitialReadPosition() + std::streampos(folder.dataOffset);
        std::streampos blockpos = block.streamOffset;
        std::cout << "reading block at " << std::hex << blockpos << std::dec << std::endl;
        istr->seekg(blockpos);

        std::vector<uint8_t> compressedBuffer;
        serialize(compressedBuffer, block.compressedSize);

        std::vector<uint8_t> uncompressedBuffer;
        uncompressedBuffer.resize(block.uncompressedSize);

        std::cout << "compressed size " << block.compressedSize << " uncompressed " << block.uncompressedSize << std::endl;
        std::cout << "compressed size " << compressedBuffer.size() << " uncompressed " << uncompressedBuffer.size() << std::endl;

        LZXStatePtr decompressor(LZXinit(windowSize), ::LZXteardown);
        int ret = LZXdecompress(decompressor.get(),
                                compressedBuffer.data(), uncompressedBuffer.data(),
                                int(compressedBuffer.size()), int(uncompressedBuffer.size())
                );
        if (ret != DECR_OK) {
            std::cout << "Failed to decode block " << ret << std::endl;
            return;
        }
//        std::cout << uncompressedBuffer << std::endl;
    }
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
    if (getOperation() != OP_READ) {
        std::cerr << "Writing CAB files not implemented yet" << std::endl;
        return;
    }

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
        Folder folder;
        folder.dataOffset = read<uint32_t>();
        if (verbose_) {
            std::cout << "data offset: " << folder.dataOffset << std::endl;
        }
        folder.blockCount = read<uint16_t>();
        if (verbose_) {
            std::cout << "block count: " << folder.blockCount << std::endl;
        }
        folder.compression = read<uint16_t>();
        if (verbose_) {
            std::cout << "compression: 0x" << std::hex << folder.compression << std::dec << std::endl;
        }

        m_folders.push_back(std::move(folder));
    }

    for (uint16_t i=0; i<fileCount; i++) {
        File file;
        file.size = read<uint32_t>();
        if (verbose_) {
            std::cout << "file size: " << file.size << std::endl;
        }

        file.offsetInFolder = read<uint32_t>();
        if (verbose_) {
            std::cout << "offset in folder: " << file.offsetInFolder << std::endl;
        }
        file.folder = read<uint16_t>();
        if (verbose_) {
            std::cout << "folder: " << file.folder << std::endl;
        }

        file.date = read<uint16_t>();
        if (verbose_) {
            std::cout << "date: " << std::hex << file.date << std::dec << std::endl;
        }
        file.time = read<uint16_t>();
        if (verbose_) {
            std::cout << "time: " << std::hex << file.time << std::dec << std::endl;
        }
        file.attributes = read<uint16_t>();
        if (verbose_) {
            std::cout << "attributes: " << std::hex << file.attributes << std::dec << std::endl;
        }

        while (!istr->eof()) {
            char c = istr->get();
            if (c == 0x0) {
                break;
            }

            file.filename += c;
        }

        if (verbose_) {
            std::cout << "filename: " << file.filename << std::endl;
        }

        m_files[file.filename] = std::move(file);
    }


    uint32_t uncompressedOffset = 0;
    for (uint16_t i=0; i<folderCount; i++) {
        Folder &folder = m_folders[i];
        for (uint16_t j=0; j<folder.blockCount; j++) {
            Block block;
            block.checksum = read<uint32_t>();
            block.compressedSize = read<uint16_t>();
            block.uncompressedSize = read<uint16_t>();
            block.uncompressedOffset = uncompressedOffset;
            block.streamOffset = istr->tellg();
            uncompressedOffset += block.uncompressedSize;
            istr->seekg(istr->tellg() + std::streampos(block.compressedSize));
//            std::cout << "checksum: " << std::hex << block.checksum << std::dec << std::endl;
//            std::cout << "compressed size: " << block.compressedSize << std::endl;
//            std::cout << "uncompressed size: " << block.uncompressedSize << std::endl;

            folder.blocks.push_back(std::move(block));
        }
    }
}

//------------------------------------------------------------------------------
void CabFile::unload()
{
}
}
