#include "genie/resource/WavFile.h"

#include <memory>

#include <limits>

namespace genie {
Logger &WavFile::log = Logger::getLogger("freeaoe.DrsFile");

WavFile::WavFile()
{

}

void WavFile::serializeObject()
{
    static_assert(sizeof(RiffHeader) == 8);
    static_assert(sizeof(WaveHeader) == 4);
    static_assert(sizeof(ChunkHeader) == 8);
    static_assert(sizeof(WavFormatChunk) == 24);
//    static_assert(sizeof(WwiseData) == 48);

    serialize<WavHeader>(m_header);
    if (m_header.riffHeader.id != RiffHeader::expectedId) {
        log.warn("Invalid riff header %", m_header.riffHeader.id.data());
        return;
    }

    if (m_header.waveHeader.id != WaveHeader::expectedId) {
        log.warn("Invalid wave header %", m_header.waveHeader.id.data());
        return;
    }

    if (m_header.format.codec == WavFormatChunk::Wwise) {
        serialize<WwiseData>(m_extraData);
        log.debug("align %", m_header.format.blockAlign);
        log.debug("bps %", m_header.format.bitsPerSample);
        log.debug("extra %", m_extraData.formatLength2);
        log.debug("unk %", m_extraData.unknown1);
        log.debug("subtype %", m_extraData.subType);

        if (m_header.format.size != sizeof(WavFormatChunk) + sizeof(WwiseData)) {
//            for (size_t i=0; i<0x18; i++) {
//                std::cout << std::hex << (uint(m_extraData.whoknows[i]) & 0xff) << std::endl;

//            }
            log.warn("Invalid format chunk, size is % but expected %", m_header.format.size, (sizeof(WavFormatChunk) + sizeof(WwiseData)));
            return;
        }
    }

    serialize<ChunkHeader>(m_dataHeader);

    if (m_dataHeader.size >= std::numeric_limits<int>::max()) {
        log.warn("I seriously doubt there's a wav file this big, remove this check if that's the case");
        return;
    }

    serialize<uint8_t>(m_data, m_dataHeader.size - sizeof(ChunkHeader));
    log.warn("test");
}

} // namespace genie
