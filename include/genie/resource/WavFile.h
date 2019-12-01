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

    struct RiffHeader {
        static constexpr std::array<uint8_t, 4> expectedId = {'R', 'I', 'F', 'F'};
        std::array<uint8_t, 4> id = {'R', 'I', 'F', 'F'};

        uint32_t dataSize = 0; // the file size minus riff and wave header
    };

    struct WaveHeader {
        static constexpr std::array<uint8_t, 4> expectedId = {'W', 'A', 'V', 'E'};
        std::array<uint8_t, 4> id = {'W', 'A', 'V', 'E'};
    };

    struct ChunkHeader {
        enum ChunkTypes : uint32_t {
            Format = 0x20746d66u, // 'fmt '
            Data = 0x61746164u, // 'data'

            // the rest are 'cue ', 'LIST', 'smpl and 'vorb',
            // but I can't be assed to convert to hex mentally
        } type{};

        uint32_t size{};
    };

    struct WavFormatChunk : public ChunkHeader {
        enum AudioFormats : uint16_t {
            PCM = 0x1,
            ADPCM = 0x2,
            IEEEFloat = 0x3,
            ALaw = 0x6,
            MULaw = 0x7,
            DVIADPCM = 0x11,
            AAC = 0xff,
            Wwise = 0xffffu,
        } codec{};

        uint16_t numChannels{};
        uint32_t sampleRate{};
        uint32_t byteRate{};
        uint16_t blockAlign{};
        uint16_t bitsPerSample{};
    };

    struct WavHeader {
        RiffHeader riffHeader;
        WaveHeader waveHeader;

        // We hardcode the chunk layout here (because that's how all files seem to be in practice)
        WavFormatChunk format;
    };

    struct WwiseData {
        // ??
        uint16_t formatLength2;
        uint16_t unknown1{};
        uint16_t subType{};
        char whoknows[0x18];
//        uint16_t unknown2[0x10];
    } __attribute__((packed));

    const WavFormatChunk &format() { return m_header.format; }
    const uint8_t *audioData() { return m_data.data(); }


    // ISerializable interface
protected:
    void serializeObject() override;

private:
    static Logger &log;

    uint32_t m_size = 0;
    uint32_t m_type = 0;

    WavHeader m_header;
    WwiseData m_extraData;

    ChunkHeader m_dataHeader;

    std::vector<uint8_t> m_data;
};

} //namespace genie
