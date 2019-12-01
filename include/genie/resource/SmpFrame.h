#pragma once

#include "genie/file/ISerializable.h"
#include "genie/util/Logger.h"

namespace genie {

/// New crap style for AoE2:DE
#ifdef _MSC_VER
#pragma pack(push,1)
#endif
struct SmpPixel
{
    uint8_t index; /// Normal palette index
    uint8_t section; /// Need to look up in palette.conf to find the correct color table
    uint8_t damageMask; /// When units get damaged
    uint8_t damageMask2; /// When units get damaged 2

    inline uint8_t paletteIndex() const noexcept { return section >> 2; }
    inline uint8_t paletteSection() const noexcept { return section & 0b11; }
    inline bool isMasked1() const noexcept { return damageMask & 0x80; }
    inline bool isMasked2() const noexcept { return damageMask & 3; }
}
#ifndef _MSC_VER
__attribute__((packed));
#else
;
#pragma pack(pop)
#endif

/// TODO: unify smp and non-smp
struct SmpPlayerColorXY {
    uint32_t x;
    uint32_t y;
    SmpPixel pixel;
};


class SmpFrame : public ISerializable
{
    static Logger &log;

public:
    static SmpFrame null;

protected:
    void serializeObject() override;

private:
    void readImage();
    void readSmpPixelstoImage(uint32_t row, uint32_t &col, uint32_t count,
                           bool player_col = false);

    std::vector<SmpPixel> smp_pixels;
    std::vector<SmpPlayerColorXY> smp_player_color_mask;

    uint32_t width_;
    uint32_t height_;

    int32_t hotspot_x = 0;
    int32_t hotspot_y = 0;

    uint32_t m_frameType = 0;

    std::vector<uint16_t> left_edges_;
    std::vector<uint16_t> right_edges_;

    uint32_t properties_;
    uint32_t cmd_table_offset_;
    uint32_t outline_table_offset_;
    std::streampos slp_file_pos_;
};

}//namespace genie
