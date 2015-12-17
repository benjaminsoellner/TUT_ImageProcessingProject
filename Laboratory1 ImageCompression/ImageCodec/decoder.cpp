/*
 * Joonas Meriläinen 176182 joonas.merilainen@tut.fi
 * Benjamin Söllner 206342 benjamin.sollner@tut.fi
 */
#include "decoder.h"

bool decode(const char* inputfile, const char* outputfile, int width, int height, bool rgb) {

	bitstream* instream = open_input_bitstream(inputfile);
	if (instream == NULL) return false;
	int numberOfBlocks = width/8*height/8;
	int numberOfLayers = (rgb ? 3 : 1);
	init_huffman_tables();

	vector<double*> blocks;

	// 2.2.1 VLC Expansion
	// 2.2.3 Differential Expansion
	double lastDC = 0.0;
	for (int block = 0; block < numberOfBlocks; block++) {
		for (int value = 0; value < BLOCK_SIZE; value++) {
			int zigzagPositionX = 1;
			int zigzagPositionY = 0;
			bool zigzagHeadingDown = true;
			if (value == 0) {
				// new block
				blocks.push_back(new double[BLOCK_SIZE]);
				// read DC value, do differential decoding
				lastDC = blocks.at(block)[0] =
					lastDC + (double) getvli(instream, getvlcdc(instream));
			} else {
				int run;
				int category;
				// read AC value
				getvlcac(instream, &run, &category);
				if (run == 0 && category == 0) {
					// "End of block": fill the rest of the block with zeros
					for (; value < BLOCK_SIZE; value++)
						blocks.at(block)[value] = 0.0;
					break;
				} else {
					// jump over the next "run" values
					for (int i = 0; i < run; i++) {
						blocks.at(block)[value] = 0.0;
						value++;
					}
				}
				// save the value calculated from category and vli value
				blocks.at(block)[value] = (double) getvli(instream, category);
			}
		}
	}

	// 2.2.2 Zig-zag-expansion
	for (int block = 0; block < numberOfBlocks; block++) {
		double* unzigzagged = new double[BLOCK_SIZE];
		for (int value = 0; value < BLOCK_SIZE; value++) {
			unzigzagged[value] = blocks.at(block)[ZIGZAG_ORDER[value]];
		}
		delete blocks.at(block);
		blocks.at(block) = unzigzagged;
	}

	// 2.2.4 Rescaling - only luminance component
	for (int block = 0; block < numberOfBlocks; block++) {
		for (int value = 0; value < BLOCK_SIZE; value++) {
			blocks.at(block)[value] = blocks.at(block)[value] * Q[value];
		}
	}

	// 2.2.5 IDCT
	for (int block = 0; block < numberOfBlocks; block++) {
		double* untransformed = new double[BLOCK_SIZE];
		idct(blocks.at(block), untransformed);
		delete blocks.at(block);
		blocks.at(block) = untransformed;
	}

	// This is our final image
	Image image(width, height, numberOfLayers);

	// Unblocking & 2.2.8 Offset
	// 2.2.7 YUV2RGB convertion
	const int blocksPerLine = width/BLOCK_WIDTH;
	for (int block = 0; block < numberOfBlocks; block++) {
		int blockXPos = (block%blocksPerLine) * BLOCK_WIDTH;
		int blockYPos = (block/blocksPerLine) * BLOCK_WIDTH;
		for (int value = 0; value < BLOCK_SIZE; value++) {
			int valueXPos = value%BLOCK_WIDTH;
			int valueYPos = value/BLOCK_WIDTH;
			if (numberOfLayers == 3) {
				double y  = blocks.at(block)[value];
				double cb = 0;
				double cr = 0;
				double r = 1 *y  +0       *cb  +1.402   *cr;
				double g = 1 *y  -0.34414 *cb  -0.71414 *cb;
				double b = 1 *y  +1.772   *cb  +0       *cr;
				r = 128 + floor( r+0.5 );
				g = 128 + floor( g+0.5 );
				b = 128 + floor( b+0.5 );
				int ri = (int) (r > 255 ? 255 : (r < 0 ? 0 : r));
				int gi = (int) (g > 255 ? 255 : (g < 0 ? 0 : g));
				int bi = (int) (b > 255 ? 255 : (b < 0 ? 0 : b));
				image.setPixel(blockYPos+valueYPos, blockXPos+valueXPos, 0, ri);
				image.setPixel(blockYPos+valueYPos, blockXPos+valueXPos, 1, gi);
				image.setPixel(blockYPos+valueYPos, blockXPos+valueXPos, 2, bi);
			} else {
				double y = 128 + (int) floor( blocks.at(block)[value] +0.5 );
				int yi = (int) (y > 255 ? 255 : (y < 0 ? 0 : y));
				image.setPixel(blockYPos+valueYPos, blockXPos+valueXPos, 0, yi);
			}
		}
	}
	
	// Writing to the file
	bitstream* file = open_output_bitstream(outputfile);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < numberOfLayers; c++) {
				putbits(file, image.getPixel(y, x, c), 8);
			}
		}
	}

	// Cleaning up
	close_bitstream(file);
	for (int block = 0; block < numberOfBlocks; block++)
		delete blocks.at(block);

	return true;
}