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

#pragma once

#include <istream>
#include <vector>

#include "genie/file/IFile.h"
#include "genie/util/Logger.h"
#include "PalFile.h"
#include "SlpFrame.h"
#include "SlpFile.h"

namespace genie {

//------------------------------------------------------------------------------
/// Basically a patch for an SLP frame, for sloped terrain
/// Overwrites the header (size, hotspot and command tables)
//
class SlpTemplateFile : public IFile
{
    struct SlpTemplate {
        uint32_t width_;
        uint32_t height_;

        int32_t hotspot_x;
        int32_t hotspot_y;

        int32_t data_size;

        std::vector<unsigned short> left_edges_;
        std::vector<unsigned short> right_edges_;

        std::vector<uint32_t> cmd_offsets_;

        uint32_t outline_table_offset_;
        uint32_t cmd_table_offset_;
    };

public:
    enum Slope {
        SlopeFlat        = 0,
        SlopeSouthUp     = 1,
        SlopeNorthUp     = 2,
        SlopeWestUp      = 3,
        SlopeEastUp      = 4,
        SlopeSouthWestUp = 5,
        SlopeNorthWestUp = 6,
        SlopeSouthEastUp = 7,
        SlopeNorthEastUp = 8,
        SLOPE_S_UP2      = 9,    // what are these?
        SLOPE_N_UP2      = 10,
        SLOPE_W_UP2      = 11,
        SLOPE_E_UP2      = 12,
        SlopeNorthDown   = 13,
        SlopeSouthDown   = 14,
        SlopeWestDown    = 15,
        SlopeEastDown    = 16,

        SlopeCount       = 17,

        SlopeSouthWestEastUp = SlopeNorthDown,
        SlopeNorthWestEastUp = SlopeSouthDown,
        SlopeNorthSoutEastUp = SlopeWestDown,
        SlopeNorthSouthWestUp = SlopeWestDown,
    };

    //----------------------------------------------------------------------------
    /// Constructor
    //
    SlpTemplateFile();

    //----------------------------------------------------------------------------
    /// Destructor
    //
    virtual ~SlpTemplateFile();

    //----------------------------------------------------------------------------
    /// Frees all content of a slp file.
    //
    void unload(void);

    //----------------------------------------------------------------------------
    /// Check whether the files content is loaded or not.
    //
    bool isLoaded(void) const;

    //----------------------------------------------------------------------------
    /// Returns the slp frame at given frame index.
    ///
    /// @param frame frame index
    /// @return SlpFrame
    //
    SlpFramePtr getFrame(const SlpFilePtr &baseFile, const Slope slope);

private:
    static Logger &log;

    bool loaded_ = false;

    //----------------------------------------------------------------------------
    virtual void serializeObject(void);

    //----------------------------------------------------------------------------
    /// Loads the file and its frames.
    //
    void loadFile(void);
    void saveFile(void);

    std::array<SlpTemplate, SlopeCount> templates_;
};

typedef std::shared_ptr<SlpTemplateFile> SlpTemplateFilePtr;
}
