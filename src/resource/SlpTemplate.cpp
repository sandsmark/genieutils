/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Armin Preiml
    Copyright (C) 2015 - 2016  Mikko "Tapsa" P
    Copyright (C) 2017  Martin Sandsmark

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

#include "genie/resource/SlpTemplate.h"

#include <stdexcept>
#include <chrono>
#include <cassert>

#include "genie/resource/SlpFrame.h"
#include "genie/resource/PalFile.h"
#include "genie/resource/Color.h"
#include "genie/resource/SlpFile.h"
#include "genie/util/Utility.h"

namespace genie {

Logger &SlpTemplateFile::log = Logger::getLogger("genie.SlpTemplate");

//------------------------------------------------------------------------------
void SlpTemplateFile::serializeObject()
{
    if (isOperation(OP_READ) && !loaded_) {
        loadFile();
    } else if (isOperation(OP_WRITE)) { // && loaded_)
        saveFile();
    }
}

//------------------------------------------------------------------------------
void SlpTemplateFile::loadFile() noexcept
{
    for (SlpTemplate &slpTemplate : templates) {
        uint32_t stemplSize{};
        serialize<uint32_t>(stemplSize);

        std::streampos templateBegin = tellg();
        std::streampos nextPos = tellg() + std::streampos(stemplSize);

        serialize<uint32_t>(slpTemplate.width_);
        serialize<uint32_t>(slpTemplate.height_);
        serialize<int32_t>(slpTemplate.hotspot_x);
        serialize<int32_t>(slpTemplate.hotspot_y);
        serialize<int32_t>(slpTemplate.data_size);

        serialize<uint32_t>(slpTemplate.outline_table_offset_);
        slpTemplate.outline_table_offset_ += templateBegin;

        serialize<uint32_t>(slpTemplate.cmd_table_offset_);
        slpTemplate.cmd_table_offset_ += templateBegin;

        getIStream()->seekg(nextPos);
    }

    for (SlpTemplate &slpTemplate : templates) {
        getIStream()->seekg(std::streampos(slpTemplate.outline_table_offset_));

        slpTemplate.left_edges_.resize(slpTemplate.height_);
        slpTemplate.right_edges_.resize(slpTemplate.height_);

        for (uint32_t row = 0; row < slpTemplate.height_; ++row) {
            serialize<uint16_t>(slpTemplate.left_edges_[row]);
            serialize<uint16_t>(slpTemplate.right_edges_[row]);
        }

        getIStream()->seekg(std::streampos(slpTemplate.cmd_table_offset_));
        serialize<uint32_t>(slpTemplate.cmd_offsets_, slpTemplate.height_);
    }

    loaded_ = true;
}

//------------------------------------------------------------------------------
void SlpTemplateFile::saveFile() noexcept
{
}

//------------------------------------------------------------------------------
void SlpTemplateFile::unload() noexcept
{
    if (!loaded_) {
        log.warn("Trying to unload a not loaded slpfile!");
    }

    for (SlpTemplate &slpTemplate : templates) {
        slpTemplate.cmd_offsets_.clear();
        slpTemplate.left_edges_.clear();
        slpTemplate.right_edges_.clear();
    }

    loaded_ = false;
}

//------------------------------------------------------------------------------
bool SlpTemplateFile::isLoaded() const noexcept
{
    return loaded_;
}



void FiltermapFile::serializeObject() noexcept
{
    for (int i = 0; i < SlopeCount; i++) {
        uint32_t dataSize = 0;
        serialize<uint32_t>(dataSize);

        serialize<uint32_t>(maps[i].height);

        for (uint32_t y = 0; y < maps[i].height; y++) {
            FilterLine line;
            serialize<uint8_t>(line.width);

            for (int x = 0; x < line.width; x++) {
                FilterCmd command;
                serialize<uint16_t>(command.sourcePixelCount);
                command.lightIndex = command.sourcePixelCount >> 4;
                command.sourcePixelCount = command.sourcePixelCount & 0xF;

                for (uint16_t n = 0; n < command.sourcePixelCount; n++) {
                    SourcePixel sourcePixel;

                    // this dumb fucking way is only way to get msvc to work
                    const uint8_t b1 = read<uint8_t>();
                    const uint8_t b2 = read<uint8_t>();
                    const uint8_t b3 = read<uint8_t>();
                    const uint32_t packedCommand = (b3 << 16) | (b2 << 8) | b1;

                    sourcePixel.alpha = packedCommand & 0x1ff;
                    sourcePixel.sourceIndex = packedCommand >> 9;

                    command.sourcePixels.push_back(sourcePixel);
                }

                line.commands.push_back(command);
            }

            maps[i].lines.push_back(std::move(line));
        }
    }

    if (getOperation() == OP_READ) {
        m_loaded = true;
    }
}

void PatternMasksFile::serializeObject() noexcept
{
    for (int i = 0; i < 40; i++) {
        int32_t size = 4096;
        serialize<int32_t>(size);
        read(m_masks[i].pixels, size);
    }

    if (getOperation() == OP_READ) {
        m_loaded = true;
    }
}

void IcmFile::serializeObject() noexcept
{
    while (!getIStream()->eof()) {
        InverseColorMap map;
        getIStream()->read((char *)&map.map, 32 * 32 * 32);
        maps.push_back(std::move(map));
    }

    if (getOperation() == OP_READ) {
        m_loaded = true;
    }
}

void LightmapFile::serializeObject() noexcept
{
    getIStream()->read((char *)&lightmaps, 18 * 4096);

    if (getOperation() == OP_READ) {
        m_loaded = true;
    }
}

} // namespace genie
