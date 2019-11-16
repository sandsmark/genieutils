#pragma once

#include <genie/file/IFile.h>
#include <genie/util/Logger.h>

#include <memory>

#include <stdint.h>

namespace genie {

class WavFile : public IFile
{
public:
    WavFile();

    struct WavHeader {
        // RIFF header
        uint32_t ChunkID{};
        uint32_t ChunkSize{};
        uint32_t Format{};

        // fmt subchunk
        uint32_t Subchunk1ID{};
        uint32_t Subchunk1Size{};

        enum AudioFormats {
            PCM = 0x1,
            ADPCM = 0x2,
            IEEEFloat = 0x3,
            ALaw = 0x6,
            MULaw = 0x7,
            DVIADPCM = 0x11
        };
        uint16_t AudioFormat{};

        uint16_t NumChannels{};
        uint32_t SampleRate{};
        uint32_t ByteRate{};
        uint16_t BlockAlign{};
        uint16_t BitsPerSample{};

        // data subchunk
        uint32_t Subchunk2ID{};
        uint32_t Subchunk2Size{};
    };

    const WavHeader &header() { return m_header; }
    const uint8_t *audioData() { return m_data.data(); }


    // ISerializable interface
protected:
    void serializeObject() override;

private:
    static Logger &log;

    uint32_t m_size = 0;
    uint32_t m_type = 0;
    WavHeader m_header;
    std::vector<uint8_t> m_data;
};

} //namespace genie
