#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include "gd.h"
#include "getopt.h"

#include "doubleimage.hpp"
#include "triangletree.hpp"
#include "constant.hpp"
#include "imageutils.hpp"
#include "output.hpp"

using namespace std;

unsigned char outputLevel = DEFAULT_OUTPUT_LEVEL;

std::ostream& output = DEFAULT_OUTPUT_STREAM;

static int width = DEFAULT_WIDTH;
static int height = DEFAULT_HEIGHT;
static int iterations = DEFAULT_ITERATIONS;
static double errorCutoff = DEFAULT_ERROR_CUTOFF;
static DoubleImage::SamplingType sType = DEFAULT_SAMPLING_TYPE;
static bool fixErrors = DEFAULT_FIX_ERRORS;

static const char* name = "Fractal Image Compressor";

static const char* version = "0.1a";

static const struct option longOptions[] = {
	{"verbose", no_argument, 0, 'v'},
	{"quiet", no_argument, 0, 'q'},
	{"encode", no_argument, 0, 'e'},
	{"decode", no_argument, 0, 'd'},
	{"output", required_argument, 0, 'o'},
	{"help", no_argument, 0, 'H'},
	{"width", required_argument, 0, 'w'},
	{"height", required_argument, 0, 'h'},
	{"iterations", required_argument, 0, 'i'},
	{"cutoff", required_argument, 0, 'c'},
	{"info", no_argument, 0, 'I'},
	{"version", no_argument, 0, 'V'},
	{"subsample", no_argument, 0, '1'},
	{"supersample", no_argument, 0, '2'},
	{"bothsample", no_argument, 0, '3'},
	{"fixerrors", no_argument, 0, '4'},
	{"no-fixerrors", no_argument, 0, '5'},
	{"seed", required_argument, 0, 's'}
};

static const char* shortOptions = "vqedo:Hw:h:i:c:IVs:";

static int encodeImage(const char* in, const char* out);
static int decodeImage(const char* in, const char* out, const char* seed);
static int printHelp();
static int printVersion();
static int infoImage(const char* in);

int main(int argc, char* argv[]) {
	enum op_mode {
		M_UNDEF,
		M_ENCODE,
		M_DECODE,
		M_HELP,
		M_INFO,
		M_VERSION
	} mode = M_UNDEF;
	char * outputFilename = NULL;
	char * seed = NULL;
	int optIndex = 0;
	int c;

	while ((c = getopt_long(argc, argv,
	              shortOptions, longOptions, &optIndex)) != -1) {
		switch (c) {
		case 'v':
			outputLevel++;
			break;
		case 'q':
			if (outputLevel != 0) {
				outputLevel--;
			}
			break;
		case 'e':
			mode = M_ENCODE;
			break;
		case 'd':
			mode = M_DECODE;
			break;
		case 'H':
			mode = M_HELP;
			break;
		case 'o':
			outputFilename = optarg;
			break;
		case 'w':
			width = atoi(optarg);
			break;
		case 'h':
			height = atoi(optarg);
			break;
		case 'i':
			iterations = atoi(optarg);
			break;
		case 'c':
			errorCutoff = atof(optarg);
			break;
		case 'I':
			mode = M_INFO;
			break;
		case 'V':
			mode = M_VERSION;
			break;
		case '1':
			sType = DoubleImage::T_SUBSAMPLE;
			break;
		case '2':
			sType = DoubleImage::T_SUPERSAMPLE;
			break;
		case '3':
			sType = DoubleImage::T_BOTHSAMPLE;
			break;
		case '4':
			fixErrors = true;
			break;
		case '5':
			fixErrors = false;
			break;
		case 's':
			seed = optarg;
			break;
		default:
		case '?':
			return 1;
			break;
		}
	}

	int result = 0;

	switch (mode) {
	case M_UNDEF:
		if (outputError()) {
			output << "No operation specified. Printing Help." << endl;
		}
	case M_HELP:
		result = printHelp();
		break;
	case M_ENCODE:
		if (optind == argc) {
			if (outputError()) {
				output << "Encoding requires a source image." << endl;
			}
			result = 1;
		} else {
			result = encodeImage(argv[optind], outputFilename);
		}
		break;
	case M_DECODE:
		if (optind == argc) {
			if (outputError()) {
				output << "Decoding requires a source fractal." << endl;
			}
			result = 1;
		} else {
			result = decodeImage(argv[optind], outputFilename, seed);
		}
		break;
	case M_INFO:
		if (optind == argc) {
			if (outputError()) {
				output << "Info requires a source fractal." << endl;
			}
			result = 1;
		} else {
			result = infoImage(argv[optind]);
		}
		break;
	case M_VERSION:
		result = printVersion();
		break;
	}
	output.flush();
	return result;
}

