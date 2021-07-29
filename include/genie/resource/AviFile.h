#pragma once

#include "genie/file/IFile.h"

namespace genie {
class AviFile : public IFile
{
    class Header : public ISerializable {
        uint32_t msPerFrame = 0;
        uint32_t maxBytesPerSec = 0;
        uint32_t paddingGranularity = 0;

        enum Flags {
            HasIndex = 1 << 0,
            ForceIndex = 1 << 1,
            Interleaved = 1 << 2,
            CaptureFile = 1 << 3,
            Copyrighted = 1 << 4
        };

        uint32_t flags;

        uint32_t totalFrames;
        uint32_t initialFrames;
        uint32_t streams;
        uint32_t suggestedBufferSize;

        uint32_t width;
        uint32_t height;

        uint32_t reserved1;
        uint32_t reserved2;
        uint32_t reserved3;
        uint32_t reserved4;

        void serializeObject() override;
    };

    void serializeObject() override;

    uint32_t m_fileSize;

    void serializeChunk();
    void readList();

    Header m_header;
};
} // namespace genie
