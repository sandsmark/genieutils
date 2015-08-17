/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Armin Preiml
    Copyright (C) 2015  Mikko "Tapsa" P

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

#include "genie/resource/SlpFrame.h"
#include "genie/resource/PalFile.h"

namespace genie
{

Logger& SlpFile::log = Logger::getLogger("genie.SlpFile");

//------------------------------------------------------------------------------
SlpFile::SlpFile() : IFile()
{
  loaded_ = false;
  num_frames_ = 0;
  version = "2.0N";
  comment = "genieutils";
}

//------------------------------------------------------------------------------
SlpFile::~SlpFile()
{
}

//------------------------------------------------------------------------------
void SlpFile::serializeObject(void)
{
  if (isOperation(OP_READ) && !loaded_)
  {
    loadFile();
  }
  else if (isOperation(OP_WRITE) && loaded_)
  {
    saveFile();
  }
}

//------------------------------------------------------------------------------
void SlpFile::loadFile()
{
  serializeHeader();

  frames_.resize(num_frames_);

  // Load frame headers
  for (uint32_t i = 0; i < num_frames_; ++i)
  {
    frames_[i] = SlpFramePtr(new SlpFrame());
    frames_[i]->setLoadParams(*getIStream());
    frames_[i]->serializeHeader();
    frames_[i]->setSlpFilePos(getInitialReadPosition());
  }

  // Load frame content
  for (uint32_t i = 0; i < num_frames_; ++i)
  {
    frames_[i]->load(*getIStream());
  }

  loaded_ = true;
}

//------------------------------------------------------------------------------
void SlpFile::saveFile()
{
  serializeHeader();

  // Write frame headers
  for (uint32_t i = 0; i < num_frames_; ++i)
  {
    frames_[i]->setSaveParams(*getOStream());
    frames_[i]->serializeHeader();
  }

  // Write frame content
  for (uint32_t i = 0; i < num_frames_; ++i)
  {
    //frames_[i]->save(getOStream);
  }
}

//------------------------------------------------------------------------------
void SlpFile::unload(void )
{
  if (!loaded_)
    log.warn("Trying to unload a not loaded slpfile!");

  frames_.clear();
  num_frames_ = 0;

  loaded_ = false;
}

//------------------------------------------------------------------------------
bool SlpFile::isLoaded(void ) const
{
  return loaded_;
}

//------------------------------------------------------------------------------
uint32_t SlpFile::getFrameCount()
{
  return num_frames_;
}

//------------------------------------------------------------------------------
SlpFramePtr SlpFile::getFrame(uint32_t frame)
{
  if (frame >= frames_.size())
  {
    log.error("Trying to get frame [%u] from index out of range!", frame);
    throw std::out_of_range("getFrame()");
  }

  return frames_[frame];
}

//------------------------------------------------------------------------------
void SlpFile::serializeHeader()
{
  serialize(version, 4);
  serialize<uint32_t>(num_frames_);
  serialize(comment, 24);
}

}
