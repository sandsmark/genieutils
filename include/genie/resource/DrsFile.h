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

#ifndef GENIE_DRSFILE_H
#define GENIE_DRSFILE_H

#include <vector>
#include <list>
#include <unordered_map>
#include <stdint.h>

#include "genie/file/IFile.h"
#include "SlpFile.h"
#include "BinaFile.h"
#include "UIFile.h"

namespace genie {

class Logger;

//------------------------------------------------------------------------------
/// Base class for .drs files
//
class DrsFile : public IFile
{
    static constexpr char binaryTableHeader[] = "anib";
    static constexpr char soundTableHeader[] = " vaw";
    static constexpr char slpTableHeader[] = " pls";

public:
    //----------------------------------------------------------------------------
    /// Default Constructor.
    //
    DrsFile();

    //----------------------------------------------------------------------------
    /// Destructor
    //
    virtual ~DrsFile();

    //----------------------------------------------------------------------------
    /// Get a shared pointer to a slp file.
    ///
    /// @param id resource id
    /// @return slp file pointer or "empty" shared pointer if not found
    //
    SlpFilePtr getSlpFile(uint32_t id);

    //----------------------------------------------------------------------------
    /// Get a shared pointer to a color palette file.
    ///
    /// @param id resource id
    /// @return bina file pointer or "empty" shared pointer if not found
    //
    const PalFile &getPalFile(uint32_t id);
    UIFilePtr getUIFile(uint32_t id);
    UIFilePtr getUIFile(const std::string &knownName);
    BmpFilePtr getBmpFile(uint32_t id);
    std::string getScriptFile(uint32_t id);
    ScnFilePtr getScnFile(uint32_t id);

    std::string idType(uint32_t id);

    std::shared_ptr<uint8_t[]> getWavPtr(uint32_t id);

    std::vector<uint32_t> binaryFileIds() const;

  std::vector<uint32_t> slpFileIds() const;

private:
    static Logger &log;

    bool header_loaded_ = false;

    uint32_t num_of_tables_;
    uint32_t header_offset_;

    std::vector<std::string> table_types_;
    std::vector<uint32_t> table_num_of_files_;

    std::unordered_map<uint32_t, std::shared_ptr<PalFile>> pal_files_;

    std::unordered_map<uint32_t, SlpFilePtr> slp_map_;
    std::unordered_map<uint32_t, BinaFilePtr> bina_map_;
    std::unordered_map<uint32_t, uint32_t> wav_offsets_;

    size_t copyrightHeaderSize() const;

    //----------------------------------------------------------------------------
    /// Loads table and resource headers.
    //
    void loadHeader();

    void serializeObject() override;
};
}

#endif // GENIE_DRSFILE_H
