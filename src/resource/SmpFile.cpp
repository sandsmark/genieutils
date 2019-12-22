#include "genie/resource/SmpFile.h"

namespace genie {

Logger &SmpFile::log = Logger::getLogger("genie.SmpFile");

void SmpFile::unload()
{
    if ((!m_loaded)) {
        log.error("Trying to unload, but not loaded");
    }

    m_frames.clear();
    m_loaded = false;
}

void SmpFile::serializeObject()
{
    serialize(m_header);
    if (getOperation() == OP_READ && m_header != smpHeader) {
        // todo throw exception
        log.error("Invalid header % % % %", m_header[0], m_header[1], m_header[2], m_header[3]);
        return;
    }
    std::cout << m_header[0] << m_header[1] << m_header[2] << m_header[3] << std::endl;

    serialize(m_version);
    serializeSize(m_numFrames, m_frames.size());
    serialize(m_numFacets);
    serialize(m_framesPerFacet);
    serialize(m_checksum);
    serialize(m_byteCount);
    serialize(m_sourceFormat);
    serialize(m_comment, 32);

    log.warn("ver % frames % size % source % comment %", m_version, m_numFrames, m_size, m_byteCount, m_comment);

    serialize(m_frameOffsets, m_numFrames);
    m_frames.resize(m_numFrames);

    for (size_t i=0; i<m_numFrames; i++) {
        if (!m_frames[i]) {
            m_frames[i] = std::make_shared<SmpFrame>();
        }
        m_frames[i]->setInitialReadPosition(getInitialReadPosition() + std::streampos(m_frameOffsets[i]));
        //m_frames[i]->serializeHeader();
        m_frames[i]->readObject(*getIStream());

//        serialize(m_frames[i]);
    }

//    serialize(m_frames, m_numFrames);

    m_loaded = true;
}

} // namespace genie
