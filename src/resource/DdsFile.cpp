
#include "genie/resource/DdsFile.h"

#include <bitset>

namespace genie {

void DdsFile::serializeObject()
{
    if (isOperation(OP_WRITE)) {
        assert(m_magic == ddsFourCC);
        m_magic = ddsFourCC;
    }

    serialize<uint32_t>(m_magic);
    if (m_magic != ddsFourCC) {
        throw std::ios_base::failure("Invalid magic " + std::to_string(m_magic));
    }
    serialize<Header>(header);
    if ((header.flags & Header::necessaryFlags) != Header::necessaryFlags) {
        throw std::ios_base::failure("Unsupported format"); // sue me, am lazy, only implement what's necessary for aoe2
    }
    if ((header.pixelFormat.flags & Header::PixelFormat::HasFourCC) == 0 || header.pixelFormat.fourCC != dxt1FourCC) {
        throw std::ios_base::failure("Unsupported pixel format"); // sue me, am lazy, only implement what's necessary for aoe2
    }

//    if (header.flags & Header::HasCaps) std::cout << "has caps" << std::endl;
//    if (header.flags & Header::HasHeight) std::cout << "has height" << std::endl;
//    if (header.flags & Header::HasWidth) std::cout << "has width" << std::endl;
//    if (header.flags & Header::HasPitch) std::cout << "has pitch" << std::endl;
//    if (header.flags & Header::HasPixelformat) std::cout << "has pixelformat" << std::endl;
//    if (header.flags & Header::HasMipmapCount) std::cout << "has mipmapcount" << std::endl;
//    if (header.flags & Header::HasLinearSize) std::cout << "has linearsize" << std::endl;
//    if (header.flags & Header::HasDepth) std::cout << "has depth" << std::endl;

    std::cout << "flags " << header.flags << std::endl;
    std::cout << "caps: " << header.caps << " caps2: " << header.caps2 << std::endl;
    std::cout << "format size: " << header.pixelFormat.size << std::endl;
    std::cout << "bits: "
              << " rgb: " << header.pixelFormat.rgbBitCount
              << " red: " << std::bitset<sizeof(uint32_t)>(header.pixelFormat.rBitMask).count()
              << " green: " << std::bitset<sizeof(uint32_t)>(header.pixelFormat.gBitMask).count()
              << " blue: " << std::bitset<sizeof(uint32_t)>(header.pixelFormat.bBitMask).count()
              << " alpha: " << std::bitset<sizeof(uint32_t)>(header.pixelFormat.aBitMask).count()
              << std::endl;

    int blocksize = 0;
    switch(header.pixelFormat.fourCC) {
    case dxt1FourCC:
        blocksize = 8;
        break;
    default:
        assert(false && "only dxt 1 implemented");
        throw std::ios_base::failure("Unsupported format");
    }

    assert(!isOperation(OP_WRITE) || m_data.size() == header.mipMapCount);

    m_data.resize(header.mipMapCount);
    std::cout << "mipmaps: " << header.mipMapCount << std::endl;

    for (uint32_t mip = 0; mip < header.mipMapCount; mip++) {
        const int width = std::max<int>(header.width >> mip, 1);
        const int height = std::max<int>((header.height >> mip) / 4, 1);
        const int depth = std::max<int>(header.depth >> mip, 1);
        const int pitch = std::max(blocksize, blocksize * ((width + 3) / 4));

        std::cout << "size: " << width << "x" << height << "x" << depth << std::endl;
        std::cout << "pitch: " << pitch << std::endl;
        std::cout << "to read " << (depth * height * pitch) << std::endl;

        m_data[mip].resize(depth * height * pitch);
        std::cout << tellg() << std::endl;
        getIStream()->read((char*)m_data[mip].data(), depth * height * pitch);
//        serialize(m_data[mip], depth * height * pitch);
    }
    std::cout << tellg() << std::endl;
//    getIStream()->seekg(0, std::ios_base::end);
//    std::cout << tellg() << std::endl;
    if (isOperation(OP_READ)) {
        decodeDxt1(m_data[0]); // idk
    } else {
        throw std::ios_base::failure("Writing not implemented");
    }
}

inline void unpack565(uint8_t *dest, uint16_t packed) {
    const uint32_t r = (packed >> 11) & 0x1f;
    const uint32_t g = (packed >>  5) & 0x3f;
    const uint32_t b = (packed >>  0) & 0x1f;
    dest[0] = uint8_t((r << 3) | (r >> 2));
    dest[1] = uint8_t((g << 2) | (g >> 4));
    dest[2] = uint8_t((b << 3) | (b >> 2));
}

std::vector<uint32_t> DdsFile::decodeDxt1(const std::vector<uint8_t> &compressed)
{
    /**
    DXT1 (also known as Block Compression 1 or BC1) is the smallest variation of
    S3TC, storing 16 input pixels in 64 bits of output, consisting of two 16-bit
    RGB 5:6:5 color values c0 and c1, and a 4x4 two-bit lookup table.

    If c0 > c1 (compare these colors by interpreting them as two 16-bit
    unsigned numbers), then two other colors are calculated, such that for each
    component, c2=(2 * c0) / 3 + c1/3 and c3= c0/3 + (c1 * 2) / 3.

    Otherwise, if c0 <= c1, then c2=c0/2 + c1/2 and c2 = c0/2 + c_1/2 and c3 is
    transparent black corresponding to a premultiplied alpha format. This color
    sometimes causes a black border surrounding the transparent area when
    linear texture filtering is used, due to colors being interpolated between
    the color of opaque texel and neighbouring black transparent texel.

    The lookup table is then consulted to determine the color value for each
    pixel, with a value of 0 corresponding to c0 and a value of 3 corresponding
    to c3.
*/
    // not the most efficient and only little endian
    const uint8_t *source = compressed.data();
    const uint16_t *sourceColor = reinterpret_cast<const uint16_t*>(source);

    uint16_t a = sourceColor[0];
    uint16_t b = sourceColor[1];

//    uint32_t color[4];
//    std::vector<uint8_t> targetData(4 * 4);
    std::vector<uint32_t> target(4);
    uint8_t *targetData = reinterpret_cast<uint8_t*>(target.data());
    unpack565(targetData, a);
    unpack565(targetData + 4, b);

    targetData[3] = 0xff; // alpha
    targetData[7] = 0xff; // alpha

    if (a > b) {
        targetData[8] = uint8_t(uint16_t(targetData[0] * 2 + targetData[4]) / 3);
        targetData[9] = uint8_t(uint16_t(targetData[1] * 2 + targetData[5]) / 3);
        targetData[10] = uint8_t(uint16_t(targetData[2] * 2 + targetData[6]) / 3);
        targetData[11] = 0xff; // alpha

        // bitcode 11
        targetData[12] = uint8_t(uint16_t(targetData[0] + targetData[4] * 2) / 3);
        targetData[13] = uint8_t(uint16_t(targetData[1] + targetData[5] * 2) / 3);
        targetData[14] = uint8_t(uint16_t(targetData[2] + targetData[6] * 2) / 3);
        targetData[15] = 0xff; // alpha
    } else {
        // bit code 10
        targetData[8] = uint8_t(uint16_t(targetData[0] + targetData[4]) / 2);
        targetData[9] = uint8_t(uint16_t(targetData[1] + targetData[5]) / 2);
        targetData[10] = uint8_t(uint16_t(targetData[2] + targetData[6]) / 2);
        targetData[11] = 0xff; // alpha

        // bit code 11: transparent
        targetData[12] = 0;
        targetData[13] = 0;
        targetData[14] = 0;
        targetData[15] = 0;
    }

    std::vector<uint32_t> destVec(header.width * header.height);
    uint8_t *dest = reinterpret_cast<uint8_t*>(destVec.data());
    uint32_t data = *reinterpret_cast<const uint32_t*>(&source[5]);
    for (int y = 0; y < 4; ++y) {
        uint32_t* d = reinterpret_cast<uint32_t*>(dest);
        d[0] = targetData[(data >> 0) & 3];
        d[1] = targetData[(data >> 2) & 3];
        d[2] = targetData[(data >> 4) & 3];
        d[3] = targetData[(data >> 6) & 3];
        data >>= 8;
        dest += 4;
    }
//    uint32_t *targetColor = reinterpret_cast<uint32_t>()

    return destVec;
}

}
