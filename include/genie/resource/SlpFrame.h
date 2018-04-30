/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Armin Preiml
    Copyright (C) 2015 - 2017  Mikko "Tapsa" P

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

#ifndef GENIE_SLPFRAME_H
#define GENIE_SLPFRAME_H

#include "genie/file/ISerializable.h"
#include "genie/resource/SlpTemplate.h"
#include "genie/util/Logger.h"

#include <istream>
#include <vector>
#include <set>
#include <stdint.h>

#include "PalFile.h"

namespace genie {

class SlpTemplate;
class FiltermapFile;

struct XY
{
    uint32_t x;
    uint32_t y;
};

inline bool operator<(const XY &l, const XY &r)
{
    return l.y == r.y ? l.x < r.x : l.y < r.y;
}

// Element for player_color vector, the vector stores position (x, y) of
// a player color pixel and the palette index for the color
struct PlayerColorXY
{
    uint32_t x;
    uint32_t y;
    uint8_t index;
};

inline bool operator<(const PlayerColorXY &l, const PlayerColorXY &r)
{
    return l.y == r.y ? l.x < r.x : l.y < r.y;
}

struct SlpFrameData
{
    std::vector<uint8_t> pixel_indexes;
    std::vector<uint32_t> bgra_channels;
    std::vector<uint8_t> alpha_channel;

    std::vector<XY> shadow_mask;
    std::vector<XY> shield_mask;
    std::vector<XY> outline_pc_mask;
    std::vector<XY> transparency_mask;

    std::vector<PlayerColorXY> player_color_mask;
    std::vector<genie::Color> palette;
};

//------------------------------------------------------------------------------
/// Class for reading a frame of a slp file. Once loaded the image can be
/// obtained as a pixel array. A pixel is stored as the index of a color
/// in a palette.
///
/// TODO: Check file corruption
/// TODO: One (or more??) mask for player color and outline (and shadow?)
//
class SlpFrame;
typedef std::shared_ptr<SlpFrame> SlpFramePtr;
class SlpFrame : protected ISerializable
{

public:
    //----------------------------------------------------------------------------
    /// Constructor
    ///
    SlpFrame();

    //----------------------------------------------------------------------------
    /// Destructor
    //
    virtual ~SlpFrame();

    //----------------------------------------------------------------------------
    /// Set slp file position inside the stream. It's necesarry for calculating
    /// file offsets.
    //
    void setSlpFilePos(std::streampos pos);

    //----------------------------------------------------------------------------
    /// Loads header data. The headers of frames are stored after the header of
    /// the slp file.
    //
    void serializeHeader(void);
    void setLoadParams(std::istream &istr);
    void setSaveParams(std::ostream &ostr, uint32_t &slp_offset_);

    //----------------------------------------------------------------------------
    /// Loads frame data and creates an image. Frame data is located after all
    /// frame headers of the slp file.
    //
    void load(std::istream &istr);
    void save(std::ostream &ostr);

    //----------------------------------------------------------------------------
    /// Get images width.
    //
    uint32_t getWidth(void) const;

    //----------------------------------------------------------------------------
    /// Get images height.
    //
    uint32_t getHeight(void) const;

    void setSize(const uint32_t width, const uint32_t height);
    void enlarge(const uint32_t width, const uint32_t height, const int32_t offset_x, const int32_t offset_y);
    void enlargeForMerge(const SlpFrame &frame, int32_t &os_x, int32_t &os_y);

    //----------------------------------------------------------------------------
    /// Returns a pixel array containing the indexes of a color in a palette.
    ///
    /// @return pixel array size of (width * height)
    //
    uint32_t getProperties(void) const;
    uint32_t getPaletteOffset(void) const;
    bool is32bit(void) const;

    //----------------------------------------------------------------------------
    /// Returns the outline created by loading the frame.
    /// Outline is the border of the sprite shown if a object is behind another
    /// one.
    ///
    /// @return loaded outline
    //
    // TODO

    //----------------------------------------------------------------------------
    /// Returns the mask colored with color of given players index.
    ///
    /// @param player index
    /// @return player color mask
    //
    // getPlayerColorMask(uint8_t player) const; //TODO

    //----------------------------------------------------------------------------
    /// Get the hotspot of the frame. The Hotspot is the isometric center of
    /// the object presented by this frame.

