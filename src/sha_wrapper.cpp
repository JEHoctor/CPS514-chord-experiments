//
// Created by Inchan Hwang on 2019-11-21.
//

#include "sha_wrapper.h"

#define SHA1_LENGTH 20

uint32_t extractID(std::string input) {
    unsigned char ibuf[256];
    unsigned char obuf[SHA1_LENGTH];
    std::copy(input.begin(), input.end(), ibuf);
    SHA1(ibuf, input.length(), obuf);

    uint64_t m = 1;
    uint64_t ret = 0;
    uint64_t modulo = 1ull << 32;
    for (unsigned char i : obuf) {
        ret = (ret + i * m) % modulo;
        m = (m * 16) % modulo;
    }

    return ret;
}
