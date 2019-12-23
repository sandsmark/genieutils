/*
    Mostly used for sloped terrains
    Copyright (C) 2018 - 2019 Martin Sandsmark

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

    Most of the documentation is written by aap, and hence the copyright is owned by him
*/

#pragma once

#include <istream>
#include <vector>
#include <cassert>

#include "genie/file/IFile.h"
#include "genie/util/Logger.h"

#include "Slope.h"
#include "MaskPattern.h"

namespace genie {

class SlpFile;
typedef std::shared_ptr<SlpFile> SlpFilePtr;
class SlpFrame;
typedef std::shared_ptr<SlpFrame> SlpFramePtr;

/// This is used together with the high quality lighting defined in PatternMasks.dat.
/// Its purpose is to increase the number of brightness steps from 9 (number of
/// ICMs) to 17 (the last one is neutral, ICM 4) by dithering them.
class LightmapFile : public IFile
{
public:
    /// Inverse color map to use given a light index (0-18) and brightness
    uint8_t lightmaps[18][4096];

    /// Is the file loaded
    operator bool() const noexcept
    {
        return m_loaded;
    }

private:
    void serializeObject() noexcept override;

    bool m_loaded = true;
};

/// view_icm.dat defines a number of inverse color maps to convert\n
/// RGB colors to palette indices.\n
/// These are normally used for lighting sloped\n
/// terrain tiles and finding palette indices after blending\n
/// RGB colors (for terrain blending e.g.).
class IcmFile : public IFile
{
public:
    /// Used to look up in @ref maps
    enum ColorMapType {
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

    /// Defines a single map from RGB to palette index
    struct InverseColorMap {
        /// The palette indices for each possible value of red, green and blue
        uint8_t map[32][32][32];

        /// Look up a palette index from a "normal" RGB value.\n
        /// So what you probably want to do (if you have normal RGB values) is to shift 3 bits right, e. g.:\n
        ///   const uint8_t index = icm.paletteIndex(r >> 3, g >> 3, b >> 3);
        inline constexpr uint8_t paletteIndex(const uint8_t r, const uint8_t g, const uint8_t b) const noexcept
        {
            return map[r][g][b];
        }
    };

    /// AoK defines 9 ICMs (0-8); 4 being neutral, 0-3 being darkened,
    /// 5-8 being brightened. TC has one more at the end which seems to
    /// be the neutral one as well.
    /// See @ref ColorMapType for which to look up
    std::vector<InverseColorMap> maps;

    /// Is the file loaded
    operator bool() const noexcept
    {
        return m_loaded;
    }

private:
    void serializeObject() noexcept override;

    bool m_loaded = true;
};
/// PatternMasks.dat contains lighting textures used to light sloped
/// terrain tiles. It is used in conjunction with Lightmaps.dat
class PatternMasksFile : public IFile
{
public:
    /// Lightmaps.dat, used for dithering for high quality lighting
    LightmapFile lightmapFile;

    /// view_icm.dat, tables for looking up a palette index given an RGB color
    IcmFile icmFile;

    /// Every non-flat tile is lit by multiple patternmasks that depend on slope
    /// type and neighboring slopes types. One patternmask serves as the base with
    /// which the others are combined.
    /// Every pixel in the 64x64 textures is a brightness value (lightmap index) plus
    /// two combination flag bits. For the base pattern the combination flags are ignored.
    /// For all following patterns if bit 0 of a pixel is set, it is ignored. If bit 1
    /// is set, the pixel replaces the current one if it is brighter. If bit 1 is clear,
    /// the pixel replaces the current one if it is darker. Bits 2-7 are the brightness
    /// value. However do note that the uppermost bit is masked off in the end, it is
    /// used with bit 1 to effectively replace any value (it is brighter than any
    /// pixel in the 0-17 range).
    struct PatternMask {

        /// Should the source pixel be used as is?
        inline static constexpr bool isIgnore(const uint8_t pixel) noexcept
        {
            return pixel & 0x1;
        }

