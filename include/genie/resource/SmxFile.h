#pragma once

// Documentation copyright Christoph Heine, because I can't be assed to rewrite what he wrote
// From openage/doc/media/smx-files.md

#include "genie/file/IFile.h"
#include "genie/util/Logger.h"

#include "SmxFrame.h"

#include <array>

namespace genie {

/**
 * @brief SMX is the sprite storage format of Age of Empires 2: Definitive Edition
 *
 * The SMX format is a compressed version of the SMP format that was used during the Beta of the game.
 * In the release version almost all sprite media files are compressed SMX files.
 * It is recommended to read the SMP documentation first to get a better understanding
 * of the concepts of the format.
 */

class SmxFile : public IFile
{
    static constexpr std::array<uint8_t, 4> defaultHeader = {'S', 'M', 'P', 'X' };

public:
    const SmxFrame &frame(size_t frameNum = 0) {
        if (frameNum >= m_frames.size()) {
            std::cerr << "invalid framenum" << frameNum << std::endl;
            return SmxFrame::null;
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
    std::array<uint8_t, 4> m_header = {'S', 'M', 'P', 'X' };

    /// Version
    /// 2, 0x0002 (for almost all units, some have 0x0001)
    uint16_t m_version = 0;

    /// Number of frames
    /// Ex: 961, 0x03C1
    uint16_t m_numFrames = 0;

    /// File size SMX (this file)
    /// Ex: 2706603, 0x000294CAB (size without header)
    uint32_t m_size = 0;

    /// File size SMP (source file)
    /// 6051456, 0x0005C5680 (size without header)
    uint32_t m_sourceSize = 0;

    /// Comment
    /// Always empty
    std::string m_comment;

    std::vector<SmxFrame> m_frames;
};

} // namespace genie
