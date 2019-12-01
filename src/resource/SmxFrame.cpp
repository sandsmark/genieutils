#include "genie/resource/SmxFrame.h"

#include <limits.h>
#include <cmath>

namespace genie {

Logger &SmxFrame::log = Logger::getLogger("genie.SmxFrame");
SmxFrame SmxFrame::null;

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

    if (m_bundleHeader.type & BundleHeader::HasDamageMask) {
        log.debug("Has damage masks");
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
//    log.debug("Size: %x%; Hotspot %,%; Size %; Unknown %", header.width, header.height, header.centerX, header.centerY, header.size, header.unknown);
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

    const std::vector<SmpPixel> pixelsVector = m_bundleHeader.type & BundleHeader::HasDamageMask ?
                decode8To5(std::move(pixelData)) :
                decode4Plus1(std::move(pixelData));

    const SmpPixel *pixels = pixelsVector.data();
    size_t pixelPos = 0;

    m_mask.resize(m_normalHeader.width * m_normalHeader.height);
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
            for (size_t i=0; i<amount; i++) {
                m_playerColorPixels.push_back({x++, y, pixels[pixelPos++]});
            }
            break;
        }
        case Draw: {
            memcpy(&target[offset + x], &pixels[pixelPos], amount * sizeof(SmpPixel));
            pixelPos += amount;
            for (size_t i=0; i<amount; i++) {
                m_mask[y * m_normalHeader.width + x + i] = true;
            }
            x += amount;
            break;
        }
        case EndOfRow:
            if ((x) + m_normalHeader.rowEdges[y].padRight != m_normalHeader.width) {
                std::cout << x << std::endl;
                std::cout << m_normalHeader.rowEdges[y].padLeft  << std::endl;
                std::cout << m_normalHeader.rowEdges[y].padRight  << std::endl;
                std::cout << ((x) + m_normalHeader.rowEdges[y].padRight)  << std::endl;
                std::cout << m_normalHeader.width << std::endl;
                throw std::runtime_error("Failed to read data for frame");
            }
            assert((x) + m_normalHeader.rowEdges[y].padRight == m_normalHeader.width);
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

std::vector<SmpPixel> SmxFrame::decode4Plus1(const std::vector<uint8_t> &data)
{
    std::vector<SmpPixel> pixelsVector;
    pixelsVector.resize(data.size() / 1.25);
    SmpPixel *pixels = pixelsVector.data();

    SmpPixel p0, p1, p2, p3;
    p0.damageMask = 0;
    p0.damageMask2 = 0;
    p1.damageMask = 0;
    p1.damageMask2 = 0;
    p2.damageMask = 0;
    p2.damageMask2 = 0;
    p3.damageMask = 0;
    p3.damageMask2 = 0;

    size_t pixelsPos = 0;
    for (size_t i=0; i<data.size(); i += 5) {
        p0.index = data[i + 3];
        p1.index = data[i + 2];
        p2.index = data[i + 1];
        p3.index = data[i + 0];

        const uint8_t sections = data[i + 4];
        p0.palette = (sections >> 0) & 0b11;
        p1.palette = (sections >> 2) & 0b11;
        p2.palette = (sections >> 4) & 0b11;
        p3.palette = (sections >> 6) & 0b11;

        pixels[pixelsPos++] = p0;
        pixels[pixelsPos++] = p1;
        pixels[pixelsPos++] = p2;
        pixels[pixelsPos++] = p3;
    }

    return pixelsVector;
}

static inline uint16_t rotateRight2(const uint16_t n)
{
    return (n >> 2) | (n << (16 - 2));
}

std::vector<SmpPixel> SmxFrame::decode8To5(const std::vector<uint8_t> &data)
{
    // untested, haven't found any that needs this yet

    std::vector<SmpPixel> pixelsVector;
    pixelsVector.resize(0.4 * data.size());
    SmpPixel *pixels = pixelsVector.data();

    SmpPixel p0, p1;
    size_t pixelsPos = 0;

    for (size_t i=0; i<data.size(); i += 5) {
        const uint8_t byte1 = data[i + 0];
        const uint8_t byte2 = data[i + 1];
        const uint8_t byte3 = data[i + 2];
        const uint8_t byte4 = data[i + 3];
        const uint8_t byte5 = data[i + 4];

        p0.index =       byte1 & 0xff;
        p0.palette =     byte1 & 0x03;
        p0.damageMask =  byte2 & 0xf0;
        p0.damageMask2 = byte2 & 0x3f;

        uint16_t part1 = rotateRight2((uint16_t(byte2) << 8) | byte3);
        uint16_t part2 = rotateRight2((uint16_t(byte4) << 8) | byte5);

        p1.index =       (part1 >> 8) & 0xff;
        p1.palette =     (part1 >> 0) & 0x03;
        p1.damageMask =  (part2 >> 8) & 0xf0;
        p1.damageMask2 = (part2 >> 0) & 0x3f;

        pixels[pixelsPos++] = p0;
        pixels[pixelsPos++] = p1;
    }

    return pixelsVector;
}


}
