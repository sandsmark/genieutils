#include "genie/resource/SmpFile.h"

namespace genie {

Logger &SmpFile::log = Logger::getLogger("genie.SmpFile");

void SmpFile::serializeObject()
{
    serialize(m_header);
    if (getOperation() == OP_READ && m_header != smpHeader) {
        // todo throw exception
        log.error("Invalid header % % % %", m_header[0], m_header[1], m_header[2], m_header[3]);
        return;
    }


    serialize(m_version2);
    serialize(m_numFrames);
    serialize(m_size);
    serialize(m_unknown);
    serialize(m_numFrames2);
    serialize(m_checksum);
    serialize(m_byteCount);
    serialize(m_version);
    serialize(m_comment, 32);

    log.warn("ver % frames % size % source % comment %", m_version, m_numFrames, m_size, m_byteCount, m_comment);

    serialize(m_frames, m_numFrames);
}

} // namespace genie
