#pragma once

#include "genie/file/ISerializable.h"
#include "genie/util/Logger.h"

#include <algorithm>
#include <cmath>
#include <memory>

namespace genie {

/// New crap style for AoE2:DE
#ifdef _MSC_VER
#pragma pack(push,1)
#endif
struct SmpPixel
{
    uint8_t index; /// Normal palette index
    uint8_t section; /// Need to look up in palette.conf to find the correct color table
    uint16_t damageMask; /// When units get damaged

    inline uint8_t paletteIndex() const noexcept { return section >> 2; }
    inline uint8_t paletteSection() const noexcept { return section & 0b11; }

    inline bool damageFlag() const noexcept {
        return damageMask & (1 << 9);
    }

    inline float damageRgbMultiplier(const float damagePercent) const noexcept {
        const float damageWindow99To50 = std::clamp(damagePercent * 2.0f, 0.f, 1.f);
        const float damageWindow74To25 = std::clamp(damageWindow99To50 - 0.5f, 0.f, 1.f);
        const float damageWindow49To0 =  std::clamp(damageWindow74To25 - 0.5f, 0.f, 1.f);

        const float damage_modifier = (damageMask >> 4) & 0x1ff;

        float a = std::floor(damage_modifier / 64.f);
        float temp = damage_modifier - 64.f * a + 0.5f;
        float b = std::floor(temp / 8.f);
        float c = temp - 8.f * b;

        a = a * damageWindow99To50;
        b = b * damageWindow74To25;
        c = c * damageWindow49To0;

        return 1.f - std::clamp((a + b + c) / 7.f, 0.0f, 0.65f);
    }
}
#ifndef _MSC_VER
__attribute__((packed));
#else
;
#pragma pack(pop)
#endif

struct SmpPlayerColorXY
{
    uint32_t index;
    uint32_t x;
    uint32_t y;

};

class SmpFrame : public ISerializable
{
    static Logger &log;

public:
    static SmpFrame null;

    int width() const {
        return width_;
    }
    int height() const {
        return height_;
    }
    int hotspotX() const {
        return hotspot_x;
    }
    int hotspotY() const {
        return hotspot_y;
    }

    const std::vector<uint32_t> &pixels() const {
        return smp_pixels;
    }
    const std::vector<uint8_t> &alphaMask() const {
        return smp_alpha_mask;
    }

    const std::vector<SmpPlayerColorXY> &playerColors() const {
        return smp_player_color_mask;
    }

protected:
    void serializeObject() override;

private:
    void readImage();
    void readSmpPixelstoImage(uint32_t row, uint32_t &col, uint32_t count,
                           bool player_col = false);

    std::vector<uint32_t> smp_pixels;
    std::vector<SmpPlayerColorXY> smp_player_color_mask;
    std::vector<uint8_t> smp_alpha_mask;

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

using SmpFramePtr = std::shared_ptr<SmpFrame>;

}//namespace genie
