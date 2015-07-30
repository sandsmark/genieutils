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
  header_loaded_ = false;
}


//------------------------------------------------------------------------------
DrsFile::~DrsFile()
{
}

//------------------------------------------------------------------------------
SlpFilePtr DrsFile::getSlpFile(uint32_t id)
{
  SlpMap::iterator i = slp_map_.find(id);

  if (i != slp_map_.end())
  {
    i->second->readObject(*getIStream());
    return i->second;
  }
  else
  {
    log.warn("No slp file with id [%d] found!", id);
    return SlpFilePtr();
  }
}

//------------------------------------------------------------------------------
SlpFile* DrsFile::getSlp(uint32_t id)
{
  SlpMap::iterator i = slp_map_.find(id);

  log.info("Searching for slp with id [%u]", id);
  if (i != slp_map_.end())
  {
    i->second->readObject(*getIStream());
    return (i->second).get();
  }
  else
  {
    log.warn("No slp file with id [%d] found!", id);
    return new SlpFile();
  }
}

//------------------------------------------------------------------------------
PalFilePtr DrsFile::getPalFile(uint32_t id)
{
  BinaMap::iterator i = bina_map_.find(id);

  if (i != bina_map_.end())
  {
    return i->second->readPalFile(getIStream());
  }
  else
  {
    log.warn("No bina file with id [%d] found!", id);
    return PalFilePtr();
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
        // else other TODO: Sounds

      }
    }

    header_loaded_ = true;
  }
}

}

