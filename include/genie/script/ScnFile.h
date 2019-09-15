/*
    genieutils - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2015  Mikko "Tapsa" P
    Copyright (C) 2015  JustTesting1234

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

#ifndef GENIE_SCNFILE_H
#define GENIE_SCNFILE_H

#include <stdint.h>

#include "genie/file/IFile.h"
#include "genie/file/Compressor.h"

#include "scn/ScnPlayerData.h"
#include "scn/MapDescription.h"
#include "scn/Trigger.h"

namespace genie {

class Logger;

// Used for blending between screens in the cutscenes/cinematics in the campaigns
// This is just based on some random post on aokheaven, but I haven't found a better description
// http://aok.heavengames.com/cgi-bin/forums/display.cgi?action=ct&f=9,44036,0,30

//   BLN files are used for fading the Multimedia Cinematic Screens.
//   This type of file is compressed from start to end (does not contain uncompressed header, ect).
//   I have named them "Blender" files since they are used for fading the color indices.
//    - Offset 0x0000 { (float) Version [1.0] }
//    - Offset 0x0004 { 20 Frames * 256 Indexes * 256 Palettes }
//   The total inflated length of these files (as of the currently used Version 1.0) should measure 4+(20*256*256)=1,310,724 bytes.
// --
// AOHH
class BlnFile : public IFile
{
public:
    struct Palette {
        std::vector<uint8_t> colors;
    };
    struct Frame {
        std::array<Palette, 256> palettes;
    };
    BlnFile();

    float version = 0.f;
    std::array<Frame, 20> frames;

private:
    void serializeObject() override;
    Compressor compressor_;
};

class ScnPersonalityScript : public ISerializable
{
public:
    ScnPersonalityScript();
    ~ScnPersonalityScript() override;

    std::string filename;
    std::string content;

private:
    void serializeObject(void) override;
};

//------------------------------------------------------------------------------
/// Class to read and write scenario files (.scn and .scx). The version of the
/// game will be auto-detected on read.
//
class ScnFile : public IFile
{
public:
    ScnFile();

    //----------------------------------------------------------------------------
    /// Extracts a scenario (for debugging purpose).
    //
    void extractRaw(const char *from, const char *to);

    static uint32_t getSeparator(void);

    std::string version;

    // Uncompressed Header:

    int32_t saveType;

    /// Timestamp of last save
    uint32_t lastSaveTime;

    std::string scenarioInstructions;

    int32_t victoryType = 0;

    uint32_t enabledPlayerCount;

    // Compressed header:

    /// aokts description: "Next unit ID to place" ??
    uint32_t nextUnitID;

    ScnMainPlayerData playerData;

    ScnMap map;

    std::vector<ScnPlayerResources> playerResources;
    std::vector<ScnPlayerUnits> playerUnits;

    std::vector<ScnMorePlayerData> players;

    double triggerVersion;
    int8_t objectivesStartingState;
    std::vector<Trigger> triggers;
    std::vector<int32_t> triggerDisplayOrder;

    uint32_t includeFiles;
    uint32_t perErrorIncluded;
    std::vector<uint32_t> perError;
    std::vector<ScnPersonalityScript> includedFiles;

private:
    static Logger &log;
    bool verifyVersion();

    uint32_t headerLength_; //starting after this
    uint32_t playerUnitsCount;
    uint32_t playerCount2_;
    uint32_t numTriggers_;
    uint32_t fileCount_;

    Compressor compressor_;

    void serializeObject(void) override;

    void serializeVersion(void);
};

typedef std::shared_ptr<ScnFile> ScnFilePtr;

// CPN files are basically identical it seems, but with a different compression in the scn files (which breaks)
// Might also have more data in the header
class CpxIncludedFile : public ISerializable
{
public:
    std::string identifier;
    std::string filename;
    uint32_t size;
    uint32_t offset;

    ScnFilePtr getScnFile();

private:
    void serializeObject(void) override;
};


class CpxFile : public IFile
{
public:
    void serializeObject(void) override;

    std::vector<std::string> getFilenames() const;
    ScnFilePtr getScnFile(const std::string &filename);
    ScnFilePtr getScnFile(size_t index);

    std::string version;
    std::string name;

private:
    uint32_t filecount = 0;
    std::vector<CpxIncludedFile> m_files;
};

}

#endif // GENIE_SCNFILE_H