    int32_t hotspot_x;
    int32_t hotspot_y;
    SlpFrameData img_data;

    std::shared_ptr<SlpFrame> mirrorX(void);

    void readImage();

    SlpFramePtr filtered(const FiltermapFile &filterFile, uint8_t filterNum, const std::vector<Pattern> patterns, const std::vector<Color> &palette);

private:
    friend class SlpTemplateFile;

    enum Commands : uint8_t {
        EndOfRow = 0xF,
        GreaterBlockCopy = 0x2,
        GreaterSkip = 0x3,
        CopyAndTransform = 0x6,
        FillColor = 0x7,
        TransformBlock = 0xA,
        Shadow = 0xB,
        ExtendedCommand = 0xE
    };
    enum ExtendedCommands : uint8_t {
        ForwardDraw = 0x0E,
        ReverseDraw = 0x1E,
        NormalTransform = 0x2E,
        AlternativeTransform = 0x3E,
        OutlinePlayerColor = 0x4E,
        OutlineShieldColor = 0x6E,
        OutlinePlayerColorSpan = 0x5E,
        OutlineShieldColorSpan = 0x7E,
        Dither = 0x8E,
        PremultipliedAlpha = 0x9E,
        OriginalAlpha = 0xAE,

    };

    static Logger &log;

    std::streampos slp_file_pos_;

    uint32_t cmd_table_offset_;
    uint32_t outline_table_offset_;
    uint32_t palette_offset_;
    uint32_t properties_;

    uint32_t width_;
    uint32_t height_;

    std::vector<uint16_t> left_edges_;
    std::vector<uint16_t> right_edges_;

    std::vector<uint32_t> cmd_offsets_;
    std::vector<std::vector<uint8_t>> commands_; // Don't you dare use std::list here!

    virtual void serializeObject(void);

    //----------------------------------------------------------------------------
    /// Reads pixel indexes from file and sets the pixels according to the
    /// colors from the palette.
    /// It is assumed that the stream pointer is at the start of the pixel array.
    ///
    /// @param row row to set pixels at
    /// @param col column to set pixels from
    /// @param count how many pixels should be read
    /// @param player_col if true, pixel will be written to player color image
    //
    void readPixelsToImage(uint32_t row, uint32_t &col, uint32_t count,
                           bool player_col = false);
    void readPixelsToImage32(uint32_t row, uint32_t &col, uint32_t count,
                             uint8_t special = 0);

    //----------------------------------------------------------------------------
    /// Sets the next count of pixels to given color without reading from stream.
    ///
    /// @param row row to set pixels at
    /// @param col column to set pixels from
    /// @param count how many pixels should be set
    /// @param color color to set
    /// @param player_col if true, pixel will be written to player color image
    //
    void setPixelsToColor(uint32_t row, uint32_t &col, uint32_t count,
                          bool player_col = false);
    void setPixelsToColor32(uint32_t row, uint32_t &col, uint32_t count,
                            bool player_col = false);

    //----------------------------------------------------------------------------
    /// Sets the next count of pixels to shadow without reading from stream.
    void setPixelsToShadow(uint32_t row, uint32_t &col, uint32_t count);
    void setPixelsToShield(uint32_t row, uint32_t &col, uint32_t count);
    void setPixelsToPcOutline(uint32_t row, uint32_t &col, uint32_t count);

    //----------------------------------------------------------------------------
    /// This method returns either the count stored in command byte or (if not
    /// stored in command) the value of the next byte.
    ///
    /// @param data command byte
    //
    uint8_t getPixelCountFromData(uint8_t data);

    enum cnt_type { CNT_LEFT,
                    CNT_SAME,
                    CNT_DIFF,
                    CNT_TRANSPARENT,
                    CNT_FEATHERING,
                    CNT_PLAYER,
                    CNT_SHIELD,
                    CNT_PC_OUTLINE,
                    CNT_SHADOW };
    void handleColors(cnt_type count_type, uint32_t row, uint32_t col, uint32_t count);
    void handleSpecial(uint8_t cmd, uint32_t row, uint32_t col, uint32_t count, uint32_t pixs);
    void pushPixelsToBuffer(uint32_t row, uint32_t col, uint32_t count);
};

}

#endif // GENIE_SLPFRAME_H
