/*
 * Joonas Meriläinen 176182 joonas.merilainen@tut.fi
 * Benjamin Söllner 206342 benjamin.sollner@tut.fi
 *
 * This is a C++ file and compiles therefore with g++
 * _not_ gcc.
 */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "lib/bitstream.h"
#include "lib/dct.h"
#include "encoder.h"
#include "decoder.h"

using namespace std;

int main(int argc, char* argv[]) {

	//If user doesn't give width and height, 256 is set by default
	int width = 256;
	int height = 256;
	
	//Checks that at least 4 parameters were given
	if(argc < 4) {
		cout << "Not enough parameters!" << endl;
		cout << "Use this syntax: icodec [encode|decode|encodergb|decodergb] input output w h" << endl;
		return false;
	}

	string mode = argv[1];
	string input = argv[2];
	string output = argv[3];
		
	//If width and height were given, transformation to integer is done
	if(argc > 5) {	
		string s_width = argv[4];
		string s_height = argv[5];
		istringstream widthStream(s_width);
		istringstream heightStream(s_height);
		widthStream >> width;
		heightStream >> height;
	}
	
	//Acts to given action
	if (mode=="encode") {
		if (encode(input.c_str(), output.c_str(), width, height, false)) {
			cerr << "Given input file was not found." << endl;
		}
	} else if(mode=="decode") {
		if (decode(input.c_str(), output.c_str(), width, height, false)) {
			cerr << "Given input file was not found." << endl;
		}
	} else if(mode=="encodergb") {
		if (encode(input.c_str(), output.c_str(), width, height, true)) {
			cerr << "Given input file was not found." << endl;
		}
	} else if(mode=="decodergb") {
		if (decode(input.c_str(), output.c_str(), width, height, true)) {
			cerr << "Given input file was not found." << endl;
		}
	} else {
		cout<<"Invalid input value for mode."<<endl;
	}
	
	return EXIT_SUCCESS;

}
