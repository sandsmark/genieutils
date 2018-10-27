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

#define IS_LIKELY(x)      __builtin_expect(!!(x), 1)
#define IS_UNLIKELY(x)    __builtin_expect(!!(x), 0)

namespace genie {

Logger &SlpTemplateFile::log = Logger::getLogger("genie.SlpTemplate");

//------------------------------------------------------------------------------
SlpTemplateFile::SlpTemplateFile() :
    IFile()
{
}

//------------------------------------------------------------------------------
SlpTemplateFile::~SlpTemplateFile()
{
}

//------------------------------------------------------------------------------
void SlpTemplateFile::serializeObject(void)
{
    if (isOperation(OP_READ) && !loaded_) {
        loadFile();
    } else if (isOperation(OP_WRITE)) // && loaded_)
    {
        saveFile();
    }
}

//------------------------------------------------------------------------------
void SlpTemplateFile::loadFile()
{
//    std::streampos beginning = tellg();
    for (SlpTemplate &slpTemplate : templates) {

        uint32_t stemplSize;
        serialize(stemplSize);
        std::streampos templateBegin = tellg();
        std::streampos nextPos = tellg() + std::streampos(stemplSize);

        serialize(slpTemplate.width_);
        serialize(slpTemplate.height_);
        serialize(slpTemplate.hotspot_x);
        serialize(slpTemplate.hotspot_y);
        serialize(slpTemplate.data_size);

        serialize(slpTemplate.outline_table_offset_);
        slpTemplate.outline_table_offset_ += templateBegin;

        serialize(slpTemplate.cmd_table_offset_);
        slpTemplate.cmd_table_offset_ += templateBegin;

        getIStream()->seekg(nextPos);
    }

    for (SlpTemplate &slpTemplate : templates) {
        getIStream()->seekg(std::streampos(slpTemplate.outline_table_offset_));

        slpTemplate.left_edges_.resize(slpTemplate.height_);
        slpTemplate.right_edges_.resize(slpTemplate.height_);
        for (uint32_t row = 0; row < slpTemplate.height_; ++row) {
            serialize(slpTemplate.left_edges_[row]);
            serialize(slpTemplate.right_edges_[row]);
        }

        getIStream()->seekg(std::streampos(slpTemplate.cmd_table_offset_));
        serialize(slpTemplate.cmd_offsets_, slpTemplate.height_);
    }

    loaded_ = true;
}

//------------------------------------------------------------------------------
void SlpTemplateFile::saveFile()
{
//#ifndef NDEBUG
//    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
//#endif
//    serializeHeader();
//    slp_offset_ = 32 + 32 * num_frames_;

//    // Write frame headers
//    for (uint32_t i = 0; i < num_frames_; ++i) {
//        frames_[i]->setSaveParams(*getOStream(), slp_offset_);
//        frames_[i]->serializeHeader();
//    }

//    // Write frame content
//    for (uint32_t i = 0; i < num_frames_; ++i) {
//        frames_[i]->save(*getOStream());
//    }
//#ifndef NDEBUG
//    std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now();
//    log.debug("SLP (%u bytes) saving took [%u] milliseconds", slp_offset_, std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count());
//#endif
}

//------------------------------------------------------------------------------
void SlpTemplateFile::unload(void)
{
    if (!loaded_)
        log.warn("Trying to unload a not loaded slpfile!");

    for (SlpTemplate &slpTemplate : templates) {
        slpTemplate.cmd_offsets_.clear();
        slpTemplate.left_edges_.clear();
        slpTemplate.right_edges_.clear();
    }

    loaded_ = false;
}

//------------------------------------------------------------------------------
bool SlpTemplateFile::isLoaded(void) const
{
    return loaded_;
//    return loaded_ && icmFile && patternmasksFile && filtermapFile;
}

const IcmFile::InverseColorMap &PatternMasksFile::getIcm(const uint16_t lightIndex, const std::vector<Pattern> &patterns) const
{
    if (patterns.empty()) {
        return icmFile.maps[4];
    }

    uint8_t lightmapIndex = m_masks[patterns[0]].pixels[lightIndex] >> 2;

    for (size_t i=1; i < patterns.size(); i++) {
        lightmapIndex = m_masks[patterns[i]].apply(lightmapIndex, lightIndex);
    }

    const size_t icmIndex = lightmapFile.lightmaps[lightmapIndex][lightIndex];

    if (icmIndex >= icmFile.maps.size()) {
//        std::cerr << "Icm index out of range " << icmIndex << " " << icmFile.maps.size() << std::endl;

        return icmFile.maps[4];
    }

    return icmFile.maps[icmIndex];
}

void FiltermapFile::serializeObject()
{
    for (int i=0; i<SlopeCount; i++) {
        uint32_t dataSize = 0;
        serialize(dataSize);

        serialize(maps[i].height);

        for (uint32_t y = 0; y<maps[i].height; y++) {
            FilterLine line;
            serialize(line.width);

            for (int x=0; x<line.width; x++) {
                FilterCmd command;
                serialize(command.sourcePixelCount);
                command.lightIndex = command.sourcePixelCount >> 4;
                command.sourcePixelCount = command.sourcePixelCount & 0xF;

                for (uint16_t n=0; n<command.sourcePixelCount; n++) {
                    SourcePixel sourcePixel;

                    // this dumb fucking way is only way to get msvc to work
                    const uint8_t b1 = read<uint8_t>();
                    const uint8_t b2 = read<uint8_t>();
                    const uint8_t b3 = read<uint8_t>();
                    const uint32_t packedCommand = (b3 << 16) | (b2 << 8) | b1;

                    sourcePixel.alpha = packedCommand & 0x1ff;
                    sourcePixel.sourceIndex = packedCommand >> 9;

                    command.sourcePixels.push_back(std::move(sourcePixel));
                }
                line.commands.push_back(std::move(command));
            }

            maps[i].lines.push_back(std::move(line));
        }
    }

    if (getOperation() == OP_READ) {
        m_loaded = true;
    }
}

}
