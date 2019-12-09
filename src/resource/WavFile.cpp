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
    serialize<WavHeader>(m_header);
    if (m_header.Subchunk2Size >= std::numeric_limits<uint32_t>::max()) {
        log.warn("I seriously doubt there's a wav file this big, remove this check if that's the case");
        return;
    }

    serialize<uint8_t>(m_data, m_header.Subchunk2Size - sizeof(WavHeader));
}

} // namespace genie
