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

#include "genie/resource/DrsFile.h"

#include <string>

#include "genie/util/Logger.h"
#include "genie/file/ISerializable.h"

//#include <file/BinaFile.h>

namespace genie
{

using std::string;

Logger& DrsFile::log = Logger::getLogger("freeaoe.DrsFile");

//------------------------------------------------------------------------------
DrsFile::DrsFile()
{
}

//------------------------------------------------------------------------------
DrsFile::~DrsFile()
{
}

//------------------------------------------------------------------------------
SlpFilePtr DrsFile::getSlpFile(uint32_t id)
{
  auto i = slp_map_.find(id);

  if (i != slp_map_.end())
  {
#ifndef NDEBUG
    log.debug("Loading SLP file [%u]", id);
#endif
    i->second->readObject(*getIStream());
    return i->second;
  }
  else
  {
    log.warn("No slp file with id [%u] found!", id);
    return SlpFilePtr();
  }
}

//------------------------------------------------------------------------------
PalFilePtr DrsFile::getPalFile(uint32_t id)
{
  auto i = bina_map_.find(id);

  if (i != bina_map_.end())
  {
    return i->second->readPalFile(getIStream());
  }
  else
  {
    log.warn("No bina file with id [%u] found!", id);
    return PalFilePtr();
  }
}

//------------------------------------------------------------------------------
unsigned char* DrsFile::getWavPtr(uint32_t id)
{
  auto i = wav_offsets_.find(id);

  if (i != wav_offsets_.end())
  {
    wav_file_.clear();
    getIStream()->seekg(std::streampos(i->second));
    uint32_t type = read<uint32_t>();
    uint32_t size = read<uint32_t>();
#ifndef NDEBUG
    log.debug("WAV [%u], type [%X], size [%u]", id, type, size);
#endif
    getIStream()->seekg(std::streampos(i->second));
    wav_file_.resize(size + 8);
    for (auto &pos: wav_file_)
    {
      pos = read<uint8_t>();
    }
    return wav_file_.data();
  }
  else
  {
    log.warn("No sound file with id [%u] found!", id);
    return NULL;
  }
}

//------------------------------------------------------------------------------
void DrsFile::serializeObject(void)
{
  loadHeader();
}

//------------------------------------------------------------------------------
unsigned int DrsFile::getCopyRightHeaderSize(void) const
{
  if (getGameVersion() >= GV_SWGB)
    return 0x3C;
  else
    return 0x28;
}

//------------------------------------------------------------------------------
std::string DrsFile::getSlpTableHeader(void) const
{
  return " pls";
}

//------------------------------------------------------------------------------
std::string DrsFile::getBinaryTableHeader(void) const
{
  return "anib";
}

//------------------------------------------------------------------------------
std::string DrsFile::getSoundTableHeader(void) const
{
  return " vaw";
}

//------------------------------------------------------------------------------
void DrsFile::loadHeader()
{
  if (header_loaded_)
    log.warn("Trying to load header again!");
  else
  {
    string copy_right = readString(getCopyRightHeaderSize());

    string version = readString(4);

    //File type
    string file_type = readString(12);

    num_of_tables_ = read<uint32_t>();
    header_offset_ = read<uint32_t>();

    // Load table data
    for (uint32_t i = 0; i < num_of_tables_; ++i)
    {
      table_types_.push_back(readString(4));
      read<uint32_t>(); // TODO: Unknown
      table_num_of_files_.push_back(read<uint32_t>());
    }

    // Load file headers
    for (uint32_t i = 0; i < num_of_tables_; ++i)
    {
      for (uint32_t j = 0; j < table_num_of_files_[i]; ++j)
      {
        uint32_t id = read<uint32_t>();
        uint32_t pos = read<uint32_t>();
        /*uint32_t len =*/ read<uint32_t>();

        if (table_types_[i].compare(getSlpTableHeader()) == 0)
        {
          SlpFilePtr slp(new SlpFile());
          slp->setInitialReadPosition(pos);

          slp_map_[id] = slp;
        }
        else if (table_types_[i].compare(getBinaryTableHeader()) == 0)
        {
          BinaFilePtr bina(new BinaFile());
          bina->setInitialReadPosition(pos);

          bina_map_[id] = bina;
        }
        else if (table_types_[i].compare(getSoundTableHeader()) == 0)
        {
          wav_offsets_[id] = pos;
        }
      }
    }

    header_loaded_ = true;
  }
}

}
