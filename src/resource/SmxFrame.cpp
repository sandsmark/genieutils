#include "genie/resource/SmxFrame.h"

#include <limits.h>
#include <cmath>

namespace genie {

Logger &SmxFrame::log = Logger::getLogger("genie.SmxFrame");

void SmxFrame::serializeObject()
{
//    log.debug(" ======= starting bundle at %", getIStream()->tellg());

    serialize(m_bundleHeader.type);
    if (m_bundleHeader.type == 0) {
        std::cerr << "invalid bundle" << std::endl;
        return;
    }
    serialize(m_bundleHeader.palette);
    serialize(m_bundleHeader.maybeSize);

    if (m_bundleHeader.type & BundleHeader::SkipDamageMask) {
        log.debug("No damage masks");
    }

    //log.debug("Palette: %", m_bundleHeader.palette);
    //log.debug("Maybe size: %", m_bundleHeader.maybeSize);
    //log.debug("Type: %", m_bundleHeader.type);

    if (getOperation() != OP_READ) {
        log.error("Operation % not supported yet", getOperation());
        return;
    }

    if (m_bundleHeader.type & BundleHeader::NormalFrame) {
        //log.debug("Contains normal frame");
        readNormalGraphics();
    }

    if (m_bundleHeader.type & BundleHeader::ShadowFrame) {
//        log.debug("Contains shadow frame");
        readShadowGraphics();
    }

    if (m_bundleHeader.type & BundleHeader::OutlineFrame) {
//        log.debug("Contains outline frame");
        readOutlineGraphics();
    }

}

void SmxFrame::serializeFrameHeader(SmxFrame::FrameHeader &header)
{
    serialize(header.width);
    serialize(header.height);

    serialize(header.centerX);
    serialize(header.centerY);

    serialize(header.size);
    serialize(header.unknown);

    serialize(header.rowEdges, header.height);
    log.debug("Size: %x%; Hotspot %,%; Size %; Unknown %", header.width, header.height, header.centerX, header.centerY, header.size, header.unknown);
}

static inline uint16_t rotateRight2(const uint16_t n)
{
    return (n >> 2) | (n << (16 - 2));
}

void SmxFrame::readNormalGraphics()
{
    serializeFrameHeader(m_normalHeader);

    uint32_t commandsSize = 0;
    serialize(commandsSize);

    uint32_t pixelDataSize = 0;
    serialize(pixelDataSize);

    std::vector<uint8_t> commands;
    serialize(commands, commandsSize);
    uint32_t x=0, y=0;
    x = m_normalHeader.rowEdges[0].padLeft;

//    std::vector<SmpPixel> unusedPixels;

    std::vector<uint8_t> pixelData;
    serialize(pixelData, pixelDataSize);
    if ((m_bundleHeader.type & BundleHeader::SkipDamageMask)) {
        std::cerr << "fixme" << std::endl;
        exit(1);
    }

    static_assert(sizeof(SmxPixel) == sizeof(SmpPixel));

    const std::vector<SmxPixel> pixelsVector = decode4Plus1(std::move(pixelData));
    const SmxPixel *pixels = pixelsVector.data();
    size_t pixelPos = 0;

    m_pixels.resize(m_normalHeader.width * m_normalHeader.height);
    SmpPixel *target = m_pixels.data();
    for (const uint8_t byte : commands) {
        const uint8_t amount = (byte >> 2) + 1;
        const uint8_t command = byte & 0b11;

        const size_t offset = y * m_normalHeader.width;

        switch(command) {
        case Skip:
            x += amount;
            break;
        case PlayerColor: {
            pixelPos += amount;
            x += amount;
            break;
        }
        case Draw: {
            memcpy(&target[offset + x], &pixels[pixelPos], amount * sizeof(SmpPixel));
            pixelPos += amount;
            x += amount;
            break;
        }
        case EndOfRow:
            if (x != 0xFFFF) {
                if ((x) + m_normalHeader.rowEdges[y].padRight != m_normalHeader.width) {
                    std::cout << x << std::endl;
                    std::cout << m_normalHeader.rowEdges[y].padLeft  << std::endl;
                    std::cout << m_normalHeader.rowEdges[y].padRight  << std::endl;
                    std::cout << ((x) + m_normalHeader.rowEdges[y].padRight)  << std::endl;
                    std::cout << m_normalHeader.width << std::endl;

                }
                assert((x) + m_normalHeader.rowEdges[y].padRight == m_normalHeader.width);
            }
            do {
                y++;
                x = m_normalHeader.rowEdges[y].padLeft;
            } while (y < m_normalHeader.rowEdges.size()  && x == 0xFFFF);

            if (y >= m_normalHeader.rowEdges.size()) {
                x = 0;
            }
            break;
        }
    }
}

void SmxFrame::readShadowGraphics()
{
    serializeFrameHeader(m_shadowHeader);
    if (m_shadowHeader.width > 1000 || m_shadowHeader.height > 1000) {
        std::cerr << "invalid size" << std::endl;
        return;
    }

    uint32_t commandsSize = 0;
    serialize(commandsSize);

    std::vector<uint8_t> commands;
    serialize(commands, commandsSize);
}

void SmxFrame::readOutlineGraphics()
{
    serializeFrameHeader(m_outlineHeader);
    if (m_outlineHeader.width > 1000 || m_outlineHeader.height > 1000) {
        std::cerr << "invalid size" << std::endl;
        return;
    }
    uint32_t commandsSize = 0;
    serialize(commandsSize);

    std::vector<uint8_t> commands;
    serialize(commands, commandsSize);
}

std::vector<SmxFrame::SmxPixel> SmxFrame::decode4Plus1(const std::vector<uint8_t> &data)
{
    std::vector<SmxPixel> pixelsVector;
    pixelsVector.resize(data.size() / 1.25);
    SmxPixel *pixels = pixelsVector.data();

    SmxPixel p0, p1, p2, p3;
    p0.damageMask = 0;
    p0.damageMask = 1;
    size_t pixelsPos = 0;
    for (size_t i=0; i<data.size(); i += 5) {
        p0.index = data[i + 0];
        p1.index = data[i + 1];
        p2.index = data[i + 2];
        p3.index = data[i + 3];

        const uint8_t sections = data[i + 4];
        p0.palette = (sections >> 6) & 0b11;
        p1.palette = (sections >> 4) & 0b11;
        p2.palette = (sections >> 2) & 0b11;
        p3.palette = (sections >> 0) & 0b11;

        pixels[pixelsPos + 0] = p0;
        pixels[pixelsPos + 1] = p1;
        pixels[pixelsPos + 2] = p2;
        pixels[pixelsPos + 3] = p3;
    }

    return pixelsVector;
}


}
