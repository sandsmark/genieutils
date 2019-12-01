#include "genie/resource/SmxFrame.h"

#include <limits.h>
#include <cmath>

namespace genie {

Logger &SmxFrame::log = Logger::getLogger("genie.SmxFrame");

void SmxFrame::serializeObject()
{
    std::cerr << " ======= starting bundle at " << getIStream()->tellg() << std::endl;
    serialize(m_bundleHeader.type);
    if (m_bundleHeader.type == 0) {
        std::cerr << "invalid bundle" << std::endl;
        return;
    }
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
    exit(0);

    if (m_bundleHeader.type & BundleHeader::OutlineFrame) {
        std::cerr << "outline frame" << std::endl;
        readOutlineGraphics();
    }

}

void SmxFrame::serializeFrameHeader(SmxFrame::FrameHeader &header)
{
    std::cerr << " ----------- starting frame at " << getIStream()->tellg() << " ------------" << std::endl;

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
    std::cout << "before sizes: " << (getIStream()->tellg()) << std::endl;

    uint32_t commandsSize = 0;
    serialize(commandsSize);

    uint32_t pixelDataSize = 0;
    serialize(pixelDataSize);

    std::cout << "after sizes: " << (getIStream()->tellg()) << std::endl;


    std::cout << "commands: " << commandsSize << std::endl;
    std::cout << "start: " << (getIStream()->tellg()) << std::endl;

    std::vector<uint8_t> commands;
    serialize(commands, commandsSize);
    std::cout << "after commands: " << (getIStream()->tellg()) << std::endl;
    uint32_t x=0, y=0;
    x = m_normalHeader.rowEdges[0].padLeft;

    std::streampos expectedEnd = getIStream()->tellg() + std::streampos(pixelDataSize);
    std::streampos posbefore = getIStream()->tellg();
    std::cout << "expected end: " << expectedEnd << std::endl;

    size_t shadowRead = 0;
    size_t normalRead = 0;
    size_t pixelsRead = 0;
    size_t skipped = 0;
    size_t expectPixelsRead = 0;
    int expectPixelsRead2 = 0;

    m_pixels.resize(m_normalHeader.width * m_normalHeader.height);
    for (const uint8_t byte : commands) {
        std::cout << "x " << x << " y " << y << std::endl;
        int amount = (byte >> 2) + 1;
        const uint8_t command = byte & 0b11;

        const uint32_t offset = y * m_normalHeader.width;

        switch(command) {
        case Skip:
            std::cout << " - skipping " << amount << std::endl;
            skipped += amount;
            x += amount;
            break;
        case PlayerColor: {
            std::cout << " + player color " << int(amount) << std::endl;
            std::streampos posbefore = getIStream()->tellg();
            int expectDataRead = std::ceil(amount / 4.) * 5;
            expectPixelsRead += amount;
            //todo refactor reading and use here
            for (int i=0; i<amount; ) {
                std::streampos posbefore = getIStream()->tellg();
                if ((m_bundleHeader.type & BundleHeader::SkipDamageMask) == 0) {
                    SmpPixel p0, p1, p2, p3;
                    serialize<uint8_t>(p0.index);
                    //if (amount > 1) {
                        serialize<uint8_t>(p1.index);
                    //}
                    //if (amount > 2) {
                        serialize<uint8_t>(p2.index);
                    //}
                    //if (amount > 3) {
                        serialize<uint8_t>(p3.index);
                    //}

                    uint8_t sections = 0;
                    serialize(sections);

                    if (i < amount) {
                        pixelsRead++;
                        x++;
                        i++;
                    }
                    if (i < amount) {
                        pixelsRead++;
                        x++;
                        i++;
                    }
                    if (i < amount) {
                        pixelsRead++;
                        x++;
                        i++;
                    }
                    if (i < amount) {
                        pixelsRead++;
                        x++;
                        i++;
                    }
                } else {
                    exit(1);
                    uint8_t bytes1[3]{};
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
                if  (getIStream()->tellg() - posbefore != 5) {
                    std::cout << " * read " << (getIStream()->tellg() - posbefore) << " bytes" << std::endl;
                    std::cerr << "read invalid amount of bytes" << std::endl;
                    exit(1);
                }
            }
            if  (getIStream()->tellg() - posbefore != expectDataRead) {
                std::cout << " * read " << (getIStream()->tellg() - posbefore) << " bytes" << std::endl;
                std::cerr << "read invalid amount of bytes" << std::endl;
                exit(1);
            }
            shadowRead += (getIStream()->tellg() - posbefore);
            //std::cout << " + read " << (getIStream()->tellg() - posbefore) << " bytes" << std::endl;

            break;
        }
        case Draw: {
            std::cout << " * normal draw " << int(amount) << std::endl;
            int expectDataRead = std::ceil(amount / 4.) * 5;
            expectPixelsRead += amount;
            std::streampos posbefore = getIStream()->tellg();
            // todo :pull this out into a function or something
            for (int i=0; i<amount; ) {
                std::streampos posbefore = getIStream()->tellg();
                if ((m_bundleHeader.type & BundleHeader::SkipDamageMask) == 0) {
                    //std::cout << "adding 4" << std::endl;
                    SmpPixel p0, p1, p2, p3;
                    uint8_t sections = 0;
                    serialize(p0.index);
                    //if (amount > 1) {
                        serialize(p1.index);
                    //}
                    //if (amount > 2) {
                        serialize(p2.index);
                    //}
                    //if (amount > 3) {
                        serialize(p3.index);
                    //}
                    serialize(sections);

                    p0.palette = (sections >> 6) & 0b11;
                    p1.palette = (sections >> 4) & 0b11;
                    p2.palette = (sections >> 2) & 0b11;
                    p3.palette = (sections >> 0) & 0b11;

                    if (i < amount) {
                        m_pixels[offset + x++] = std::move(p0);
                        pixelsRead++;
                        i++;
                    }
                    if (i < amount) {
                        m_pixels[offset + x++] = std::move(p1);
                        pixelsRead++;
                        i++;
                    }
                    if (i < amount) {
                        m_pixels[offset + x++] = std::move(p2);
                        pixelsRead++;
                        i++;
                    }
                    if (i < amount) {
                        m_pixels[offset + x++] = std::move(p3);
                        pixelsRead++;
                        i++;
                    }
                } else {
                    exit(1);
                    //std::cout << "adding 2" << std::endl;
                    // What an absolute clusterfuck, how did you figure this out heinezen

                    uint8_t bytes1[2]{};
                    getIStream()->read(reinterpret_cast<char*>(bytes1), 2);

                    SmpPixel p0;
                    serialize(p0.index);
                    //p0.index = bytes1[0] & 0xff;
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
                if  (getIStream()->tellg() - posbefore != 5) {
                    std::cout << " * read " << (getIStream()->tellg() - posbefore) << " bytes" << std::endl;
                    std::cerr << "read invalid amount of bytes" << std::endl;
                    exit(1);
                }
            }
            normalRead += (getIStream()->tellg() - posbefore);
            if  (getIStream()->tellg() - posbefore != expectDataRead) {
                    std::cout << " * read " << (getIStream()->tellg() - posbefore) << " bytes" << std::endl;
                    std::cerr << "read invalid amount of bytes" << std::endl;
                    exit(1);
            }
            //std::cout << " * read " << (getIStream()->tellg() - posbefore) << " bytes" << std::endl;
            break;
        }
        case EndOfRow:
            expectPixelsRead2 += ((x) -m_normalHeader.rowEdges[y].padLeft);
            std::cout << "-------" << std::endl;
            //std::cout << " ! pixels read: " << (x) << std::endl;
            //std::cout << " ! full width: " << ((x) +m_normalHeader.rowEdges[y].padRight) << std::endl;
//            std::cout << " ! expected: " << (m_normalHeader.width - (m_normalHeader.rowEdges[y].padLeft + m_normalHeader.rowEdges[y].padRight) + 1) << std::endl;
            //std::cout << " ! frame width: " << m_normalHeader.width << std::endl;
            //std::cout << " ! padleft: " << m_normalHeader.rowEdges[y].padLeft << " padright "  << m_normalHeader.rowEdges[y].padRight << std::endl;
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
    std::cout << "height " << m_normalHeader.height << std::endl;
    std::cout << "pixels read: " << pixelsRead << std::endl;
    std::cout << "expect pixels read: " << expectPixelsRead << std::endl;
    std::cout << "expect pixels read 2: " << (expectPixelsRead2 - skipped) << std::endl;
    std::cout << "bytes read: " << (shadowRead + normalRead) << std::endl;
    std::cout << "expect bytes read: " << pixelDataSize << std::endl;
    std::cout << "expected pos  " << expectedEnd << " but is at " << getIStream()->tellg() << std::endl;
    if (getIStream()->tellg() > expectedEnd) {
        std::cerr << "overshoot: " << (getIStream()->tellg() - expectedEnd) << std::endl;
    } else if (getIStream()->tellg() < expectedEnd) {
        std::cerr << "undershoot: " << (expectedEnd - getIStream()->tellg()) << std::endl;
    }
    std::cerr << "bytes for shadow frame: " << shadowRead << std::endl;
    std::cerr << "bytes for normal frame: " << normalRead << std::endl;
    //getIStream()->seekg(expectedEnd);
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


}
