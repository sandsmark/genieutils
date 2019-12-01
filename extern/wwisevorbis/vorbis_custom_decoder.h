#ifndef _VORBIS_CUSTOM_DECODER_H_
#define _VORBIS_CUSTOM_DECODER_H_

#include <vorbis/codec.h>

#include <istream>
#include "coding.h"

typedef int16_t sample_t;

typedef enum { WWV_HEADER_TRIAD, WWV_FULL_SETUP, WWV_INLINE_CODEBOOKS, WWV_EXTERNAL_CODEBOOKS, WWV_AOTUV603_CODEBOOKS } wwise_setup_t;

typedef enum { WWV_TYPE_8, WWV_TYPE_6, WWV_TYPE_2 } wwise_header_t;
typedef enum { WWV_STANDARD, WWV_MODIFIED } wwise_packet_t;

/* custom Vorbis modes */
typedef enum {
    VORBIS_FSB,         /* FMOD FSB: simplified/external setup packets, custom packet headers */
    VORBIS_WWISE,       /* Wwise WEM: many variations (custom setup, headers and data) */
    VORBIS_OGL,         /* Shin'en OGL: custom packet headers */
    VORBIS_SK,          /* Silicon Knights AUD: "OggS" replaced by "SK" */
    VORBIS_VID1,        /* Neversoft VID1: custom packet blocks/headers */
} vorbis_custom_t;

typedef struct {
    /* to reconstruct init packets */
    int channels;
    int sample_rate;
    int blocksize_0_exp;
    int blocksize_1_exp;

    uint32_t setup_id; /* external setup */
    int big_endian; /* flag */

    /* Wwise Vorbis config */
    wwise_setup_t setup_type;
    wwise_header_t header_type;
    wwise_packet_t packet_type;

    /* output (kinda ugly here but to simplify) */
    off_t data_start_offset;

} vorbis_custom_config;

typedef struct {
    std::istream * streamfile;    /* file used by this channel */
    off_t channel_start_offset; /* where data for this channel begins */
    off_t offset;               /* current location in the file */

    off_t frame_header_offset;  /* offset of the current frame header (for WS) */
    int samples_left_in_frame;  /* for WS */

    /* format specific */

    /* adpcm */
    int16_t adpcm_coef[16];     /* for formats with decode coefficients built in */
    int32_t adpcm_coef_3by32[0x60];     /* for Level-5 0x555 */
    union {
        int16_t adpcm_history1_16;  /* previous sample */
        int32_t adpcm_history1_32;
    };
    union {
        int16_t adpcm_history2_16;  /* previous previous sample */
        int32_t adpcm_history2_32;
    };
    union {
        int16_t adpcm_history3_16;
        int32_t adpcm_history3_32;
    };
    union {
        int16_t adpcm_history4_16;
        int32_t adpcm_history4_32;
    };

    double adpcm_history1_double;
    double adpcm_history2_double;

    int adpcm_step_index;       /* for IMA */
    int adpcm_scale;            /* for MS ADPCM */

    /* state for G.721 decoder, sort of big but we might as well keep it around */
//    struct g72x_state g72x_state;

    /* ADX encryption */
    int adx_channels;
    uint16_t adx_xor;
    uint16_t adx_mult;
    uint16_t adx_add;

} VGMSTREAMCHANNEL;

typedef struct {
    vorbis_info vi;             /* stream settings */
    vorbis_comment vc;          /* stream comments */
    vorbis_dsp_state vd;        /* decoder global state */
    vorbis_block vb;            /* decoder local state */
    ogg_packet op;              /* fake packet for internal use */

    uint8_t * buffer;           /* internal raw data buffer */
    size_t buffer_size;

    size_t samples_to_discard;  /* for looping purposes */
    int samples_full;           /* flag, samples available in vorbis buffers */

    vorbis_custom_t type;        /* Vorbis subtype */
    vorbis_custom_config config; /* config depending on the mode */

    /* Wwise Vorbis: saved data to reconstruct modified packets */
    uint8_t mode_blockflag[64+1];   /* max 6b+1; flags 'n stuff */
    int mode_bits;                  /* bits to store mode_number */
    uint8_t prev_blockflag;         /* blockflag in the last decoded packet */
    /* Ogg-style Vorbis: packet within a page */
    int current_packet;
    /* reference for page/blocks */
    off_t block_offset;
    size_t block_size;

    VGMSTREAMCHANNEL * loop_ch;

    int prev_block_samples;     /* count for optimization */

} vorbis_custom_codec_data;

/* used by vorbis_custom_decoder.c, but scattered in other .c files */
int vorbis_custom_setup_init_wwise(std::istream *streamFile, off_t start_offset, vorbis_custom_codec_data *data);

int vorbis_custom_parse_packet_wwise(VGMSTREAMCHANNEL *stream, vorbis_custom_codec_data *data);

#endif/*_VORBIS_CUSTOM_DECODER_H_ */
