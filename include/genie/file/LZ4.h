#pragma once

#include <cstdint>
#include <cstdio>

#define LZ4_EXCESS (16+(LZ4_BLOCK_SIZE/255))
#define LZ4_BLOCK_SIZE (8<<20) // 8 MB

struct LZ4
{
    void compress(FILE* g_in, FILE* g_out, const int max_chain);
    bool decompress(FILE* g_in, FILE* g_out);
    void compress_optimal(FILE* g_in, FILE* g_out);

private:
    inline void wild_copy(int d, int s, int n);
    int compress_block(const int n, const int max_chain);
    bool decompress_block(int &p, int ip, const int ip_end);
    
    uint8_t g_buf[LZ4_BLOCK_SIZE+LZ4_BLOCK_SIZE+LZ4_EXCESS];
};