int encodeImage(const char* in, const char* out) {
	if (out == NULL) {
		out = DEFAULT_ENC_FNAME;
	}

	gdImagePtr lenna;

	try {
		lenna = loadImage(in);
	} catch (const runtime_error& e) {
		openError(in, e.what());
		return 1;
	}

	DoubleImage img(lenna);
	TriangleTree tree(img);

	gdFree(lenna);

	if (outputStd()) {
		output << in << " loaded, encoding..." << endl;
	}

	Triangle* cur;

	while((cur = tree.assignOne(errorCutoff*errorCutoff)) != NULL) {
		if (outputVerbose()) {
			output << "Triangle #" << cur->getId();
			output << " assigned - "<< (cur->isTerminal()?"Terminal":"Not Terminal") << endl;
			output << tree.getUnassigned().size() << " unassigned / ";
			output << tree.getAllTriangles().size() << " total. (";
			output << (((double)tree.getUnassigned().size())/((double)tree.getAllTriangles().size())*100) << "%)" << endl;
		}
	}

	ofstream outStream(out, ios_base::out | ios_base::trunc | ios_base::binary);

	if (!outStream.good()) {
		openError(out);
		return 1;
	}

	if (outputStd()) {
		output << "Saving fractal (" << tree.getLastId() << " triangles) to " << out << "." << endl;
	}

	tree.serialize(outStream);
	outStream.close();

	if (outputStd()) {
		output << "Done." << endl;
	}
	return 0;
}

int decodeImage(const char * in, const char * out, const char* seed) {
	if (out == NULL) {
		out = DEFAULT_DEC_FNAME;
	}

	if (outputStd()) {
		output << "Loading fractal " << in << "..." << endl;
	}

	ifstream inStream(in, ios_base::in | ios_base::binary);

	if (!inStream.good()) {
		openError(in);
		return 1;
	}

	gdImagePtr seedImage = NULL;

	if (seed == NULL) {
		inStream.seekg (0, ios::end);
		unsigned long length = inStream.tellg();
		inStream.seekg (0, ios::beg);

		seedImage = blankCanvas(width, height, length);
	} else {
		gdImagePtr temp = NULL;
		try {
			temp = loadImage(seed);
		} catch (const runtime_error& e) {
			openError(seed, e.what());
			return 1;
		}

		seedImage = gdImageCreateTrueColor(width, height);

		gdImageCopyResampled(seedImage, temp, 0, 0, 0, 0,
		                     gdImageSX(seedImage), gdImageSY(seedImage),
		                     gdImageSX(temp), gdImageSY(temp));
		gdFree(temp);
	}

	TriangleTree tree(DoubleImage(seedImage), inStream);

	gdFree(seedImage);
	inStream.close();

	if (outputStd()) {
		output << "fractal loaded. (" << tree.getAllTriangles().size() << " triangles)" << endl;

		output << "rendering fractal..." << endl;
	}

	for (int i = 0; i < iterations; i++) {
		if (outputVerbose()) {
			output << "evaulating iteration #" << i << endl;
		}
		tree.eval(sType, fixErrors);
		if (false) {
			ostringstream s;
			s << "e" << i << ".png";
			string fname = s.str();
			output << "saving to " << fname << endl;
			FILE* oimg = fopen(fname.c_str(), "w");
			if (oimg == NULL) {
				openError(fname);
				return 1;
			}
			gdImagePng(tree.getImage().getImage(), oimg);
			fclose(oimg);
		}
		if (outputVerbose()) {
			output << "Iteration #" << i <<" done." << endl;
		}
	}

	if (outputStd()) {
		output << "Rendering done, saving to " << out << "..." << endl;
	}

	FILE* outputImg = fopen(out, "w");

	if (outputImg == NULL) {
		openError(out);
		return 1;
	}

	gdImagePng(tree.getImage().getImage(), outputImg);
	fclose(outputImg);
	if (outputStd()) {
		output << "Done." << endl;
	}
	return 0;
}

