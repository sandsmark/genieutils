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

#pragma once

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
        std::array<uint8_t, 256> colors;
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
    std::string filename;
    std::string content;

private:
    void serializeObject() override;
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
    void extractRaw(std::istream &ifs, std::ostream &ofs);

    static uint32_t getSeparator();

    uint32_t getFileCount() const { return fileCount_; }

    std::string version;

    // Uncompressed Header:

    enum SaveType : int32_t {
        Scenario = 2, // SCN, SCX, SC1, SCX2
        AoE2Scenario = 3, // African Kingdoms
    };

    int32_t saveType = Scenario;

    /// Timestamp of last save
    uint32_t lastSaveTime = 0;

    std::string scenarioInstructions;

    int32_t victoryType = 0;

    uint32_t enabledPlayerCount = 0;

    /// African Kingdoms apparently
    enum class ExpansionDataset { // stolen from aokts
        AOK_xUnk0 = 0,
        AOC_xUnk1 = 1,
        Unk_xAOK  = 2,
        Unk_xAOC  = 3,
        Unk_xAOF  = 4,
        Unk_xAOAK = 5
    };

    uint32_t aoe2ScenUnknown = 0;
    uint32_t usesExpansions = 0;
    uint32_t expansionsCount = 0;
    std::vector<uint32_t> expansions;

    // Compressed header:

    /// aokts description: "Next unit ID to place" ??
    uint32_t nextUnitID = 0;

    ScnMainPlayerData playerData{};

    ScnMap map;

    std::array<ScnPlayerResources, 8> playerResources;
    std::vector<ScnPlayerUnits> playerUnits;

    std::array<ScnMorePlayerData, 8> players;

    double triggerVersion = 0.;
    int8_t objectivesStartingState = 0;
    std::vector<Trigger> triggers;
    std::vector<int32_t> triggerDisplayOrder;

    uint32_t includeFiles = 0;
    uint32_t perErrorIncluded = 0;
    std::array<uint32_t, 99> perError{};
    std::vector<ScnPersonalityScript> includedFiles;

    bool verbose = false;
    bool assumeSwgb = false;

private:
    static Logger &log;
    bool verifyVersion();

    uint32_t headerLength_ = 0; //starting after this
    uint32_t playerUnitsCount = 0;
    uint32_t playerCount2_ = 0;
    uint32_t numTriggers_ = 0;
    uint32_t fileCount_ = 0;

    Compressor compressor_;

    void serializeObject() override;

    void serializeVersion();
};

typedef std::shared_ptr<ScnFile> ScnFilePtr;

/// Wrapper for file data in CPX/CPN files
class CpxIncludedFile : public ISerializable
{
public:
    std::string identifier;
    std::string filename;
    uint32_t size = 0;
    uint32_t offset = 0;

    bool probablySwgb = false;

    ScnFilePtr getScnFile();

    bool extractTo(const std::string &outPath);

private:
    void serializeObject() override;
};


/// Misleading name, it supports both CPN and CPX
class CpxFile : public IFile
{
public:
    void serializeObject() override;

    std::vector<std::string> getFilenames() const;
    ScnFilePtr getScnFile(const std::string &filename);
    ScnFilePtr getScnFile(size_t index);
    CpxIncludedFile getRawFile(const std::string &filename);

    uint32_t getFilecount() const { return filecount; }

    std::string version;
    std::string name;

    bool verbose = false;

private:
    uint32_t filecount = 0;
    std::vector<CpxIncludedFile> m_files;
};

} // namespace genie

