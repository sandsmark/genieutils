/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Armin Preiml
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

#include "genie/resource/SlpFile.h"

#include <stdexcept>
#include <chrono>

#include "genie/resource/SlpFrame.h"
#include "genie/resource/PalFile.h"

namespace genie {

Logger &SlpFile::log = Logger::getLogger("genie.SlpFile");

//------------------------------------------------------------------------------
SlpFile::SlpFile(const size_t size) :
    IFile(),
    size_(size)
{
}

//------------------------------------------------------------------------------
void SlpFile::serializeObject()
{
    if (isOperation(OP_READ) && !loaded_) {
        loadFile();
    } else if (isOperation(OP_WRITE)) { // && loaded_)
        saveFile();
    }
}

//------------------------------------------------------------------------------
void SlpFile::loadFile()
{
    serializeHeader();

    frames_.resize(num_frames_);

    if (m_graphicsFileData.empty()) {
        m_graphicsFileData.resize(size_, 0);
        std::streampos orig = getIStream()->tellg();
        getIStream()->seekg(getInitialReadPosition());
        char *data = reinterpret_cast<char *>(m_graphicsFileData.data());
        getIStream()->read(data, size_);
        getIStream()->seekg(orig);
    } else {
        std::cerr << "already loaded data" << std::endl;
    }

    // Load frame headers
    for (uint32_t i = 0; i < num_frames_; ++i) {
        frames_[i] = SlpFramePtr(new SlpFrame());
        frames_[i]->setSlpFilePos(std::streampos(0));
        frames_[i]->setLoadParams(*getIStream());
        frames_[i]->serializeHeader();
    }

    const char *data = reinterpret_cast<const char *>(m_graphicsFileData.data());
    std::istringstream istr(std::string(data, m_graphicsFileData.size()));

    // Load frame header
    for (uint32_t i = 0; i < num_frames_; ++i) {
        frames_[i]->load(istr);
    }

    loaded_ = true;
}

//------------------------------------------------------------------------------
void SlpFile::saveFile()
{
#ifndef NDEBUG
    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
#endif
    serializeHeader();
    slp_offset_ = 32 + 32 * num_frames_;

    // Write frame headers
    for (uint32_t i = 0; i < num_frames_; ++i) {
        frames_[i]->setSaveParams(*getOStream(), slp_offset_);
        frames_[i]->serializeHeader();
    }

    // Write frame content
    for (uint32_t i = 0; i < num_frames_; ++i) {
        frames_[i]->save(*getOStream());
    }

#ifndef NDEBUG
    std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now();
    log.debug("SLP (%u bytes) saving took [%u] milliseconds", slp_offset_, std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count());
#endif
}

//------------------------------------------------------------------------------
void SlpFile::unload()
{
    if (!loaded_) {
        log.warn("Trying to unload a not loaded slpfile!");
    }

    frames_.clear();
    num_frames_ = 0;

    loaded_ = false;
}

//------------------------------------------------------------------------------
bool SlpFile::isLoaded() const
{
    return loaded_;
}

//------------------------------------------------------------------------------
uint32_t SlpFile::getFrameCount()
{
    return frames_.size();
}

//------------------------------------------------------------------------------
void SlpFile::setFrameCount(uint32_t count)
{
    frames_.resize(count);
    num_frames_ = count;
}

//------------------------------------------------------------------------------
const SlpFramePtr &SlpFile::getFrame(uint32_t frame)
{
    if (frame >= frames_.size()) {
        if (!loaded_) {
#ifndef NDEBUG
            log.debug("Reloading SLP, seeking frame [%u]", frame);
#endif
            readObject(*getIStream());
            return getFrame(frame);
        }

        log.error("Trying to get frame [%u] from index out of range!", frame);
        throw std::out_of_range("getFrame()");
    }

    if (frames_[frame]->img_data.pixel_indexes.empty()) {
        const char *data = reinterpret_cast<const char *>(m_graphicsFileData.data());
        std::istringstream istr(std::string(data, m_graphicsFileData.size()));
        frames_[frame]->setLoadParams(istr);
        frames_[frame]->readImage();

        if (frames_[frame]->getWidth() == 0) {
            log.debug("Got null frame");
        }

        frames_[frame]->setLoadParams(*getIStream());
    }

    return frames_[frame];
}

//------------------------------------------------------------------------------
void SlpFile::setFrame(uint32_t frame, SlpFramePtr data)
{
    if (frame < frames_.size()) {
        frames_[frame] = std::move(data);
    }
}

int SlpFile::frameCommandsOffset(const size_t frame, const int row)
{
    if (!loaded_) {
        readObject(*getIStream());
    }

    if (frame >= frames_.size()) {
        log.error("Trying to get frame [%u] from index out of range!", frame);
        throw std::out_of_range("getFrame()");
    }

    return frames_[frame]->commandsOffset(row);
}

int SlpFile::frameHeight(const size_t frame)
{
    if (!loaded_) {
        readObject(*getIStream());
    }

    if (frame >= frames_.size()) {
        log.error("Trying to get frame [%u] from index out of range!", frame);
        throw std::out_of_range("getFrame()");
    }

    return frames_[frame]->getHeight();
}

int SlpFile::frameWidth(const size_t frame)
{
    if (!loaded_) {
        readObject(*getIStream());
    }

    if (frame >= frames_.size()) {
        log.error("Trying to get frame [%u] from index out of range!", frame);
        throw std::out_of_range("getFrame()");
    }

    return frames_[frame]->getWidth();
}

//------------------------------------------------------------------------------
void SlpFile::serializeHeader()
{
    serialize(version, 4);
    serializeSize<uint32_t>(num_frames_, frames_.size());
    serialize(comment, 24);
}
} // namespace genie
