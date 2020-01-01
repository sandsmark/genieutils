#include "genie/resource/SmxFile.h"

namespace genie {

Logger &SmxFile::log = Logger::getLogger("genie.SmxFile");

void SmxFile::serializeObject()
{
    serialize<uint8_t, 4>(m_header);
    if (getOperation() == OP_READ && m_header != defaultHeader) {
        // todo throw exception
        log.error("Invalid header % % % %", m_header[0], m_header[1], m_header[2], m_header[3]);
        return;
    }

    serialize<uint16_t>(m_version);
    serializeSize<uint16_t>(m_numFrames, m_frames.size());
    serialize<uint32_t>(m_size);
    serialize<uint32_t>(m_sourceSize);

    serialize(m_comment, 16);

    log.warn("ver % frames % size % source % comment %", m_version, m_numFrames, m_size, m_sourceSize, m_comment);

    serialize<SmxFrame>(m_frames, m_numFrames);
}

} // namespace genie
