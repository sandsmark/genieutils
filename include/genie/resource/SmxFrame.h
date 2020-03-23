#pragma once

#include "SmpFrame.h"

#include "genie/file/ISerializable.h"
#include "genie/util/Logger.h"

#include <memory>

namespace genie {

/// TODO: unify smp and non-smp
struct SmxPlayerColorXY {
    uint32_t x;
    uint32_t y;
    SmpPixel pixel;
};

/**
 * @brief The SmxFrame class
 * The frame definitions start directly after the file header.
 * Like in the SMP format, the frames have up to 3 images:
 *  - main layer
 *  - shadow layer (optional)
 *  - outline layer (optional)
 * Which of these layers are present in a frame is determined by the value frame_type from the frame header.
 */
class SmxFrame : public ISerializable
{
    static Logger &log;

public:
    static SmxFrame null;

    inline int width() const noexcept { return m_normalHeader.width; }
    inline int height() const noexcept { return m_normalHeader.height; }

    inline const SmpPixel &pixel(const uint32_t x, const uint32_t y) const {
        const size_t pixelIndex = x + y * m_normalHeader.width;
        assert(pixelIndex < m_pixels.size());
        return m_pixels[pixelIndex];
    }
    inline bool isVisible(const uint32_t x, const uint32_t y) const {
        const size_t pixelIndex = x + y * m_normalHeader.width;
        assert(pixelIndex < m_pixels.size());
        return m_mask[pixelIndex];
    }

    inline int paletteIndex(const uint32_t x, const uint32_t y) const {
        const size_t pixelIndex = x + y * m_normalHeader.width;
        assert(pixelIndex < m_pixels.size());
        assert(m_pixels[pixelIndex].palette <= 4);
        return m_pixels[pixelIndex].palette * 256 + m_pixels[pixelIndex].index;
    }

    bool isLoaded() const {
        return m_isLoaded;
    }

    bool load();

protected:
    void serializeObject() override;

private:
    struct LayerHeader {
        uint16_t width = 0; /// Width of image
        uint16_t height = 0; /// Height of image

        int16_t centerX = 0; /// Centre of sprite (X coord)
        int16_t centerY = 0; /// Centre of sprite (Y coord)

        uint32_t size = 0; /// Length of frame in bytes
        uint32_t unknown = 0;

        struct RowEdge : public ISerializable {
            uint16_t padLeft = 0;
            uint16_t padRight = 0;

            void serializeObject() override {
                serialize<uint16_t>(padLeft);
                serialize<uint16_t>(padRight);
            }
        };
        /// Directly after the layer header, an array of smp_layer_row_edge (of length height) structs begins. These work exactly like the row edges in the SMP files.
        std::vector<RowEdge> rowEdges;

        std::streampos filePosition;
        uint32_t commandsSize = 0;
        uint32_t pixelDataSize = 0; // only normal graphics
    };
    void serializeLayerHeader(LayerHeader &header);

    void readNormalLayer();
    void readShadowLayer();
    void readOutlineLayer();

    std::vector<SmpPixel> decode4Plus1(const std::vector<uint8_t> &data);
    std::vector<SmpPixel> decode8To5(const std::vector<uint8_t> &data);

    std::vector<SmxPlayerColorXY> m_playerColorPixels;

    std::vector<SmpPixel> m_pixels;
    std::vector<uint8_t> m_mask;

    enum SmpCommand {
        Skip = 0b00,
        Draw = 0b01,
        PlayerColor = 0b10,
        EndOfRow = 0b11
    };

    /// SMX Frame header
    struct FrameHeader {
        enum TypeBits : uint8_t {
            Unused1 = 1 << 7,
            Unused2 = 1 << 6,
            Unused3 = 1 << 5,

            /// Unknown, but only used by bridges
            BridgeRelated = 1 << 4,

            /// Determines the compression algorithm for the main graphic frame.
            /// 0 = 4plus1; 1 = 8to5 (see the Compression Algorithms section)
            HasDamageModifier = 1 << 3,

            /// If set to 1, the bundle contains an outline frame
            OutlineLayer = 1 << 2,

            /// If set to 1, the bundle contains a shadow frame
            ShadowLayer = 1 << 1,

            /// If set to 1, the bundle contains a main graphic frame
            NormalLayer = 1 << 0
        };

        /// Frame Type
        /// frame_type is a bit field. This means that every bit set to 1 in this values tells us something about the bundle.
        uint8_t frameType = 0;

        /// Palette number
        uint8_t palette = 0;
        /// possibly uncompressed size
        uint32_t size = 0;
    } m_frameHeader;

    LayerHeader m_normalHeader;
    LayerHeader m_shadowHeader;
    LayerHeader m_outlineHeader;

    bool m_isLoaded = false;
};

typedef std::shared_ptr<SmxFrame> SmxFramePtr;

} // namespace genie
