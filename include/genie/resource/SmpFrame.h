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
    uint8_t palette; /// Need to look up in palette.conf to find the correct color table
    uint16_t damageMask; /// When units get damaged

    inline uint8_t paletteNumber() const noexcept { return palette >> 2; }
    inline uint16_t colorNumber() const noexcept { return (palette & 0b11) * 256 + index; }

    inline bool damageFlag() const noexcept {
        return damageMask & (1 << 9);
    }

    inline float damageRgbMultiplier(const float damagePercent) const noexcept {
        const float damageWindow99To50 = std::clamp(damagePercent * 2.0f, 0.f, 1.f);
        const float damageWindow74To25 = std::clamp(damageWindow99To50 - 0.5f, 0.f, 1.f);
        const float damageWindow49To0 =  std::clamp(damageWindow74To25 - 0.5f, 0.f, 1.f);

        const float damage_modifier = (damageMask >> 4) & 0x1ff;

        float a = std::floor(damage_modifier / 64.f);
        const float temp = damage_modifier - 64.f * a + 0.5f;
        float b = std::floor(temp / 8.f);
        float c = temp - 8.f * b;

        a = a * damageWindow99To50;
        b = b * damageWindow74To25;
        c = c * damageWindow49To0;

        return 1.f - std::clamp((a + b + c) / 7.f, 0.0f, 0.65f);
    }

    operator uint32_t() const {
        // I'm lazy, sue me
        return *reinterpret_cast<const uint32_t*>(this);
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

struct SmpLayerHeader {
    enum  Type {
        Base,
        Shadow,
        Outline
    };

    uint32_t width;
    uint32_t height;
    uint32_t hotspotX;
    uint32_t hotspotY;
    uint32_t type;
    uint32_t paddingTableOffset;
    uint32_t pixelDataOffset;
    uint32_t flags;
};

class SmpFrame;
struct SmpLayer
{
    SmpLayerHeader header;

    std::vector<uint16_t> leftEdges;
    std::vector<uint16_t> rightEdges;

    std::vector<uint32_t> rowOffsets;

private:
    friend class SmpFrame;
    bool isLoaded = false;
};

struct SmpBaseLayer : public SmpLayer
{
    inline bool isVisible(const uint32_t x, const uint32_t y) const {
        const size_t pixelIndex = x + y * header.width;
        assert(pixelIndex < alphaMask.size());
        return alphaMask[pixelIndex] > 0;
    }
    inline int colorNumber(const uint32_t x, const uint32_t y) const {
        const size_t pixelIndex = x + y * header.width;
        return pixels[pixelIndex].colorNumber();
    }
    inline int paletteNumber(const uint32_t x, const uint32_t y) const {
        const size_t pixelIndex = x + y * header.width;
        return pixels[pixelIndex].paletteNumber();
    }

    std::vector<SmpPixel> pixels;
    std::vector<SmpPlayerColorXY> playerColors;
    std::vector<uint8_t> alphaMask;
};

struct SmpSimpleLayer : public SmpLayer
{
    std::vector<uint8_t> colors;
};


class SmpFrame : public ISerializable
{
    static Logger &log;

public:
    static SmpFrame null;

    bool hasBaseLayer() const { return m_baseLayer != nullptr; }
    bool hasShadowLayer() const { return m_shadowLayer != nullptr; }
    bool hasOutlineLayer() const { return m_outlineLayer != nullptr; }

    const SmpBaseLayer &baseLayer();
    const SmpSimpleLayer &shadowLayer();
    const SmpSimpleLayer &outlineLayer();

protected:
    void serializeObject() override;

private:
    enum SmpCommand : uint8_t {
        SkipPixels = 0b00,
        DrawPixels = 0b01,
        DrawPlayerColor = 0b10,
        EndOfRow = 0b11
    };

    void loadLayer(SmpLayer &layer);
    void loadLayerContent(SmpBaseLayer &layer);
    void loadLayerContent(SmpSimpleLayer &layer);

    uint32_t m_layerCount = 0;
    std::vector<uint8_t> m_outlineValues;

    std::unique_ptr<SmpBaseLayer> m_baseLayer;
    std::unique_ptr<SmpSimpleLayer> m_shadowLayer;
    std::unique_ptr<SmpSimpleLayer> m_outlineLayer;
};

using SmpFramePtr = std::shared_ptr<SmpFrame>;

}//namespace genie
