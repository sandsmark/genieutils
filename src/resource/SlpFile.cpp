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
    //log.info("Loaded SLP file");
  }
}

//------------------------------------------------------------------------------
void SlpFile::loadFile()
{
  //log.info("Header beg [%d]", tellg() - getInitialReadPosition());
  readHeader();
  //log.info("Header end [%d]", tellg() - getInitialReadPosition());

  frames_.resize(num_frames_);

  // Load frame headers
  for (uint32_t i = 0; i < num_frames_; ++i)
  {
    //log.info("Frame [%d] info beg [%d]", i, tellg() - getInitialReadPosition());
    frames_[i] = SlpFramePtr(new SlpFrame());
    frames_[i]->loadHeader(*getIStream());
    frames_[i]->setSlpFilePos(getInitialReadPosition());
  }
  //log.info("Frame info end [%d]", tellg() - getInitialReadPosition());

  // Load frame content
  unsigned int fid = 0;
  //for (FrameVector::iterator it = frames_.begin(); it != frames_.end(); ++it)
  for (uint32_t i = 0; i < num_frames_; ++i)
  {
    //log.info("Frame [%d] content beg [%d]", fid++, tellg() - getInitialReadPosition());
    //(*it)->load(*getIStream());
    frames_[i]->load(*getIStream());
  }
  //log.info("Frame content end [%d]", tellg() - getInitialReadPosition());

  loaded_ = true;
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
// int32_t SlpFile::getId()
// {
//   return id_;
// }

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
void SlpFile::readHeader()
{
  std::string version = readString(4);
  num_frames_ = read<uint32_t>();

  std::string comment = readString(24);
}

}
