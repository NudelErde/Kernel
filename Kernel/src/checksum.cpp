#include "checksum.hpp"

namespace Kernel {


//source: https://github.com/Voxer/sse4_crc32/blob/master/src/table.cpp
constexpr uint32_t CRC32C_POLYNOMIAL = 0x82F63B78;

uint32_t table[8][256];

/**
 * Initializes the CRC-32C lookup table for software-based CRC calculation
 */
void crc32c_init() {
    uint32_t i, j, crc;

    for (i = 0; i < 256; i++) {
        crc = i;
        crc = crc & 1 ? (crc >> 1) ^ CRC32C_POLYNOMIAL : crc >> 1;
        crc = crc & 1 ? (crc >> 1) ^ CRC32C_POLYNOMIAL : crc >> 1;
        crc = crc & 1 ? (crc >> 1) ^ CRC32C_POLYNOMIAL : crc >> 1;
        crc = crc & 1 ? (crc >> 1) ^ CRC32C_POLYNOMIAL : crc >> 1;
        crc = crc & 1 ? (crc >> 1) ^ CRC32C_POLYNOMIAL : crc >> 1;
        crc = crc & 1 ? (crc >> 1) ^ CRC32C_POLYNOMIAL : crc >> 1;
        crc = crc & 1 ? (crc >> 1) ^ CRC32C_POLYNOMIAL : crc >> 1;
        crc = crc & 1 ? (crc >> 1) ^ CRC32C_POLYNOMIAL : crc >> 1;
        table[0][i] = crc;
    }

    for (i = 0; i < 256; i++) {
        crc = table[0][i];
        for (j = 1; j < 8; j++) {
            crc = table[0][crc & 0xff] ^ (crc >> 8);
            table[j][i] = crc;
        }
    }
}

uint32_t crc32c(uint32_t initCrc, const uint8_t* buf, uint64_t len) {
    const uint8_t* next = buf;
    uint64_t crc = initCrc;

    // If the string is empty, return 0
    if (len == 0) {
        return (uint32_t) crc;
    }

    // XOR the initial CRC with INT_MAX
    crc ^= 0xFFFFFFFF;

    // Process byte-by-byte until aligned to 8-byte boundary
    while (len && ((uint64_t) next & 7) != 0) {
        crc = table[0][(crc ^ *next++) & 0xff] ^ (crc >> 8);
        len--;
    }

    // Process 8 bytes at a time
    while (len >= 8) {
        crc ^= *(uint64_t*) next;
        crc = table[7][(crc >> 0) & 0xff] ^
              table[6][(crc >> 8) & 0xff] ^
              table[5][(crc >> 16) & 0xff] ^
              table[4][(crc >> 24) & 0xff] ^
              table[3][(crc >> 32) & 0xff] ^
              table[2][(crc >> 40) & 0xff] ^
              table[1][(crc >> 48) & 0xff] ^
              table[0][(crc >> 56)];
        next += 8;
        len -= 8;
    }

    // Process any remaining bytes
    while (len) {
        crc = table[0][(crc ^ *next++) & 0xff] ^ (crc >> 8);
        len--;
    }

    // XOR again with INT_MAX
    return (uint32_t) (crc ^= 0xFFFFFFFF);
}

}// namespace Kernel
