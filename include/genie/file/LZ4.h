#pragma once

#define LZ4_EXCESS (16+(LZ4_BLOCK_SIZE/255))
#define LZ4_BLOCK_SIZE (8<<20) // 8 MB

struct LZ4
{
    void compress(const int max_chain);
    bool decompress();

private:
    U8 g_buf[LZ4_BLOCK_SIZE+LZ4_BLOCK_SIZE+LZ4_EXCESS];

    FILE* g_in;
    FILE* g_out;
};

