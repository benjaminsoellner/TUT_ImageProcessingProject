/*
 * Joonas Meriläinen 176182 joonas.merilainen@tut.fi
 * Benjamin Söllner 206342 benjamin.sollner@tut.fi
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

const int ZIGZAG_ORDER[] =
  {  0,  1,  5,  6, 14, 15, 27, 28,
     2,  4,  7, 13, 16, 26, 29, 42,
     3,  8, 12, 17, 25, 30, 41, 43,
	 9, 11, 18, 24, 31, 40, 44, 53,
	10, 19, 23, 32, 39, 45, 52, 54,
	20, 22, 33, 38, 46, 51, 55, 60,
	21, 34, 37, 47, 50, 56, 59, 61,
	35, 36, 48, 49, 57, 58, 62, 63 };

const int Q[] =
  { 16, 11, 10, 16,  24,  40,  51,  61,
    12, 12, 14, 19,  26,  58,  60,  55,
	14, 13, 16, 24,  40,  57,  69,  56,
	14, 17, 22, 29,  51,  87,  80,  62,
	18, 22, 37, 56,  68, 109, 103,  77,
	24, 35, 55, 64,  81, 104, 113,  92,
	49, 64, 78, 87, 103, 121, 120, 101,
	72, 92, 95, 98, 112, 100, 103,  99 };

const int BLOCK_SIZE  = 64;
const int BLOCK_WIDTH = 8;

class Image {
	private:
		int width;
		int height;
		int bytesperpixel;
		signed int* pixels;

	public:

		Image(int width, int height, int bytesperpixel):
					width(width), height(height), bytesperpixel(bytesperpixel) {
			this->pixels = new int[this->width * this->height * this->bytesperpixel];
		}

		~Image() {
			delete this->pixels;
		}

		int getPixel(int y, int x, int component) {
			return this->pixels[this->width * this->bytesperpixel * y + this->bytesperpixel * x + component];
		}

		void setPixel(int y, int x, int component, signed int value) {
			this->pixels[this->width * this->bytesperpixel * y + this->bytesperpixel * x + component] = value;
		}

		int getWidth() {return width;}
		int getHeight() {return height;}

		void convertPixelRGB2YUV(int y, int x) {
			signed int R = this->getPixel(y, x, 0);
			signed int G = this->getPixel(y, x, 1);
			signed int B = this->getPixel(y, x, 2);
			signed int Y =  (signed int) (0.299*R+0.587*G+0.114*B);
			signed int Cb = (signed int) ((-0.1687)*R+(-0.3313)*G+0.5*B);
			signed int Cr = (signed int) (0.5*R+(-0.4187*G)+(-0.0813*B));
			this->setPixel(y, x, 0, Y);
			this->setPixel(y, x, 1, Cb);
			this->setPixel(y, x, 2, Cr);
		}

};

#endif