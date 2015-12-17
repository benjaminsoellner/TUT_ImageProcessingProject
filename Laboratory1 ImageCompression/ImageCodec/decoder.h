/*
 * Joonas Meriläinen 176182 joonas.merilainen@tut.fi
 * Benjamin Söllner 206342 benjamin.sollner@tut.fi
 */

#ifndef _DECODER_H_
#define _DECODER_H_

#include <cmath>
#include <vector>
#include <iostream>
#include "lib/bitstream.h"
#include "lib/dct.h"
#include "utils.h"

using namespace std;

const int BLOCK_SIZE_UNSUBSAMPLED = 64;

bool decode(const char* inputfile, const char* outputfile, int width, int height, bool rgb);

#endif