/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Armin Preiml
    Copyright (C) 2013 - 2019  Mikko "Tapsa" P

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

namespace genie {

using std::string;

Logger &DrsFile::log = Logger::getLogger("genie.DrsFile");

//------------------------------------------------------------------------------
SlpFilePtr DrsFile::getSlpFile(uint32_t id)
{
    std::unordered_map<uint32_t, SlpFilePtr>::iterator slpIterator = slp_map_.find(id);

    if (slpIterator != slp_map_.end()) {
#ifndef NDEBUG
        log.debug("Loading SLP file [%]", id);
#endif
        slpIterator->second->readObject(*getIStream());
        return slpIterator->second;
    } else {
        std::unordered_map<uint32_t, BinaFilePtr>::iterator binIterator = bina_map_.find(id);

        if (binIterator != bina_map_.end()) {
#ifndef NDEBUG
            log.debug("Loading SLP file [%] from bina", id);
#endif
            SlpFilePtr slp(new SlpFile(binIterator->second->size()));

            slp->setInitialReadPosition(binIterator->second->getInitialReadPosition());

            slp->readObject(*getIStream());

            return slp;
        } else {
            log.debug("No slp file with id [%] found!", id);
            return SlpFilePtr();
        }
    }
}

//------------------------------------------------------------------------------
const PalFile &DrsFile::getPalFile(uint32_t id)
{
    std::unordered_map<uint32_t, std::shared_ptr<PalFile>>::iterator i = pal_files_.find(id);

    if (i != pal_files_.end()) {
        return *i->second;
    }

    std::unordered_map<uint32_t, BinaFilePtr>::iterator b = bina_map_.find(id);

    if (b == bina_map_.end()) {
        log.debug("No bina file with id [%] found!", id);
        return PalFile::null;
    }

    pal_files_[id] = b->second->readPalFile(getIStream());
    return *pal_files_.find(id)->second;
}

UIFilePtr DrsFile::getUIFile(uint32_t id)
{
    std::unordered_map<uint32_t, BinaFilePtr>::iterator i = bina_map_.find(id);

    if (i != bina_map_.end()) {
        return i->second->readUIFile(getIStream());
    } else {
        log.debug("No bina file with id [%] found!", id);
        return UIFilePtr();
    }
}

UIFilePtr DrsFile::getUIFile(const std::string &knownName)
{
    static const std::unordered_map<std::string, int> idMap = {
        { "dlg_gam", 50018 },
        { "dlg_dip", 50014 },
        { "dlg_men.sin", 50015 },
        { "dlg_obj", 50016 },
        { "2logos.slp", 53012 },
        { "map1280.bmp", 50409},
        { "map1024.bmp", 50408},
        { "map800.bmp.bmp", 50411},
        { "main", 50081    },
        { "scr1", 50051    },
        { "scr2", 50052    },
        { "scr3", 50053    },
        { "scr2p", 50084   },
        { "scr4", 50054    },
        { "scr5", 50055    },
        { "scr6", 50056    },
        { "scr_cred", 50059},
        { "scr_stms", 50085},
        { "xcredits", 50088},
        { "xmain", 50089   },
    };

    if (idMap.find(knownName) != idMap.end()) {
        return getUIFile(idMap.at(knownName));
    } else {
        log.error("Failed to find known id for filename %", knownName);
        return nullptr;
    }
}

BmpFilePtr DrsFile::getBmpFile(uint32_t id)
{
    std::unordered_map<uint32_t, BinaFilePtr>::iterator i = bina_map_.find(id);

    if (i != bina_map_.end()) {
        return i->second->readBmpFile(getIStream());
    } else {
        log.debug("No bina file with id [%] found!", id);
        return BmpFilePtr();
    }
}

std::string DrsFile::getScriptFile(uint32_t id)
{
    std::unordered_map<uint32_t, BinaFilePtr>::iterator i = bina_map_.find(id);

    if (i != bina_map_.end()) {
        return i->second->readScriptFile(getIStream());
    } else {
        log.debug("No bina file with id [%] found!", id);
        return std::string();
    }
}

ScnFilePtr DrsFile::getScnFile(uint32_t id)
{
    std::unordered_map<uint32_t, BinaFilePtr>::iterator i = bina_map_.find(id);

    if (i != bina_map_.end()) {
        return i->second->readScnFile(getIStream());
    } else {
        log.debug("No bina file with id [%] found!", id);
        return ScnFilePtr();
    }
}

WavFilePtr DrsFile::getWavFile(uint32_t id)
{
    std::unordered_map<uint32_t, WavFilePtr>::iterator i = wav_map_.find(id);

    if (i != wav_map_.end()) {
        i->second->readObject(*getIStream());
        return i->second;
    } else {
        log.debug("No wav file with id [%] found!", id);
        return {};
    }

}

std::string DrsFile::idType(const uint32_t id)
{
//    if (id >= 50000 && id < 50100) {
//        return "sin";
//    }

    // Just a guess, but I think good one, since there's 256 entries, and most seem to have the
    // same value as the entry number, except at least one that has various values between 0 and 255
    // Need to test and visually confirm, and see what kind of effect I get, though
    if (id >= 50900 && id <= 50911) {
        return "palette-index-translation";
    }

    if (wav_offsets_.find(id) != wav_offsets_.end()) {
        return "wav";
    }

    if (slp_map_.find(id) != slp_map_.end()) {
        return "slp";
    }

    std::unordered_map<uint32_t, BinaFilePtr>::const_iterator i = bina_map_.find(id);

    if (i == bina_map_.end()) {
        return "unknown";
    }

    return i->second->guessFileExtension(getIStream());
}

std::iostream::pos_type DrsFile::fileSize(const uint32_t id)
{
    std::unordered_map<uint32_t, uint32_t>::const_iterator i = m_allSizes.find(id);
    if (i == m_allSizes.end()) {
        return -1;
    }

    return i->second;
}

std::iostream::pos_type DrsFile::fileOffset(const uint32_t id)
{
    std::unordered_map<uint32_t, uint32_t>::const_iterator i = m_allOffsets.find(id);
    if (i == m_allOffsets.end()) {
        return -1;
    }

    return i->second;
}

//------------------------------------------------------------------------------
std::shared_ptr<uint8_t[]> DrsFile::getWavPtr(uint32_t id)
{
    std::unordered_map<uint32_t, uint32_t>::iterator i = wav_offsets_.find(id);

    if (i != wav_offsets_.end()) {
        getIStream()->seekg(std::streampos(i->second));
#ifndef NDEBUG
        uint32_t type = read<uint32_t>();
#else // Avoid -Wunused-value
        read<uint32_t>();
#endif
        uint32_t size = read<uint32_t>();

        if (!size)  {
            return nullptr;
        }

#ifndef NDEBUG
        log.debug("WAV [%], type [%], size [%]", id, type, size);
#endif
        getIStream()->seekg(std::streampos(i->second));

        std::shared_ptr<uint8_t[]> ptr(new uint8_t[size + 32]);

        uint8_t *data = ptr.get();
        read(&data, size);
        WavFile::WavHeader *header = reinterpret_cast<WavFile::WavHeader*>(data);
        if (size < header->Subchunk2Size) {
            log.warn("Invalid filesize %, expected more than %", size, header->Subchunk2Size);
            return nullptr;
        }
        return ptr;
    } else {
        log.warn("No sound file with id [%] found!", id);
        return nullptr;
    }
}

std::vector<uint32_t> DrsFile::binaryFileIds() const
{
    std::vector<uint32_t> ret;

    for (const std::pair<const uint32_t, BinaFilePtr> &entry : bina_map_) {
        ret.push_back(entry.first);
    }

    return ret;
}

std::vector<uint32_t> DrsFile::slpFileIds() const
{
    std::vector<uint32_t> ret;

    for (const std::pair<const uint32_t, SlpFilePtr> &entry : slp_map_) {
        ret.push_back(entry.first);
    }

    return ret;
}

//------------------------------------------------------------------------------
void DrsFile::serializeObject()
{
    loadHeader();
}

//------------------------------------------------------------------------------
size_t DrsFile::copyrightHeaderSize() const
{
    if (getGameVersion() >= GV_SWGB) {
        return 0x3C;
    } else {
        return 0x28;
    }
}

//------------------------------------------------------------------------------
void DrsFile::loadHeader()
{
    if (header_loaded_) {
        log.warn("Trying to load header again!");
    } else {
        string copyright = readString(copyrightHeaderSize());
        string version = readString(4);
        string filetype = readString(12);

        log.debug("copyright: %", copyright);
        log.debug("version: %", version);
        log.debug("filetype: %", filetype);

        num_of_tables_ = read<uint32_t>();
        header_offset_ = read<uint32_t>();

        std::vector<std::streampos> table_offsets;

        // Load table data
        for (uint32_t i = 0; i < num_of_tables_; ++i) {
            table_types_.push_back(readString(4));
            table_offsets.emplace_back(read<uint32_t>());
            table_num_of_files_.push_back(read<uint32_t>());
        }

        // Load file headers
        for (uint32_t i = 0; i < num_of_tables_; ++i) {
            if (tellg() != table_offsets[i]) {
                log.error("Tables aren't layed out linearly, is at position %d, but should be at %d", tellg(), table_offsets[i]);
            }

            for (uint32_t j = 0; j < table_num_of_files_[i]; ++j) {
                uint32_t id = read<uint32_t>();
                uint32_t pos = read<uint32_t>();
                uint32_t len = read<uint32_t>();

                m_allIds.push_back(id);
                m_allSizes[id] = len;
                m_allOffsets[id] = pos;

                if (table_types_[i].compare(slpTableHeader) == 0) {
                    SlpFilePtr slp(new SlpFile(len));
                    slp->setInitialReadPosition(pos);

                    slp_map_[id] = slp;
                } else if (table_types_[i].compare(binaryTableHeader) == 0) {
                    BinaFilePtr bina(new BinaFile(len));
                    bina->setInitialReadPosition(pos);

                    bina_map_[id] = bina;
                } else if (table_types_[i].compare(soundTableHeader) == 0) {
                    wav_offsets_[id] = pos;

                    WavFilePtr wav = std::make_shared<WavFile>();
                    wav->setInitialReadPosition(pos);

                    wav_map_[id] = wav;
                } else {
                    std::cerr << "unknown header " << std::hex << table_types_[i] << std::dec << std::endl;
                    return;
//                    log.error("unknown file header: %", table_types_[i]);
                }
            }
        }

        header_loaded_ = true;
    }
}
} // namespace genie
