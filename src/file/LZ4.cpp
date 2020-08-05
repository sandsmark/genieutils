/*

LZ4X - An optimized LZ4 compressor

Written and placed in the public domain by Ilya Muravyov

Originally from https://github.com/encode84/lz4x

*/

#include "genie/file/LZ4.h"

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <algorithm>

#ifndef _MSC_VER
#  define _ftelli64 ftello64
#endif

#define LZ4_MAGIC 0x184C2102
#define PADDING_LITERALS 5

#define WINDOW_BITS 16
#define WINDOW_SIZE (1<<WINDOW_BITS)
#define WINDOW_MASK (WINDOW_SIZE-1)

#define MIN_MATCH 4

#define LOAD_16(p) (*reinterpret_cast<const uint16_t*>(&g_buf[p]))
#define LOAD_32(p) (*reinterpret_cast<const uint32_t*>(&g_buf[p]))
#define STORE_16(p, x) (*reinterpret_cast<uint16_t*>(&g_buf[p])=(x))
#define COPY_32(d, s) (*reinterpret_cast<uint32_t*>(&g_buf[d])=LOAD_32(s))

#define HASH_BITS 18
#define HASH_SIZE (1<<HASH_BITS)
#define NIL (-1)

#define HASH_32(p) ((LOAD_32(p)*0x9E3779B9)>>(32-HASH_BITS))

inline void LZ4::wild_copy(int dest, const int source, const int count)
{
    COPY_32(dest, source);
    COPY_32(dest + 4, source + 4);

    for (int i = 8; i < count; i += 8) {
        COPY_32(dest + i, source + i);
        COPY_32(dest + 4 + i, source + 4 + i);
    }
}

int LZ4::compress_block(const int n, const int max_chain)
{
    static int head[HASH_SIZE];
    static int tail[WINDOW_SIZE];

    for (int i = 0; i < HASH_SIZE; ++i) {
        head[i] = NIL;
    }

    int op = LZ4_BLOCK_SIZE;
    int pp = 0;

    int p = 0;

    while (p < n) {
        int best_len = 0;
        int dist = 0;

        const int max_match = (n - PADDING_LITERALS) - p;

        if (max_match >= std::max(12 - PADDING_LITERALS, MIN_MATCH)) {
            const int limit = std::max(p - WINDOW_SIZE, NIL);
            int chain_len = max_chain;

            int s = head[HASH_32(p)];

            while (s > limit) {
                if (g_buf[s + best_len] == g_buf[p + best_len] && LOAD_32(s) == LOAD_32(p)) {
                    int len = MIN_MATCH;

                    while (len < max_match && g_buf[s + len] == g_buf[p + len]) {
                        ++len;
                    }

                    if (len > best_len) {
                        best_len = len;
                        dist = p - s;

                        if (len == max_match) {
                            break;
                        }
                    }
                }

                if (--chain_len == 0) {
                    break;
                }

                s = tail[s & WINDOW_MASK];
            }
        }

        if (best_len >= MIN_MATCH) {
            int len = best_len - MIN_MATCH;
            const int nib = std::min(len, 15);

            if (pp != p) {
                const int run = p - pp;

                if (run >= 15) {
                    g_buf[op++] = 0xF0 + nib;

                    int j = run - 15;

                    for (; j >= 255; j -= 255) {
                        g_buf[op++] = 0xFF;
                    }

                    g_buf[op++] = j;
                } else {
                    g_buf[op++] = (run << 4) + nib;
                }

                wild_copy(op, pp, run);
                op += run;
            } else {
                g_buf[op++] = nib;
            }

            STORE_16(op, dist);
            op += 2;

            if (len >= 0xF) {
                len -= 0xF;

                for (; len >= 255; len -= 255) {
                    g_buf[op++] = 0xFF;
                }

                g_buf[op++] = len;
            }

            pp = p + best_len;

            while (p < pp) {
                const uint32_t h = HASH_32(p);
                tail[p & WINDOW_MASK] = head[h];
                head[h] = p++;
            }
        } else {
            const uint32_t h = HASH_32(p);
            tail[p & WINDOW_MASK] = head[h];
            head[h] = p++;
        }
    }

    if (pp != p) {
        const int run = p - pp;

        if (run >= 0xF) {
            g_buf[op++] = 0xF0;

            int j = run - 0xF;

            for (; j >= 255; j -= 255) {
                g_buf[op++] = 0xFF;
            }

            g_buf[op++] = j;
        } else {
            g_buf[op++] = run << 4;
        }

        wild_copy(op, pp, run);
        op += run;
    }

    return op;
}

