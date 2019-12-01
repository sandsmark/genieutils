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
//    log.debug("Size: %x%; Hotspot %,%; Size %; Unknown %", header.width, header.height, header.centerX, header.centerY, header.size, header.unknown);
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

    const std::streampos expectedEnd = getIStream()->tellg() + std::streampos(pixelDataSize);

    std::vector<SmpPixel> unusedPixels;

    m_pixels.resize(m_normalHeader.width * m_normalHeader.height);
    for (const uint8_t byte : commands) {
        int amount = (byte >> 2) + 1;
        const uint8_t command = byte & 0b11;

        const uint32_t offset = y * m_normalHeader.width;

        switch(command) {
        case Skip:
            x += amount;
            break;
        case PlayerColor: {
            //todo refactor reading and use here
            for (int i=0; i<amount; ) {
                while (!unusedPixels.empty() && i < amount) {
                    unusedPixels.pop_back();
                    x++;
                    i++;
                }
                if (i >= amount) {
                    break;
                }
                if ((m_bundleHeader.type & BundleHeader::SkipDamageMask) == 0) {
                    SmpPixel p0, p1, p2, p3;
                    serialize<uint8_t>(p0.index);
                    serialize<uint8_t>(p1.index);
                    serialize<uint8_t>(p2.index);
                    serialize<uint8_t>(p3.index);

                    uint8_t sections = 0;
                    serialize(sections);
                    unusedPixels.push_back(p0);
                    unusedPixels.push_back(p1);
                    unusedPixels.push_back(p2);
                    unusedPixels.push_back(p3);
                } else {
                    exit(1);
                    uint8_t bytes1[3];
                    getIStream()->read(reinterpret_cast<char*>(bytes1), 3);

                    if (i < amount) {
                        x++;
                        i++;
                    }
                    if (i < amount) {
                        uint8_t bytes2[2]{};
                        getIStream()->read(reinterpret_cast<char*>(bytes2), 2);
                        x++;
                        i++;
                    }
                }
            }
            break;
        }
        case Draw: {
            // todo :pull this out into a function or something
            for (int i=0; i<amount; ) {
                while (!unusedPixels.empty() && i < amount) {
                    m_pixels[offset + x++] = std::move(unusedPixels.back());
                    unusedPixels.pop_back();
                    i++;
                }
                if (i >= amount) {
                    break;
                }

                if ((m_bundleHeader.type & BundleHeader::SkipDamageMask) == 0) {
                    SmpPixel p0, p1, p2, p3;
                    uint8_t sections = 0;
                    serialize(p0.index);
                    serialize(p1.index);
                    serialize(p2.index);
                    serialize(p3.index);
                    serialize(sections);

                    p0.palette = (sections >> 6) & 0b11;
                    p1.palette = (sections >> 4) & 0b11;
                    p2.palette = (sections >> 2) & 0b11;
                    p3.palette = (sections >> 0) & 0b11;
                    unusedPixels.push_back(p0);
                    unusedPixels.push_back(p1);
                    unusedPixels.push_back(p2);
                    unusedPixels.push_back(p3);
                } else {
                    exit(1);
                    // What an absolute clusterfuck, how did you figure this out heinezen

                    uint8_t bytes1[2]{};
                    getIStream()->read(reinterpret_cast<char*>(bytes1), 2);

                    SmpPixel p0;
                    serialize(p0.index);
                    p0.palette = bytes1[0] & 0x03;
                    p0.damageMask = bytes1[1] & 0xf0;
                    p0.damageMask2 = bytes1[1] & 0x3f;

                    if (amount-- > 0) {
                        m_pixels[offset + x++] = std::move(p0);
                    } else {
                        std::cerr << " --- too many pixels read?" << std::endl;
                    }
                    if (amount-- > 0) {
                        //uint8_t bytes2[2]{};
                        //getIStream()->read(reinterpret_cast<char*>(bytes2), 2);
                        uint16_t bytes2;
                        serialize(bytes2);
                        uint16_t part1 = rotateRight2((uint16_t(bytes1[0]) << 8) | bytes1[1]);
                        uint16_t part2 = rotateRight2(bytes2);

                        SmpPixel p1;
                        p1.index =       (part1 >> 8) & 0xff;
                        p1.palette =     (part1 >> 0) & 0x03;
                        p1.damageMask =  (part2 >> 8) & 0xf0;
                        p1.damageMask2 = (part2 >> 0) & 0x3f;

                        m_pixels[offset + x++] = std::move(p1);
                    }
                }
            }
            break;
        }
        case EndOfRow:
            assert((x) + m_normalHeader.rowEdges[y].padRight == m_normalHeader.width);
            y++;
            assert(y <= m_normalHeader.rowEdges.size());
            if (y < m_normalHeader.rowEdges.size()) {
                x = m_normalHeader.rowEdges[y].padLeft;
            } else {
                x = 0;
            }
            break;
        }


    }

    if (getIStream()->tellg() > expectedEnd) {
        throw std::runtime_error("Overread when reading frame");
    } else if (getIStream()->tellg() < expectedEnd) {
        throw std::runtime_error("Underread when reading frame");
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
    getIStream()->seekg(getIStream()->tellg() + std::streampos(commandsSize));
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
    getIStream()->seekg(getIStream()->tellg() + std::streampos(commandsSize));
}


}
