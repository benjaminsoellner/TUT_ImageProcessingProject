/*
 * Joonas Meriläinen 176182 joonas.merilainen@tut.fi
 * Benjamin Söllner 206342 benjamin.sollner@tut.fi
 */
#include "encoder.h"

void encode_write_bitstream(vector<double>* diff_values, vector<vector<double> >* ac_values, const char* path)
{
	int block_count = diff_values->size();
	int category = NULL;
	bitstream *output_stream = open_output_bitstream(path);

	for (int block = 0; block < block_count; block++)
	{
		category = solve_category(diff_values->at(block));
		putvlcdc(output_stream, category);

		for (int i = 0; i < 63; i++)
		{
		}


	}




}

vector<double> encode_zigzag_diff(vector<vector<double> >* blocks, vector<vector<double> >* ac_values)
{
	vector<double> zigzag(63);
	vector<double> dc_values;
	vector<double> diff_values;
	double previous_dc = 0;
	int block_count = blocks->size();
	
	for (int block = 0; block < block_count; block++)
	{
		//Copy DC value from each block to dc_values
		dc_values.push_back(blocks->at(block).at(0));

		//Copy AC values from blocks to zigzag
		for (int i = 1; i < 64; i++) {
			zigzag.at(ZIGZAG_ORDER[i]-1) = blocks->at(block).at(i);
		}
		
		//Save zigzag block to ac_values
		ac_values->push_back(zigzag);
	}

	//Calculates DIFF values from dc_values
	for (unsigned int i = 0; i < dc_values.size(); i++)
	{
		diff_values.push_back(dc_values.at(i)-previous_dc);
		previous_dc = dc_values.at(i);
	}


	return diff_values;
}


void encode_quantize(vector<vector<double> >* blocks)
{
	double dct_value[64];
	double quant_value[64];
	int block_count = blocks->size();

	for (int block = 0; block < block_count; block++)
	{
		//Calculate quantized values
		for (int i = 0; i < 64; i++) {
			dct_value[i] = blocks->at(block).at(i);
			quant_value[i] = floor((dct_value[i]/Q[i])+0.5);
		}
		
		//Put quantized values back to blocks
		for (int i = 0; i < 64; i++) {
			blocks->at(block).at(i) = quant_value[i];
		}
	}
}

void encode_dct(vector<vector<double> >* blocks)
{
	double inblock[64];
	double outblock[64];

	int block_count = blocks->size();

	for (int block = 0; block < block_count; block++)
	{
		for (int i = 0; i < 64; i++) {
			inblock[i] = blocks->at(block).at(i);
		}

		fdct(inblock, outblock);
		for (int i = 0; i < 64; i++) {
			blocks->at(block).at(i) = outblock[i];
		}

	}

}

vector<vector<vector<double> > > encode_makeBlocks(Image* image)
{
	int hor_blocks = image->getWidth()/8;
	int ver_blocks = image->getHeight()/8;
	vector<vector<vector<double> > > components;
	vector<double> y_block(64);
	vector<double> cb_block(64);
	vector<double> cr_block(64);
	vector<vector<double> > y_blocks;
	vector<vector<double> > cb_blocks;
	vector<vector<double> > cr_blocks;
	int y = 0;
	int x = 0;
	int block_count = 0;
	//double outblock[64];
	//const double* inblock_p = &inblock[0];
	//double* outblock_p = &outblock[0];

	for (int ver = 0; ver < ver_blocks; ver++)
	{
		for (int hor = 0; hor < hor_blocks; hor++)
		{
			//To get one block
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					y_block.at(i*8+j) = image->getPixel(y+i, x+j, 0);
					cb_block.at(i*8+j) = image->getPixel(y+i, x+j, 1);
					cr_block.at(i*8+j) = image->getPixel(y+i, x+j, 2);
					//cout << "Saving data to inblock index: " << i*8+j << endl;
				}
			}

			//Add last block to blocks
			y_blocks.push_back(y_block);
			cb_blocks.push_back(cb_block);
			cr_blocks.push_back(cr_block);

			//Moving to next block in horizontal direction
			x += 8;
			block_count++;
		}

		//Moving to next block in vertical direction
		y += 8;
	}
	components.push_back(y_blocks);
	components.push_back(cb_blocks);
	components.push_back(cr_blocks);

	cout << "Amount of blocks: " << block_count << endl;
	return components;
}


bool encode(const char* inputfile, const char* outputfile, int width, int height, bool rgb) {
	
	int bytesperpixel = (rgb ? 3 : 1);
	bitstream* bs = open_input_bitstream(inputfile);
	//Hold image data
	Image* image = new Image(width, height, bytesperpixel);
	//Holds all the 8x8 blocks of the image
	//vector<vector<double> > blocks;

	//Checks that given inputfile exists
	if(bs == NULL) return NULL;

	// reading the image, offsetting
	for (int y = 0; y < width; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < bytesperpixel; c++) {
				image->setPixel( y, x, c, (signed int) (getbits(bs, 8) - 128) );
				if (bytesperpixel == 3 && c == 2) {
					// RGB2YUV transformation if input image is RGB image
					image->convertPixelRGB2YUV(y, x);
				}
			}
		}
	}
	
	vector<vector<vector<double> > > components = encode_makeBlocks(image);
	cout << "Some value from components: " << components.at(0).at(456).at(34) << endl;
	vector<vector<double> > blocks = components.at(0);
	encode_dct(&blocks);

	encode_quantize(&blocks);
	vector<vector<double> > ac_values;

	//Copy quantized values to quant_values for not to mess them when separating AC/DC values
	vector<vector<double> > quant_values = blocks;
	vector<double> diff_values = encode_zigzag_diff(&quant_values, &ac_values);

	encode_write_bitstream(&diff_values, &ac_values, outputfile);


	return true;
}