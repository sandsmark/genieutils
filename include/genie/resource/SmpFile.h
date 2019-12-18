#pragma once

// Documentation copyright Christoph Heine, because I can't be assed to rewrite what he wrote
// From openage/doc/media/smx-files.md
// From openage/doc/media/smp-files.md

#include "genie/file/IFile.h"
#include "genie/util/Logger.h"

#include "SmpFrame.h"

#include <array>

namespace genie {

/**
 * @brief SMP is the sprite storage format of Age of Empires: Definitive Edition
 */

class SmpFile : public IFile
{
    static constexpr std::array<uint8_t, 4> smpHeader = {'S', 'M', 'P', '$' };

public:
    const SmpFrame &frame(size_t frameNum = 0) {
        if (frameNum >= m_frames.size()) {
            std::cerr << "invalid framenum" << frameNum << std::endl;
            return SmpFrame::null;
        }
        return m_frames[frameNum];
    }

    // ISerializable interface
protected:
    void serializeObject() override;

private:
    static Logger &log;

    /// File descriptor
    /// Always SMPX
    std::array<uint8_t, 4> m_header = {'S', 'M', 'P', '$' };

    /// Version
    /// Ex: 256, 0x00000100 (same value for almost all units)
    int32_t m_version = 0;

    /// Number of frames
    /// Ex: 721, 0x000002D1
    int32_t m_numFrames = 0;

    /// Number of facets
    /// 1, 0x0000001 (almost always 0x00000001)
    int32_t m_facetNum;

    /// Number of frames per facet
    /// 721, 0x000002D1 (0x00000001 for version 0x0B)
    int32_t m_framesPerFacet;

    /// Checksum
    /// 0x8554F6F3
    int32_t m_checksum;

    /// File size in bytes
    /// Ex:  0x003D5800
    int32_t m_size;

    /// Source format
    /// 0x0B (SLP) or 0x0C (PSD)
    int32_t m_source_format;

    /// Comment
    /// 32 bytes
    std::string m_comment;

    std::vector<SmpFrame> m_frames;
};

} // namespace genie