int printHelp() {
	output << name << " " << version << endl;
	output << "This program is used for encoding and decoding triangular fractal images." << endl;
	output << endl;
	output << "Usage: fractal [OPTIONS] [INPUTFILE]" << endl;
	output << "General Options:" << endl;
	output << "  -H, --help           Print this help message." << endl;
	output << "  -V, --version        Print version information." << endl;
	output << "  -e, --encode         Encode the input image (png) into a fractal." << endl;
	output << "  -d, --decode         Decode the input fractal into a raster (png)." << endl;
	output << "  -I, --info           Prints information about the input fractal." << endl;
	output << "  -o, --output=fname   Output file." << endl;
	output << "  -v, --verbose        Print verbose output (twice for debug)." << endl;
	output << "  -q, --quiet          Surpress all output." << endl;
	output << endl;
	output << "Decoding Options:" << endl;
	output << "  -w, --width=num      Set the output width in pixels. Default: " << DEFAULT_WIDTH << endl;
	output << "  -h, --height=num     Set the output height in pixels. Default: " << DEFAULT_HEIGHT <<  endl;
	output << "  -s, --seed=fname     Specify a custom seed image. (resized to w,h)" << endl;
	output << "  -i, --iterations=num Set the number of iterations for decoding. Default: " << DEFAULT_ITERATIONS << endl;
	output << "      --subsample      Subsamples - Few Errors";
	if (DEFAULT_SAMPLING_TYPE == DoubleImage::T_SUBSAMPLE) {
		output << " This is the default.";
	}
	output << endl;
	output << "      --supersample    Supersamples - Lots of errors.";
	if (DEFAULT_SAMPLING_TYPE == DoubleImage::T_SUPERSAMPLE) {
		output << " This is the default.";
	}
	output << endl;
	output << "      --bothsample     Sub and Supersamples. Highest Quality.";
	if (DEFAULT_SAMPLING_TYPE == DoubleImage::T_BOTHSAMPLE) {
		output << " This is the default.";
	}
	output << endl;
	output << "      --(no-)fixerrors Interpolate (or not) to fix errors. Default is " << (DEFAULT_FIX_ERRORS?"fix":"don't fix") << "." << endl;
	output << endl;
	output << "Encoding Options:" << endl;
	output << "  -c, --cutoff=float   Set the error cutoff (rms intensity). Default: " << DEFAULT_ERROR_CUTOFF << endl;
	output << endl;
	output << "Please report bugs to: <allanlw@gmail.com>" << endl;
	return 0;
}

int infoImage(const char* in) {
	if (outputStd()) {
		output << "loading fractal " << in << "..." << endl;
	}

	ifstream inStream(in, ios_base::in | ios_base::binary);

	if (!inStream.good()) {
		openError(in);
		return 1;
	}

	TriangleTree tree(inStream);

	inStream.close();

	if (outputStd()) {
		output << "fractal loaded. (" << tree.getAllTriangles().size() << " triangles)" << endl;
	}

	return 0;
}

int printVersion() {
	output << name << " " << version << endl;
	output << "Copyright (C) 2011 Allan Wirth" << endl;
	output << "License GPLv3: GNU GPL version 3 <http://gnu.org/licenses/gpl.html>" << endl;
	output << "This is free software: you are free to change and redistribute it." << endl;
	output << "There is NO WARRANTY, to the extent permitted by law." << endl;
	return 0;
}
