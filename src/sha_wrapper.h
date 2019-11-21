//
// Created by Inchan Hwang on 2019-11-21.
//

#ifndef CHORD_SHA_WRAPPER_H
#define CHORD_SHA_WRAPPER_H

#include <cstdint>
#include <string>
#include <openssl/sha.h>
#include <stdio.h>

uint32_t extractID(std::string input);

#endif //CHORD_SHA_WRAPPER_H
