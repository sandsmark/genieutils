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

#include "genie/resource/SlpFrame.h"
#include "genie/resource/PalFile.h"
#include "genie/resource/Color.h"

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
    for (SlpTemplate &slpTemplate : templates_) {

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
//        std::cout << "outline offset: " << slpTemplate.outline_table_offset_ << std::endl;
//        std::cout << "pos " << tellg() << std::endl;
        serialize(slpTemplate.cmd_table_offset_);
        slpTemplate.cmd_table_offset_ += templateBegin;
//        std::cout << "cmd offset: " << slpTemplate.cmd_table_offset_ << std::endl;
//        std::cout << "pos " << tellg() << std::endl;

        getIStream()->seekg(nextPos);
    }

    for (SlpTemplate &slpTemplate : templates_) {
        getIStream()->seekg(std::streampos(slpTemplate.outline_table_offset_));

        slpTemplate.left_edges_.resize(slpTemplate.height_);
        slpTemplate.right_edges_.resize(slpTemplate.height_);
//        serialize(slpTemplate.left_edges_, slpTemplate.height_);
//        serialize(slpTemplate.right_edges_, slpTemplate.height_);
        for (uint32_t row = 0; row < slpTemplate.height_; ++row) {
            serialize(slpTemplate.left_edges_[row]);
            serialize(slpTemplate.right_edges_[row]);
//            std::cout << slpTemplate.left_edges_[row] << std::endl;
        }

//        getIStream()->seekg(slpTemplate.cmd_table_offset_);
        getIStream()->seekg(std::streampos(slpTemplate.cmd_table_offset_));
        serialize(slpTemplate.cmd_offsets_, slpTemplate.height_);
//        for (uint32_t &ofst : slpTemplate.cmd_offsets_) {
//            std::cout << "# " << ofst << std::endl;
//        }
    }

//    std::cout << tellg() << std::endl;

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

    for (SlpTemplate &slpTemplate : templates_) {
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
}


//------------------------------------------------------------------------------
SlpFramePtr SlpTemplateFile::getFrame(const SlpFilePtr &baseFile, const Slope slope)
{
    if (!baseFile) {
        log.error("Passed nullptr");
        return nullptr;
    }
    if (!icmFile) {
        log.error("No ICM file loaded");
        return nullptr;
    }
    if (!patternmasksFile) {
        log.error("No pattern masks file loaded");
        return nullptr;
    }
    if (!filtermapFile) {
        log.error("No filter map file loaded");
        return nullptr;
    }

    SlpFramePtr frameCopy = std::make_shared<SlpFrame>(*baseFile->getFrame(0));
    frameCopy->setLoadParams(*baseFile->getIStream());
    frameCopy->setSlpFilePos(baseFile->getInitialReadPosition());
//    std::cout << "================= " << frameCopy->width_ << std::endl;
    frameCopy->setSize(templates_[slope].width_, templates_[slope].height_);
    frameCopy->hotspot_x = templates_[slope].hotspot_x;
    frameCopy->hotspot_y = templates_[slope].hotspot_y;

    frameCopy->cmd_offsets_ = templates_[slope].cmd_offsets_;
    frameCopy->left_edges_ = templates_[slope].left_edges_;
    frameCopy->right_edges_ = templates_[slope].right_edges_;

    frameCopy->readImage();

    return frameCopy;
}

}
