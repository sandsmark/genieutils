#pragma once

#include "SlpFrame.h"

#include "genie/file/ISerializable.h"
#include "genie/util/Logger.h"

namespace genie {

/**
 * @brief The SmxFrame class
 * The frame definitions start directly after the file header.
 * Like in the SMP format, the frames come in bundles that consist of up to 3 images:
 *  - main sprite
 *  - shadow for that sprite (optional)
 *  - outline (optional)
 * Which of these images are present in a bundle is determined by the value bundle_type from the bundle header.
 */
class SmxFrame : public ISerializable
{
    static Logger &log;


protected:
    void serializeObject() override;

private:
    struct FrameHeader {
        uint16_t width = 0; /// Width of image
        uint16_t height = 0; /// Height of image

        uint16_t centerX = 0; /// Centre of sprite (X coord)
        uint16_t centerY = 0; /// Centre of sprite (Y coord)

        uint32_t size = 0; /// Length of frame in bytes
        uint32_t unknown = 0;

        struct RowEdge : public ISerializable {
            uint16_t padLeft = 0;
            uint16_t padRight = 0;

            void serializeObject() override {
                serialize(padLeft);
                serialize(padRight);
            }
        };
        /// Directly after the frame header, an array of smp_frame_row_edge (of length height) structs begins. These work exactly like the row edges in the SMP files.
        std::vector<RowEdge> rowEdges;
    };
    void serializeFrameHeader(FrameHeader &header);

    void readNormalGraphics();
    void readShadowGraphics();
    void readOutlineGraphics();

    std::vector<SmpPixel> decode4Plus1(const std::vector<uint8_t> &data);
    std::vector<SmpPixel> decode8To5();

    std::vector<SmpPlayerColorXY> m_playerColorPixels;

    std::vector<SmpPixel> m_pixels;

    enum SmpCommand {
        Skip = 0b00,
        Draw = 0b01,
        PlayerColor = 0b10,
        EndOfRow = 0b11
    };

    /// SMX Bundle header
    struct BundleHeader {
        enum TypeBits : uint8_t {
            Unused1 = 1 << 7,
            Unused2 = 1 << 6,
            Unused3 = 1 << 5,

            /// Unknown, but only used by bridges
            BridgeRelated = 1 << 4,

            /// Determines the compression algorithm for the main graphic frame.
            /// 0 = 4plus1; 1 = 8to5 (see the Compression Algorithms section)
            SkipDamageMask = 1 << 3,

            /// If set to 1, the bundle contains an outline frame
            OutlineFrame = 1 << 2,

            /// If set to 1, the bundle contains a shadow frame
            ShadowFrame = 1 << 1,

            /// If set to 1, the bundle contains a main graphic frame
            NormalFrame = 1 << 0
        };

        /// Bundle Type
        /// bundle_type is a bit field. This means that every bit set to 1 in this values tells us something about the bundle.
        uint8_t type = 0;

        /// Palette number
        uint8_t palette = 0;
        /// possibly uncompressed size
        uint32_t maybeSize = 0;
    } m_bundleHeader;

    FrameHeader m_normalHeader;
    FrameHeader m_shadowHeader;
    FrameHeader m_outlineHeader;

};

} // namespace genie
