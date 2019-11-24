#include "genie/resource/SmxFile.h"

namespace genie {

Logger &SmxFile::log = Logger::getLogger("genie.SmxFile");

void SmxFile::serializeObject()
{
    serialize(m_header);
    if (getOperation() == OP_READ && m_header != defaultHeader) {
        // todo throw exception
        log.error("Invalid header % % % %", m_header[0], m_header[1], m_header[2], m_header[3]);
        return;
    }

    serialize(m_version);
    serialize(m_numFrames);
    serialize(m_size);
    serialize(m_sourceSize);

    serialize(m_comment, 16);

    log.warn("ver % frames % size % source % comment %", m_version, m_numFrames, m_size, m_sourceSize, m_comment);

    serialize(m_frames, m_numFrames);
}

} // namespace genie
