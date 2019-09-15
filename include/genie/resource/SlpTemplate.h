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
#include <cassert>

#include "genie/file/IFile.h"
#include "genie/util/Logger.h"
#include "PalFile.h"
#include "Slope.h"

namespace genie {

class SlpFile;
typedef std::shared_ptr<SlpFile> SlpFilePtr;
class SlpFrame;
typedef std::shared_ptr<SlpFrame> SlpFramePtr;

class LightmapFile : public IFile
{
public:
    uint8_t lightmaps[18][4096];

    operator bool() const noexcept {
        return m_loaded;
    }

private:
    void serializeObject() noexcept override;

    bool m_loaded = true;
};

class IcmFile : public IFile
{
public:
    enum {
        Darkest = 0,
        Darker = 1,
        Dark = 2,
        Darkened = 3,
        Neutral = 4,
        Brightened = 5,
        Bright = 6,
        Brighter = 7,
        Brightest = 8,
        AokNeutral = 9
    };

    struct InverseColorMap
    {
        uint8_t map[32][32][32];

        inline constexpr uint8_t paletteIndex(const uint8_t r, const uint8_t g, const uint8_t b) const noexcept {
            return map[r][g][b];
        }
    };
    std::vector<InverseColorMap> maps;

    operator bool() const noexcept {
        return m_loaded;
    }

private:
    void serializeObject() noexcept override;

    bool m_loaded = true;
};

// TODO: find better names for the rest of the values
// https://github.com/aap/geniedoc/blob/master/patternmasks.bmp
enum Pattern {
    FlatPattern = 0,
    BlackPattern = 1,
    DiagDownPattern = 2,
    DiagUpPattern = 3,
    HalfDownPattern = 4,
    HalfUpPattern = 5,
    HalfLeftPattern = 6,
    HalfRightPattern = 7,
    DownPattern = 8,
    UpPattern = 9,
    LeftPattern = 10,
    RightPattern = 11,
    Pattern12 = 12,
    Pattern13 = 13,
    Pattern14 = 14,
    Pattern15 = 15,
    Pattern16 = 16,
    Pattern17 = 17,
    Pattern18 = 18,
    Pattern19 = 19,
    Pattern20 = 20,
    Pattern21 = 21,
    Pattern22 = 22,
    Pattern23 = 24,
    Pattern24 = 24,
    Pattern25 = 25,
    Pattern26 = 26,
    Pattern27 = 27,
    Pattern28 = 28,
    Pattern29 = 29,
    Pattern30 = 30,
    Pattern31 = 31,
    Pattern32 = 32,
    Pattern33 = 33,
    Pattern34 = 34,
    Pattern35 = 35,
    Pattern36 = 36,
    Pattern37 = 37,
    Pattern38 = 38,
    Pattern39 = 39,
    PatternMasksCount = 40,
};

class PatternMasksFile : public IFile
{
public:
    LightmapFile lightmapFile;
    IcmFile icmFile;

    struct PatternMask {
        uint8_t pixels[4096];

        inline static constexpr bool isIgnore(const uint8_t pixel) noexcept {
            return pixel & 0x1;
        }
        inline static constexpr bool isBrighten(const uint8_t pixel) noexcept {
            return pixel & 0x2;
        }
        inline static constexpr bool isDarken(const uint8_t pixel) noexcept {
            return (pixel & 0x2) == 0;
        }

        inline constexpr uint8_t apply(const uint8_t input, const int index) const noexcept {
            const uint8_t pixel = pixels[index];
            if (isIgnore(pixel)) {
                return input;
            }

            const uint8_t icm = (pixel >> 2) & 0x1f;

            if (isBrighten(pixel)) {
                return std::max(icm, input);
            } else {
                return std::min(icm, input);
            }
        }
    };

    const IcmFile::InverseColorMap &getIcm(const uint16_t lightIndex, const std::vector<Pattern> &patterns) const noexcept
    {
        // .empty() is very slow with libstdc++; it compares iterators,
        // .size() just compares memory addresses, so this is faster.
        // And this is a very hot path, so yes it matters
        if (patterns.size() == 0) {
            return icmFile.maps[IcmFile::Neutral];
        }

        uint8_t lightmapIndex = m_masks[patterns[0]].pixels[lightIndex] >> 2 & 0x1F;

        for (size_t i=1; i < patterns.size(); i++) {
            const uint8_t pixel = m_masks[patterns[i]].pixels[lightIndex];
            if (PatternMask::isIgnore(pixel)) {
                continue;
            }

            const uint8_t newLightmapIndex = (pixel >> 2) & 0x1F;

            if (PatternMask::isBrighten(pixel)) {
                lightmapIndex = std::max(newLightmapIndex, lightmapIndex);
            } else {
                lightmapIndex = std::min(newLightmapIndex, lightmapIndex);
            }
        }

        const size_t icmIndex = lightmapFile.lightmaps[lightmapIndex][lightIndex];

        if (icmIndex >= icmFile.maps.size()) {
            return icmFile.maps[IcmFile::Neutral];
        }

        return icmFile.maps[icmIndex];
    }

    operator bool() noexcept {
        return m_loaded;
    }

    PatternMask m_masks[PatternMasksCount];

private:
    void serializeObject() noexcept override;

    bool m_loaded = false;
};

class FiltermapFile : public IFile
{
public:
    struct SourcePixel {
        uint16_t alpha;
        uint32_t sourceIndex;
    };

    struct FilterCmd {
        uint16_t sourcePixelCount;
        uint16_t lightIndex;
        std::vector<SourcePixel> sourcePixels;
    };

    struct FilterLine {
        uint8_t width;
        std::vector<FilterCmd> commands;
    };

    struct Filtermap {
        uint32_t height;
        std::vector<FilterLine> lines;
    };
    std::array<Filtermap, SlopeCount> maps;

    operator bool() noexcept {
        return m_loaded;
    }

private:
    void serializeObject() noexcept override;
    bool m_loaded = false;
};

//------------------------------------------------------------------------------
/// Basically a patch for an SLP frame, for sloped terrain
/// Overwrites the header (size, hotspot and command tables)
//
class SlpTemplateFile : public IFile
{
public:
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

    //----------------------------------------------------------------------------
    /// Frees all content of a slp file.
    //
    void unload() noexcept override;

    //----------------------------------------------------------------------------
    /// Check whether the files content is loaded or not.
    //
    bool isLoaded() const noexcept;

    std::array<SlpTemplate, SlopeCount> templates;

private:
    static Logger &log;

    bool loaded_ = false;

    //----------------------------------------------------------------------------
    void serializeObject() override;

    //----------------------------------------------------------------------------
    /// Loads the file and its frames.
    //
    void loadFile() noexcept;
    void saveFile() noexcept;

};

typedef std::shared_ptr<SlpTemplateFile> SlpTemplateFilePtr;
} // namespace genie