void LZ4::compress(FILE* g_in, FILE* g_out, const int max_chain)
{
    int n;

    while ((n = fread(g_buf, 1, LZ4_BLOCK_SIZE, g_in)) > 0) {
        const int op = compress_block(n, max_chain);

        const int comp_len = op - LZ4_BLOCK_SIZE;
        fwrite(&comp_len, 1, sizeof(comp_len), g_out);
        fwrite(&g_buf[LZ4_BLOCK_SIZE], 1, comp_len, g_out);

        fprintf(stderr, "%ld -> %ld\r", _ftelli64(g_in), _ftelli64(g_out));
    }
}

void LZ4::compress_optimal(FILE* g_in, FILE* g_out)
{
    static int head[HASH_SIZE];
    static int nodes[WINDOW_SIZE][2];
    static struct {
        int cum;

        int len;
        int dist;
    } path[LZ4_BLOCK_SIZE + 1];

    int n;

    while ((n = fread(g_buf, 1, LZ4_BLOCK_SIZE, g_in)) > 0) {
        // Pass 1: Find all matches

        for (int i = 0; i < HASH_SIZE; ++i) {
            head[i] = NIL;
        }

        for (int p = 0; p < n; ++p) {
            int best_len = 0;
            int dist = 0;

            const int max_match = (n - PADDING_LITERALS) - p;

            if (max_match >= std::max(12 - PADDING_LITERALS, MIN_MATCH)) {
                const int limit = std::max(p - WINDOW_SIZE, NIL);

                int *left = &nodes[p & WINDOW_MASK][1];
                int *right = &nodes[p & WINDOW_MASK][0];

                int left_len = 0;
                int right_len = 0;

                const uint32_t h = HASH_32(p);
                int s = head[h];
                head[h] = p;

                while (s > limit) {
                    int len = std::min(left_len, right_len);

                    if (g_buf[s + len] == g_buf[p + len]) {
                        while (++len < max_match && g_buf[s + len] == g_buf[p + len]);

                        if (len > best_len) {
                            best_len = len;
                            dist = p - s;

                            if (len == max_match || len >= (1 << 16)) {
                                break;
                            }
                        }
                    }

                    if (g_buf[s + len] < g_buf[p + len]) {
                        *right = s;
                        right = &nodes[s & WINDOW_MASK][1];
                        s = *right;
                        right_len = len;
                    } else {
                        *left = s;
                        left = &nodes[s & WINDOW_MASK][0];
                        s = *left;
                        left_len = len;
                    }
                }

                *left = NIL;
                *right = NIL;
            }

            path[p].len = best_len;
            path[p].dist = dist;
        }

        // Pass 2: Build the shortest path

        path[n].cum = 0;

        int count = 15;

        for (int p = n - 1; p > 0; --p) {
            int c0 = path[p + 1].cum + 1;

            if (--count == 0) {
                count = 255;
                ++c0;
            }

            int len = path[p].len;

            if (len >= MIN_MATCH) {
                int c1 = 1 << 30;

                const int j = std::max(len - 255, MIN_MATCH);

                for (int i = len; i >= j; --i) {
                    int tmp = path[p + i].cum + 3;

                    if (i >= (15 + MIN_MATCH)) {
                        tmp += 1 + ((i - (0xF + MIN_MATCH)) / 0xFF);
                    }

                    if (tmp < c1) {
                        c1 = tmp;
                        len = i;
                    }
                }

                if (c1 <= c0) {
                    path[p].cum = c1;
                    path[p].len = len;

                    count = 15;
                } else {
                    path[p].cum = c0;
                    path[p].len = 0;
                }
            } else {
                path[p].cum = c0;
            }
        }

        // Pass 3: Output the codes

        int op = LZ4_BLOCK_SIZE;
        int pp = 0;

        int p = 0;

        while (p < n) {
            if (path[p].len >= MIN_MATCH) {
                int len = path[p].len - MIN_MATCH;
                const int nib = std::min(len, 15);

                if (pp != p) {
                    const int run = p - pp;

                    if (run >= 15) {
                        g_buf[op++] = 0xF0 + nib;

                        int j = run - 15;

                        for (; j >= 255; j -= 255) {
                            g_buf[op++] = 0xFF;
                        }

                        g_buf[op++] = j;
                    } else {
                        g_buf[op++] = (run << 4) + nib;
                    }

                    wild_copy(op, pp, run);
                    op += run;
                } else {
                    g_buf[op++] = nib;
                }

                STORE_16(op, path[p].dist);
                op += 2;

                if (len >= 15) {
                    len -= 15;

                    for (; len >= 255; len -= 255) {
                        g_buf[op++] = 0xFF;
                    }

                    g_buf[op++] = len;
                }

                p += path[p].len;

                pp = p;
            } else {
                ++p;
            }
        }

        if (pp != p) {
            const int run = p - pp;

            if (run >= 15) {
                g_buf[op++] = 0xF0;

                int j = run - 15;

                for (; j >= 255; j -= 255) {
                    g_buf[op++] = 0xFF;
                }

                g_buf[op++] = j;
            } else {
                g_buf[op++] = run << 4;
            }

            wild_copy(op, pp, run);
            op += run;
        }

        const int comp_len = op - LZ4_BLOCK_SIZE;
        fwrite(&comp_len, 1, sizeof(comp_len), g_out);
        fwrite(&g_buf[LZ4_BLOCK_SIZE], 1, comp_len, g_out);

        fprintf(stderr, "%ld -> %ld\r", _ftelli64(g_in), _ftelli64(g_out));
    }
}

