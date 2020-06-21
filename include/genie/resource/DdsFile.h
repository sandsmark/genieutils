#pragma once

#include "genie/file/IFile.h"

namespace genie{

class DdsFile : public IFile
{
    static constexpr uint32_t ddsFourCC = 0x20534444;  // 'DDS '
    static constexpr uint32_t dxt1FourCC = 0x31545844; // 'DXT1'

public:
    #pragma pack(push,1)
    struct Header {
        enum Flags : uint32_t{
            HasCaps = 0x00000001,
            HasHeight = 0x00000002,
            HasWidth = 0x00000004,
            HasPitch = 0x00000008,
            HasPixelformat = 0x00001000,
            HasMipmapCount = 0x00020000,
            HasLinearSize = 0x00080000,
            HasDepth = 0x00800000
        };
        static constexpr int necessaryFlags = HasHeight | HasWidth | HasPixelformat | HasMipmapCount;

        uint32_t size;
        uint32_t flags;
        uint32_t height;
        uint32_t width;
        uint32_t pitchOrLinearSize;
        uint32_t depth;
        uint32_t mipMapCount;
        uint8_t reserved1[44];

        struct PixelFormat {
            enum Flags : uint32_t {
                HasAlphaPixels = 0x1,
                HasUncompressedAlpha = 0x2,
                HasFourCC = 0x4,
                HasUncompressedRgb = 0x40,
                HasUncompressedYuv = 0x200,
                HasUncompressedSingleChannel = 0x20000
            };

            uint32_t size;
            uint32_t flags;
            uint32_t fourCC;
            uint32_t rgbBitCount;
            uint32_t rBitMask;
            uint32_t gBitMask;
            uint32_t bBitMask;
            uint32_t aBitMask;
        } pixelFormat;

        uint32_t caps;
        uint32_t caps2;
        uint32_t caps3;
        uint32_t caps4;
        uint32_t reserved2;
    } header;
    #pragma pack(pop)

protected:
    void serializeObject() override;
    std::vector<uint32_t> decodeDxt1(const std::vector<uint8_t> &compressed);

    std::vector<std::vector<uint8_t>> m_data;
    uint32_t m_magic = ddsFourCC; // 'DDS '
};

}
