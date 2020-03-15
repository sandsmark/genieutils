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
#include "genie/util/Logger.h"

namespace genie {

Logger &BinaFile::log = Logger::getLogger("genie.BinaFile");

BinaFile::BinaFile(uint32_t size) :
    m_size(size)
{
}

std::shared_ptr<PalFile> BinaFile::readPalFile(std::istream *istr)
{
    std::shared_ptr<PalFile> pal = std::make_shared<PalFile>();

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
    istr->seekg(getInitialReadPosition());
    const char b = istr->get();
    const char m = istr->get();
    istr->seekg(getInitialReadPosition());

    if (b != 'B' || m != 'M') {
        std::cerr << "Invalid bmp header" << std::endl;
        return nullptr;
    }

#ifdef SHITTY_PLATFORM
#warning Get a better computer
    BmpFilePtr file(reinterpret_cast<uint8_t*>(malloc(m_size)));
#else
    BmpFilePtr file = std::make_shared<uint8_t[]>(m_size);
#endif
    uint8_t *data = file.get();
    read(&data, m_size);

    return file;
}

std::string BinaFile::readScriptFile(std::istream *istr)
{
    istr->seekg(getInitialReadPosition());

    std::vector<char> content(m_size);
    istr->read(content.data(), m_size);
    size_t readCount = istr->gcount();

    if (!readCount) {
        log.error("Invalid file, it was empty");
        return std::string();
    }

    if (content[0] != ';' && content[0] != '/' && content[0] != '(' && content[0] != ' ' && content[0] != '\t' && content[0] != '#') {
        log.error("This doesn't look like a valid script file, starts with %d", int(content[0]));
    }

    return std::string(content.data(), readCount);
}

ScnFilePtr BinaFile::readScnFile(std::istream *istr)
{
    ScnFilePtr scnFile(new ScnFile());

    scnFile->setInitialReadPosition(getInitialReadPosition());

    scnFile->readObject(*istr);
    return scnFile;
}

std::string BinaFile::filetype(std::istream *istr)
{
    if (m_size < 4) {
        return "size less than 4 bytes";
    }

    istr->seekg(getInitialReadPosition());

    char content[17];
    istr->read(content, 17);
    size_t readCount = istr->gcount();

    if (readCount < 4) {
        return "corrupted";
    }

    if (content[0] == 'J' && content[1] == 'A' && content[2] == 'S' && content[3] == 'C') {
        return "palette";
    }

    if (content[0] == 'B' && content[1] == 'M') {
        return "bmp";
    }

    if (content[0] >= '0' && content[0] <= '9' && content[1] == '.' && content[2] >= '0' && content[2] <= '9' && content[3] >= '0' && content[3] <= '9') {
        return "scenario file version " + std::string(content, 4);
    }

    if (content[0] == ';' || content[0] == '/' || content[0] == '(' || content[0] == ' ' || content[0] == '\t' || content[0] == '#') {
        return "script file? (" + std::string(content, 4) + ")";
    }

    if (std::string(content, std::min(size_t(17), readCount)) == std::string("background1_files")) {
        return "UI file";
    }

    if (std::string(content, std::min(size_t(17), readCount)) == std::string("0\r\n1\r\n2\r\n3\r\n4\r\n5\r")) {
        return "counting file?";
    }

    if (content[0] == '1' && content[1] == ' ' && content[2] == ' ' && content[3] == ' ') {
        return "campaign button location data";
    }

    return "unknown (" + std::string(content, readCount) + ")";
}

void BinaFile::serializeObject()
{
}
} // namespace genie
