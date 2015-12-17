/*
 * Joonas Meriläinen 176182 joonas.merilainen@tut.fi
 * Benjamin Söllner 206342 benjamin.sollner@tut.fi
 */

#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <cmath>
#include <iostream>
#include <vector>
#include "lib/bitstream.h"
#include "lib/dct.h"
#include "utils.h"

using namespace std;

void encode_write_bitstream(vector<double>* diff_values, vector<vector<double> >* ac_values, const char* path);
vector<double> encode_zigzag_diff(vector<vector<double> >* blocks, vector<vector<double> >* ac_values);
void encode_quantize(vector<vector<double> >* blocks);
void encode_dct(vector<vector<double> >* blocks);
vector<vector<vector<double> > > encode_makeBlocks(Image* image);
bool encode(const char* inputfile, const char* outputfile, int width, int height, bool rgb);

#endif