bool LZ4::decompress_block(int &p, int ip, const int ip_end)
{
    for (;;) {
        const int token = g_buf[ip++];

        if (token >= 0x10) {
            int run = token >> 4;

            if (run == 0xF) {
                for (;;) {
                    const int c = g_buf[ip++];
                    run += c;

                    if (c != 0xFF) {
                        break;
                    }
                }
            }

            if ((p + run) > LZ4_BLOCK_SIZE) {
                return false;
            }

            wild_copy(p, ip, run);
            p += run;
            ip += run;

            if (ip >= ip_end) {
                break;
            }
        }

        int s = p - LOAD_16(ip);
        ip += 2;

        if (s < 0) {
            return false;
        }

        int len = (token & 0xF) + MIN_MATCH;

        if (len == (0xF + MIN_MATCH)) {
            for (;;) {
                const int c = g_buf[ip++];
                len += c;

                if (c != 255) {
                    break;
                }
            }
        }

        if ((p + len) > LZ4_BLOCK_SIZE) {
            return false;
        }

        if ((p - s) >= 4) {
            wild_copy(p, s, len);
            p += len;
        } else {
            while (len-- != 0) {
                g_buf[p++] = g_buf[s++];
            }
        }
    }
    return true;
}

bool LZ4::decompress(FILE* g_in, FILE* g_out)
{
    int comp_len;

    while (fread(&comp_len, 1, sizeof(comp_len), g_in) > 0) {
        if (comp_len == LZ4_MAGIC) {
            continue;
        }

        if (comp_len < 2 || comp_len > (LZ4_BLOCK_SIZE + LZ4_EXCESS)
                || fread(&g_buf[LZ4_BLOCK_SIZE], 1, comp_len, g_in) != size_t(comp_len)) {
            return false;
        }

        int p = 0;

        int ip = LZ4_BLOCK_SIZE;
        const int ip_end = ip + comp_len;

        if (!decompress_block(p, ip, ip_end)) {
            return false;
        }

        if (fwrite(g_buf, 1, p, g_out) != size_t(p)) {
            perror("Fwrite() failed");
            return false;
        }
    }

    return true;
}
