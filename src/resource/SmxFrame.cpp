#include "genie/resource/SmxFrame.h"

#include <limits.h>
#include <cmath>

#define IS_LIKELY(x)      __builtin_expect(!!(x), 1)
#define IS_UNLIKELY(x)    __builtin_expect(!!(x), 0)

namespace genie {

Logger &SmxFrame::log = Logger::getLogger("genie.SmxFrame");
SmxFrame SmxFrame::null;

void SmxFrame::serializeObject()
{
//    log.debug(" ======= starting frame at %", getIStream()->tellg());

    serialize<uint8_t>(m_frameHeader.frameType);
    if (m_frameHeader.frameType == 0) {
        std::cerr << "invalid bundle" << std::endl;
        return;
    }
    serialize<uint8_t>(m_frameHeader.palette);
    serialize<uint32_t>(m_frameHeader.size);

    if (m_frameHeader.frameType & FrameHeader::HasDamageModifier) {
        log.debug("Has damage masks");
    }

//    log.debug("Palette: %", m_layerHeader.palette);
    //log.debug("Size: %", m_layerHeader.size);
    //log.debug("Type: %", m_frameHeader.frameType);

    if (getOperation() != OP_READ) {
        log.error("Operation % not supported yet", getOperation());
        return;
    }

    if (m_frameHeader.frameType & FrameHeader::NormalLayer) {
        serializeLayerHeader(m_normalHeader);
        serialize<uint32_t>(m_normalHeader.pixelDataSize);
        m_normalHeader.filePosition = tellg();

        getIStream()->seekg(m_normalHeader.filePosition + std::streampos(m_normalHeader.commandsSize + m_normalHeader.pixelDataSize));
    }

    if (m_frameHeader.frameType & FrameHeader::ShadowLayer) {

        serializeLayerHeader(m_shadowHeader);
        m_shadowHeader.filePosition = tellg();

        getIStream()->seekg(m_shadowHeader.filePosition + std::streampos(m_shadowHeader.commandsSize));
    }

    if (m_frameHeader.frameType & FrameHeader::OutlineLayer) {
        serializeLayerHeader(m_outlineHeader);
        m_outlineHeader.filePosition = tellg();

        getIStream()->seekg(m_outlineHeader.filePosition + std::streampos(m_outlineHeader.commandsSize));
    }

}

bool SmxFrame::load()
{
    if (m_frameHeader.frameType & FrameHeader::NormalLayer) {
        log.debug("Contains normal frame");
        getIStream()->seekg(m_normalHeader.filePosition);
        readNormalLayer();
    }

    if (m_frameHeader.frameType & FrameHeader::ShadowLayer) {
        log.debug("Contains shadow frame");
        getIStream()->seekg(m_shadowHeader.filePosition);
        readShadowLayer();
    }

    if (m_frameHeader.frameType & FrameHeader::OutlineLayer) {
        log.debug("Contains outline frame");
        getIStream()->seekg(m_outlineHeader.filePosition);
        readOutlineLayer();
    }

    return true;
}

void SmxFrame::serializeLayerHeader(SmxFrame::LayerHeader &header)
{
    serialize<uint16_t>(header.width);
    serialize<uint16_t>(header.height);

    serialize<uint16_t>(header.centerX);
    serialize<uint16_t>(header.centerY);

    serialize<uint32_t>(header.size);
    serialize<uint32_t>(header.unknown);

    serialize<LayerHeader::RowEdge>(header.rowEdges, header.height);

    serialize<uint32_t>(header.commandsSize);
    //log.debug("Size: %x%; Hotspot %,%; Size %; Unknown %", header.width, header.height, header.centerX, header.centerY, header.size, header.unknown);
}

void SmxFrame::readNormalLayer()
{
    std::vector<uint8_t> commands;
    serialize<uint8_t>(commands, m_normalHeader.commandsSize);

    std::vector<uint8_t> pixelData;
    serialize<uint8_t>(pixelData, m_normalHeader.pixelDataSize);

    const std::vector<SmpPixel> pixelsVector = m_frameHeader.frameType & FrameHeader::HasDamageModifier ?
                decode8To5(std::move(pixelData)) :
                decode4Plus1(std::move(pixelData));

    if (m_normalHeader.width >= 46340) {
        throw std::out_of_range("Width (" + std::to_string(m_normalHeader.width) + ") out of range");
    }
    if (m_normalHeader.height >= 46340) {
        throw std::out_of_range("Height (" + std::to_string(m_normalHeader.height) + ") out of range");
    }

    (void)0;
    const size_t byteCount = size_t(m_normalHeader.width) * size_t(m_normalHeader.height);

    m_mask.resize(byteCount);
    m_pixels.resize(byteCount);

    SmpPixel *target = m_pixels.data();
    uint8_t *maskData = m_mask.data();
    const SmpPixel *pixels = pixelsVector.data();
    size_t pixelPos = 0;

    uint32_t x = 0, y = 0;
    x = m_normalHeader.rowEdges[0].padLeft;

    for (const uint8_t byte : commands) {
        const uint8_t amount = (byte >> 2) + 1;
        const uint8_t command = byte & 0b11;

        // Dumb check, but keeps static analyzers happy I hope
        if (IS_UNLIKELY(y >= 46340)) {
            throw std::out_of_range("Y (" + std::to_string(y) + ") out of range");
        }

        (void)0;
        const size_t offset = size_t(y) * size_t(m_normalHeader.width);

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
            memset(&maskData[offset + x], 1, amount * sizeof(uint8_t));
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
            } while (y < m_normalHeader.rowEdges.size() && x == 0xFFFF);

            if (y >= m_normalHeader.rowEdges.size()) {
                x = 0;
            }
            break;
        }
    }
}

void SmxFrame::readShadowLayer()
{
    std::vector<uint8_t> commands;
    serialize<uint8_t>(commands, m_shadowHeader.commandsSize);
}

void SmxFrame::readOutlineLayer()
{
    std::vector<uint8_t> commands;
    serialize<uint8_t>(commands, m_outlineHeader.commandsSize);
}

std::vector<SmpPixel> SmxFrame::decode4Plus1(const std::vector<uint8_t> &data)
{
    std::vector<SmpPixel> pixelsVector(data.size() / 1.25);
    SmpPixel *pixels = pixelsVector.data();

    const uint8_t *source = data.data();

    SmpPixel p0, p1, p2, p3;
    p0.damageModifier = 0;
    p1.damageModifier = 0;
    p2.damageModifier = 0;
    p3.damageModifier = 0;

    size_t pixelsPos = 0;
    for (size_t i=0; i<data.size(); i += 5) {
        p0.index = uint32_t(source[i + 0]);
        p1.index = uint32_t(source[i + 1]);
        p2.index = uint32_t(source[i + 2]);
        p3.index = uint32_t(source[i + 3]);

        const uint8_t sections = source[i + 4];
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
    // used for buildings

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
        p0.damageModifier =  ((byte2 << 8) | byte3) & 0xf03f;

        uint16_t part1 = rotateRight2((uint16_t(byte2) << 8) | byte3);
        uint16_t part2 = rotateRight2((uint16_t(byte4) << 8) | byte5);

        p1.index = part1 & 0xff;
        p1.palette =  (part1 >> 10) & 0x3f;
        p1.damageModifier =  part2 & 0xf03f;

        pixels[pixelsPos++] = p0;
        pixels[pixelsPos++] = p1;
    }

    return pixelsVector;
}


}
