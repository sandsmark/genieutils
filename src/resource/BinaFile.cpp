/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Armin Preiml

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

#include "genie/resource/BinaFile.h"

namespace genie
{
  
BinaFile::BinaFile(uint32_t size) :
    m_size(size)
{
}
  
BinaFile::~BinaFile()
{
}

PalFilePtr BinaFile::readPalFile(std::istream *istr)
{
  PalFilePtr pal(new PalFile());
  
  pal->setInitialReadPosition(getInitialReadPosition());
  
  pal->readObject(*istr);
  
  return pal;
}

UIFilePtr BinaFile::readUIFile(std::istream *istr)
{
  UIFilePtr uifile(new UIFile());

  uifile->setInitialReadPosition(getInitialReadPosition());

  uifile->readObject(*istr);
  return uifile;
}

BmpFilePtr BinaFile::readBmpFile(std::istream *istr)
{
    std::istream::pos_type pos = istr->tellg();
    const char b = istr->get();
    const char m = istr->get();
    istr->seekg(pos);

    if (b != 'B' || m != 'M') {
        std::cerr << "Invalid bmp header" << std::endl;
        return nullptr;
    }

    BmpFilePtr file(new char[m_size]);
    istr->read(file.get(), m_size);

    return file;
}

void BinaFile::serializeObject(void)
{
}

}
