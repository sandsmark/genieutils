#include "genie/resource/SmpFrame.h"

#define IS_LIKELY(x)      __builtin_expect(!!(x), 1)
#define IS_UNLIKELY(x)    __builtin_expect(!!(x), 0)

namespace genie {

Logger &SmpFrame::log = Logger::getLogger("genie.SmpFrame");
SmpFrame SmpFrame::null;

const SmpBaseLayer &SmpFrame::baseLayer()
{
    if (!hasBaseLayer()) {
        throw std::out_of_range("No base layer!");
    }
    if (!m_baseLayer->isLoaded) {
        loadLayer(*m_baseLayer);
        loadLayerContent(*m_baseLayer);
    }
    return *m_baseLayer;
}

const SmpSimpleLayer &SmpFrame::shadowLayer()
{
    if (!hasShadowLayer()) {
        throw std::out_of_range("No shadow layer");
    }
    if (!m_shadowLayer->isLoaded) {
        loadLayer(*m_shadowLayer);
        loadLayerContent(*m_shadowLayer);
    }
    return *m_shadowLayer;
}

const SmpSimpleLayer &SmpFrame::outlineLayer()
{
    if (!hasShadowLayer()) {
        throw std::out_of_range("No outline layer");
    }
    if (!m_shadowLayer->isLoaded) {
        loadLayer(*m_shadowLayer);
        loadLayerContent(*m_shadowLayer);
    }
    return *m_shadowLayer;
}

void SmpFrame::serializeObject()
{
    // Skip 28 unused bytes (same as SLP frame header, but unused)
    //  uint32 width, height, hotspotX, hotspotY, type, diffusePaletteXid, diffusePaletteNum
    getIStream()->seekg(28, std::ios_base::cur);

    serialize<uint32_t>(m_layerCount);
    if (!m_layerCount) {
        return;
    }

    m_baseLayer = std::make_unique<SmpBaseLayer>();

    static_assert(sizeof(SmpLayerHeader) == 32);
    serialize<SmpLayerHeader>(m_baseLayer->header);

    for (uint32_t i=1; i<m_layerCount; i++) {
        std::unique_ptr<SmpSimpleLayer> layer = std::make_unique<SmpSimpleLayer>();
        serialize<SmpLayerHeader>(layer->header);
        if (layer->header.type == SmpLayerHeader::Shadow) {
            m_shadowLayer = std::move(layer);
        } else if (layer->header.type == SmpLayerHeader::Outline) {
            m_outlineLayer = std::move(layer);
        } else {
            log.warn("Invalid layer type %", layer->header.type);
        }
    }
}

void SmpFrame::loadLayer(SmpLayer &layer)
{
    std::istream *istr = getIStream();
    std::cout << "Loaded" << std::endl;
    istr->seekg(getInitialReadPosition() + std::streampos(layer.header.paddingTableOffset));
    layer.leftEdges.resize(layer.header.height);
    layer.rightEdges.resize(layer.header.height);
    for (uint32_t y=0; y<layer.header.height; y++) {
        serialize<uint16_t>(layer.leftEdges[y]);
        serialize<uint16_t>(layer.rightEdges[y]);
    }


    istr->seekg(getInitialReadPosition() + std::streampos(layer.header.pixelDataOffset));
    layer.rowOffsets.resize(layer.header.height);
    serialize<uint32_t>(layer.rowOffsets, layer.header.height);

    log.debug("width: % height: % hsx: % hsx: %", layer.header.width, layer.header.height, layer.header.hotspotX, layer.header.hotspotY);
    log.debug("padding table: % pixel data: %", layer.header.paddingTableOffset, layer.header.pixelDataOffset);
}

void SmpFrame::loadLayerContent(SmpBaseLayer &layer)
{
    std::istream *istr = getIStream();

    if (layer.header.width >= 46340) {
        throw std::out_of_range("Layer width (" + std::to_string(layer.header.width) + ") out of range");
    }

    if (layer.header.height >= 46340) {
        throw std::out_of_range("Layer height (" + std::to_string(layer.header.height) + ") out of range");
    }

    (void)0;
    const size_t pixelCount = size_t(layer.header.width)*(layer.header.height);
    layer.pixels.resize(pixelCount);
    layer.alphaMask.resize(pixelCount);

    SmpPixel *pixelData = layer.pixels.data();
    uint8_t *alphaData = layer.alphaMask.data();
    memset(alphaData, 0, layer.alphaMask.size());

    for (uint32_t row = 0; row < layer.header.height; ++row) {
        if (0xFFFF == layer.leftEdges[row] || 0xFFFF == layer.rightEdges[row]) {
            continue; // Pretend it does not exist.
        }

        // don't really need to use them, everything is sequential
        istr->seekg(getInitialReadPosition() + std::streampos(layer.rowOffsets[row]));

        uint32_t pixelPos = layer.leftEdges[row];
        if (pixelPos >= layer.header.width) {
            throw std::out_of_range("Left edge (" + std::to_string(pixelPos) + ") bigger than width (" + std::to_string(layer.leftEdges[row]) + ")");
        }

        const uint32_t toRead = int(layer.header.width) - layer.rightEdges[row];

        while (pixelPos < toRead && istr->good()) {
            const uint8_t data = read<uint8_t>();
            if (data == EndOfRow) {
                break;
            }

            const uint32_t count = (data >> 2) + 1;
            const size_t dataOffset = row * layer.header.width + pixelPos;

            const uint8_t command = data & 0b11;

            if (command == SkipPixels) {
                pixelPos += count;
                continue;

            }

            read(&pixelData[dataOffset], count);
            memset(&alphaData[dataOffset], 255, count);

            if (command == DrawPlayerColor) {
                for (uint32_t i = 0; i<count; i++)  {
                    layer.playerColors.push_back({ pixelData[dataOffset + i], i + pixelPos, row  });
                }
            }

            pixelPos += count;
        }
    }
    layer.isLoaded = true;
}

void SmpFrame::loadLayerContent(SmpSimpleLayer &layer)
{
    std::istream *istr = getIStream();

    if (layer.header.width >= 46340) {
        throw std::out_of_range("Layer width (" + std::to_string(layer.header.width) + ") out of range");
    }
    if (layer.header.height >= 46340) {
        throw std::out_of_range("Layer height (" + std::to_string(layer.header.height) + ") out of range");
    }

    (void)0;

    const size_t pixelCount = size_t(layer.header.width) * size_t(layer.header.height);
    layer.colors.resize(pixelCount);

    uint8_t *pixelData = layer.colors.data();

    for (uint32_t row = 0; row < layer.header.height; ++row) {
        if (0xFFFF == layer.leftEdges[row] || 0xFFFF == layer.rightEdges[row]) {
            std::cout << "skipping" << std::endl;
            continue; // Pretend it does not exist.
        }

        // don't really need to use them, everything is sequential
        istr->seekg(getInitialReadPosition() + std::streampos(layer.rowOffsets[row]));

        uint32_t pixelPos = layer.leftEdges[row];
        if (IS_UNLIKELY(pixelPos >= layer.header.width)) {
            throw std::out_of_range("Left edge (" + std::to_string(pixelPos) + ") bigger than width (" + std::to_string(layer.leftEdges[row]) + ")");
        }

        const uint32_t toRead = layer.header.width - layer.rightEdges[row];
        if (IS_UNLIKELY(toRead + pixelPos > layer.header.width)) {
            throw std::out_of_range("Trying to read too much (" + std::to_string(toRead + pixelPos) + ") compared to width (" + std::to_string(layer.header.width) + ")");
        }

        while (pixelPos < toRead && istr->good()) {
            const uint8_t data = read<uint8_t>();
            if (data == EndOfRow) {
                break;
            }

            const uint32_t count = (data >> 2) + 1;
            const size_t dataOffset = row * layer.header.width + pixelPos;

            const uint8_t command = data & 0b11;

            if (command == SkipPixels) {
                pixelPos += count;
                continue;

            }

            read(&pixelData[dataOffset], count);

            pixelPos += count;
        }
    }
    layer.isLoaded = true;

}

}//namespace genie
