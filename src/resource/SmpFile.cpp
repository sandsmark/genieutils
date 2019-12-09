/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2018  Mikko "Tapsa" P

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

#include "genie/resource/SmpFile.h"

#include <stdexcept>
#include <chrono>

#include "genie/resource/SmpFrame.h"
#include "genie/resource/PalFile.h"

namespace genie
{

Logger& SmpFile::log = Logger::getLogger("genie.SmpFile");

//------------------------------------------------------------------------------
SmpFile::SmpFile() : IFile()
{
}

//------------------------------------------------------------------------------
SmpFile::~SmpFile()
{
}

//------------------------------------------------------------------------------
void SmpFile::serializeObject(void)
{
  if (isOperation(OP_READ) && !loaded_)
  {
    loadFile();
  }
  else if (isOperation(OP_WRITE))// && loaded_)
  {
    saveFile();
  }
}

//------------------------------------------------------------------------------
void SmpFile::loadFile()
{
  serializeHeader();

  frames_.resize(num_frames_);

  serialize<uint32_t>(frame_offsets_, num_frames_);

  // Load frame headers
  for (uint32_t i = 0; i < num_frames_; ++i)
  {
    frames_[i] = SmpFramePtr(new SmpFrame());
    frames_[i]->setLoadParams(*getIStream(), frame_offsets_[i] + getInitialReadPosition());
    frames_[i]->serializeHeader();
  }

  // Load frame content
  for (uint32_t i = 0; i < num_frames_; ++i)
  {
    frames_[i]->load(*getIStream());
  }

  loaded_ = true;
}

//------------------------------------------------------------------------------
void SmpFile::saveFile()
{
}

//------------------------------------------------------------------------------
void SmpFile::unload(void)
{
  if (!loaded_)
    log.warn("Trying to unload a not loaded smpfile!");

  frames_.clear();
  num_frames_ = 0;

  loaded_ = false;
}

//------------------------------------------------------------------------------
bool SmpFile::isLoaded(void) const
{
  return loaded_;
}

//------------------------------------------------------------------------------
uint32_t SmpFile::getFrameCount(void)
{
  return frames_.size();
}

//------------------------------------------------------------------------------
void SmpFile::setFrameCount(uint32_t count)
{
  frames_.resize(count);
  num_frames_ = count;
}

//------------------------------------------------------------------------------
SmpFramePtr SmpFile::getFrame(uint32_t frame)
{
  if (frame >= frames_.size())
  {
    if (!loaded_)
    {
#ifndef NDEBUG
      log.debug("Reloading SMP, seeking frame [%u]", frame);
#endif
      readObject(*getIStream());
      return getFrame(frame);
    }
    log.error("Trying to get frame [%u] from index out of range!", frame);
    throw std::out_of_range("getFrame()");
  }

  return frames_[frame];
}

//------------------------------------------------------------------------------
void SmpFile::setFrame(uint32_t frame, SmpFramePtr data)
{
  if (frame < frames_.size())
  {
    frames_[frame] = data;
  }
}

//------------------------------------------------------------------------------
void SmpFile::serializeHeader()
{
  serialize(signature, 4);
  serialize<uint32_t>(version);
  serializeSize<uint32_t>(num_frames_, frames_.size());
  serialize<uint32_t>(num_animations);
  serialize<uint32_t>(num_frames_per_animation);
  serialize<uint32_t>(checksum);
  serialize<uint32_t>(size);
  serialize<uint32_t>(resource_id);
  serialize(comment, 32);
}

}