        /// Should the source pixel be brightened?
        inline static constexpr bool isBrighten(const uint8_t pixel) noexcept
        {
            return pixel & 0x2;
        }

        /// Should the source pixel be darkened?
        inline static constexpr bool isDarken(const uint8_t pixel) noexcept
        {
            return (pixel & 0x2) == 0;
        }

        /// Given a source pixel and pattern index returns a pixel with applied lighting
        inline constexpr uint8_t apply(const uint8_t input, const int index) const noexcept
        {
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
    private:
        friend class PatternMasksFile;
        uint8_t pixels[4096];
    };

    /// Returns the correct ICM given a light index, and the slope @ref Pattern of the
    /// current tile as well as the neighbouring tiles
    const IcmFile::InverseColorMap &getIcm(const uint16_t lightIndex, const std::vector<Pattern> &patterns) const noexcept
    {
        // .empty() is very slow with libstdc++; it compares iterators,
        // .size() just compares memory addresses, so this is faster.
        // And this is a very hot path, so yes it matters
        if (patterns.size() == 0) { // NOLINT
            return icmFile.maps[IcmFile::Neutral];
        }

        uint8_t lightmapIndex = m_masks[patterns[0]].pixels[lightIndex] >> 2 & 0x1F;

        for (size_t i = 1; i < patterns.size(); i++) {
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

    /// If the file is loaded
    operator bool() noexcept
    {
        return m_loaded;
    }

private:
    void serializeObject() noexcept override;

    bool m_loaded = false;
    PatternMask m_masks[PatternMasksCount];
};

/// FilterMaps.dat is used to texture map and light flat terrain tiles onto sloped
/// tiles using the SLP templates defined in STemplet.dat. It is used for high
/// and medium quality terrain rendering.
class FiltermapFile : public IFile
{
public:

    /// The pixel to use as base
    struct SourcePixel {
        /// Alpha to use for the pixel
        uint16_t alpha;

        /// Index of the original pixel
        uint32_t sourceIndex;
    };

    /// The commands for blending in the source pixel
    struct FilterCmd {
        /// Number of source pixels
        uint16_t sourcePixelCount;

        /// The initial light index
        uint16_t lightIndex;

        /// The source pixels themselves
        std::vector<SourcePixel> sourcePixels;
    };

    /// A single line of filter commands
    struct FilterLine {
        /// Width of scanline
        uint8_t width;

        /// Command for each pixel in line
        std::vector<FilterCmd> commands;
    };

    /// Defines a filter map for an entire fram
    struct Filtermap {
        /// Number of lines
        uint32_t height;

        /// Lines of filtering commands
        std::vector<FilterLine> lines;
    };

    /// Filtermap for a given slope
    std::array<Filtermap, SlopeCount> maps;

    /// Is the file loaded?
    operator bool() noexcept
    {
        return m_loaded;
    }

private:
    void serializeObject() noexcept override;
    bool m_loaded = false;
};

/// STemplet.dat contains SLP templates used as a base for sloped terrain tiles.
/// SLP templates are used to create single frame SLPs. They do not store actual
/// drawing commands but provide the general structure of an SLP.
class SlpTemplateFile : public IFile
{
public:
    /// Basically a patch for an SLP frame, for sloped terrain
    /// Overwrites the header (size, hotspot and command tables)
    struct SlpTemplate {
        /// New width for SLP frame
        uint32_t width_;

        /// New height for SLP frame
        uint32_t height_;

        /// New hotspot X for SLP Frame
        int32_t hotspot_x;

        /// New hotspot Y for SLP frame
        int32_t hotspot_y;

        /// Size of the data in this SLP template
        int32_t data_size;

        /// "Indentation" of line"
        std::vector<unsigned short> left_edges_;

        /// New amount of blank space on the right
        std::vector<unsigned short> right_edges_;

        /// Offsets into the original SLP per line
        std::vector<uint32_t> cmd_offsets_;

        /// New offset into the original SLP for outline
        uint32_t outline_table_offset_;

        /// Offset into the original SLP for command table
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

    /// Template to use for a given slope
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
