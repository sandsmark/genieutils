#include "genie/resource/AviFile.h"


// More or less stolen from okteta's definition

enum Tags : uint32_t {
    //#define MAKE_TAG(a0,a1,a2,a3) (uint32_t((a3) | ((a2) << 8) | ((a1) << 16) | ((a0) << 24)))
#define MKTAG(TAGSTR) (uint32_t(((TAGSTR)[3]) | (((TAGSTR)[2]) << 8) | (((TAGSTR)[1]) << 16) | (((TAGSTR)[0]) << 24)))
    // List headers
    ID_LIST = MKTAG("LIST"),
    ID_RIFF = MKTAG("RIFF"),
    ID_JUNK = MKTAG("JUNK"),

    // Lists
    ID_AVI  = MKTAG("AVI "),
    ID_HDRL = MKTAG("hdrl"),
    ID_MOVI = MKTAG("movi"),

    // Chunks
    ID_AVIH = MKTAG("avih"),
    ID_STRH = MKTAG("strh"),
    ID_STRF = MKTAG("strf"),

    // Stream types
    ID_VIDS = MKTAG("vids"),
    ID_AUDS = MKTAG("auds"),
    ID_TXTS = MKTAG("txts"),

    // Codecs
    ID_Indeo5 = MKTAG("IV50"),

    // Index elements, these are the only ones used in aoe videos
    ID_Stream1CompressedVideo = MKTAG("00dc"),
    ID_Stream2UncompressedAudio = MKTAG("01wb"),
#undef MKTAG
};

void genie::AviFile::serializeObject()
{
    uint32_t tag = ID_RIFF;
    serialize<uint32_t>(tag);

    if (tag != ID_RIFF) {
        throw std::runtime_error("Invalid starting tag");
    }

    serialize<uint32_t>(m_fileSize);

    uint32_t formatTag = ID_AVI;
    serialize<uint32_t>(formatTag);
    if (formatTag != ID_AVI) {
        throw std::runtime_error("Missing invalid format tag");
    }

    if (!isOperation(OP_READ)) {
        throw std::runtime_error("Writing not implemented cuz I'm lazy");
    }
    while (getIStream()->good()) {
        uint32_t chunkTag = 0;
        serialize<uint32_t>(chunkTag);
        switch(chunkTag) {
        case ID_LIST:
            readList();
        }
    }
}

void genie::AviFile::readList()
{
    uint32_t size;
    serialize<uint32_t>(size);

    uint32_t listType;
    serialize<uint32_t>(listType);
    switch(listType) {
    case ID_HDRL:
        serialize<ISerializable>(m_header);
        break;
    }
}

void genie::AviFile::Header::serializeObject()
{
    uint32_t fourcc = ID_AVIH;
    serialize<uint32_t>(fourcc);

    uint32_t size = 56;
    serialize<uint32_t>(size);

    serialize<uint32_t>(msPerFrame);
    serialize<uint32_t>(maxBytesPerSec);
    serialize<uint32_t>(paddingGranularity);
    serialize<uint32_t>(flags);
    serialize<uint32_t>(totalFrames);
    serialize<uint32_t>(initialFrames);
    serialize<uint32_t>(streams);
    serialize<uint32_t>(suggestedBufferSize);
    serialize<uint32_t>(width);
    serialize<uint32_t>(height);

    serialize<uint32_t>(reserved1);
    serialize<uint32_t>(reserved2);
    serialize<uint32_t>(reserved3);
    serialize<uint32_t>(reserved4);
}
