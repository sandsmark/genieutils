#include "genie/resource/SmxFrame.h"

#include <limits.h>

namespace genie {

Logger &SmxFrame::log = Logger::getLogger("genie.SmxFrame");

void SmxFrame::serializeObject()
{
    std::cerr << "starting bundle at " << getIStream()->tellg() << std::endl;
    serialize(m_bundleHeader.type);
    serialize(m_bundleHeader.palette);
    serialize(m_bundleHeader.maybeSize);

    if (m_bundleHeader.type & BundleHeader::SkipDamageMask) {
        std::cout << "supercompressed" << std::endl;
    }
    std::cerr << "palette: " << int(m_bundleHeader.palette) << std::endl;
    std::cerr << "maybesize: " << m_bundleHeader.maybeSize << std::endl;
    std::cerr << "type: " << uint16_t(m_bundleHeader.type) << std::endl;

    if (getOperation() != OP_READ) {
        log.error("Operation not supported yet");
        return;
    }

    if (m_bundleHeader.type & BundleHeader::NormalFrame) {
        std::cerr << "normal frame" << std::endl;
        readNormalGraphics();
    }

    if (m_bundleHeader.type & BundleHeader::ShadowFrame) {
        std::cerr << "shadow frame" << std::endl;
        readShadowGraphics();
    }

    if (m_bundleHeader.type & BundleHeader::OutlineFrame) {
        std::cerr << "outline frame" << std::endl;
        readOutlineGraphics();
    }

}

void SmxFrame::serializeFrameHeader(SmxFrame::FrameHeader &header)
{
    std::cerr << "starting frame at " << getIStream()->tellg() << std::endl;

    serialize(header.width);
    serialize(header.height);

    serialize(header.centerX);
    serialize(header.centerY);

    serialize(header.size);
    serialize(header.unknown);

    serialize(header.rowEdges, header.height);
    std::cout << "width: " << header.width << std::endl;
    std::cout << "height: " << header.height << std::endl;
    std::cout << "cx: " << header.centerX << std::endl;
    std::cout << "cy: " << header.centerY << std::endl;
    std::cout << "size: " << header.size << std::endl;
    std::cout << "unk: " << header.unknown << std::endl;

}

static inline uint16_t rotateRight2(const uint16_t n)
{
    return (n >> 2) | (n << (16 - 2));
}

void SmxFrame::readNormalGraphics()
{
    serializeFrameHeader(m_normalHeader);

    serialize(m_commandsSize);
    serialize(m_pixeldataSize);

    std::cout << "commands: " << m_commandsSize << std::endl;
    std::cout << "pixels: " << m_pixeldataSize << std::endl;

    std::vector<uint8_t> commands;
    serialize(commands, m_commandsSize);
    uint32_t x=0, y=0;

    m_pixels.resize(m_normalHeader.width * m_normalHeader.height);
    int foo =0;
    for (const uint8_t byte : commands) {
        foo++;
        std::cout << "x " << x << " y " << y << std::endl;
        const uint8_t amount = (byte >> 2) + 1;
        const uint8_t command = byte & 0b11;

        const uint32_t offset = y * m_normalHeader.width;

        switch(command) {
        case Skip:
            std::cout << "skipping " << amount << std::endl;
            x += amount;
            break;
        case PlayerColor:
            std::cout << "player color " << amount << std::endl;
            //todo
            for (int i=0; i<amount; i++) {
                uint32_t dummy;
                serialize(dummy);
                uint8_t dummy2;
                serialize(dummy2);
                if (m_bundleHeader.type & BundleHeader::SkipDamageMask) {
                    x += 4;
                } else {
                    x += 2;
                }
            }

            break;
        case Draw: {
            std::cout << "normal draw " << amount << std::endl;
            // todo :pull this out into a function or something
            for (int i=0; i<amount; i++) {
                if (m_bundleHeader.type & BundleHeader::SkipDamageMask) {
                    SmpPixel p0, p1, p2, p3;
                    serialize(p0.index);
                    serialize(p1.index);
                    serialize(p2.index);
                    serialize(p3.index);

                    uint8_t sections = 0;
                    serialize(sections);
                    p0.palette = (sections >> 6) & 0b11;
                    p1.palette = (sections >> 4) & 0b11;
                    p2.palette = (sections >> 2) & 0b11;
                    p3.palette = (sections >> 0) & 0b11;
                    m_pixels[offset + x++] = std::move(p0);
                    m_pixels[offset + x++] = std::move(p1);
                    m_pixels[offset + x++] = std::move(p2);
                    m_pixels[offset + x++] = std::move(p3);
                } else {
                    // What an absolute clusterfuck, how did you figure this out heinezen

                    uint8_t bytes[5];
                    getIStream()->read(reinterpret_cast<char*>(bytes), 5);

                    SmpPixel p0;
                    p0.index = bytes[0] & 0xff;
                    p0.palette = bytes[1] & 0x03;
                    p0.damageMask = bytes[2] & 0xf0;
                    p0.damageMask2 = bytes[2] & 0x3f;

                    uint16_t part1 = rotateRight2((uint16_t(bytes[1]) << 8) | bytes[2]);
                    uint16_t part2 = rotateRight2((uint16_t(bytes[3]) << 8) | bytes[4]);

                    SmpPixel p1;
                    p1.index =       (part1 >> 8) & 0xff;
                    p1.palette =     (part1 >> 0) & 0x03;
                    p1.damageMask =  (part2 >> 8) & 0xf0;
                    p1.damageMask2 = (part2 >> 0) & 0x3f;

                    m_pixels[offset + x++] = std::move(p0);
                    m_pixels[offset + x++] = std::move(p1);
                }
            }
            break;
        }
        case EndOfRow:
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
}

void SmxFrame::readShadowGraphics()
{
    serializeFrameHeader(m_shadowHeader);
    serialize(m_commandsSize);
}

void SmxFrame::readOutlineGraphics()
{
    serializeFrameHeader(m_outlineHeader);
    serialize(m_commandsSize);
}


}
