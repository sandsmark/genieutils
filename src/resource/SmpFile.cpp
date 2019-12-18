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

    serialize(m_version);
    serialize(m_numFrames);
    serialize(m_facetNum);
    serialize(m_framesPerFacet);
    serialize(m_checksum);
    serialize(m_size);
    serialize(m_source_format);
    serialize(m_comment, 32);

    log.warn("ver % frames % size % comment %", m_version, m_numFrames, m_size, m_comment);

    serialize(m_frames, m_numFrames);
}

} // namespace genie
