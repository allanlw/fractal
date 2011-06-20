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

unsigned char outputLevel = 1;

std::ostream& output = std::cout;

static int width = DEFAULT_WIDTH;
static int height = DEFAULT_HEIGHT;
static int iterations = DEFAULT_ITERATIONS;
static double errorCutoff = DEFAULT_ERROR_CUTOFF;

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
	{"version", no_argument, 0, 'V'}
};

static const char* shortOptions = "vqedo:Hw:h:i:c:IV";

static int encodeImage(const char* in, const char* out);
static int decodeImage(const char* in, const char* out);
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
			errorCutoff *= errorCutoff;
			break;
		case 'I':
			mode = M_INFO;
			break;
		case 'V':
			mode = M_VERSION;
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
			result = decodeImage(argv[optind], outputFilename);
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

	FILE* inputHandle = fopen(in, "r");

	if (inputHandle == NULL) {
		if (outputError()) {
			output << "Could not open file " << in << ", dying." << endl;
		}
		return 1;
	}

	gdImagePtr lenna = gdImageCreateFromPng(inputHandle);
	fclose(inputHandle);

	if (lenna == NULL) {
		if (outputError()) {
			output << "Could not load png from " << in << ", dying." << endl;
		}
		return 1;
	}

	DoubleImage img(lenna);
	TriangleTree tree(img);

	gdFree(lenna);

	if (outputStd()) {
		output << in << " loaded, encoding..." << endl;
	}

	Triangle* cur;

	while((cur = tree.assignOne(errorCutoff)) != NULL) {
		if (outputVerbose()) {
			output << "Triangle #" << cur->getId();
			output << " assigned - "<< (cur->isTerminal()?"Terminal":"Not Terminal") << endl;
			output << tree.getUnassigned().size() << " unassigned / ";
			output << tree.getAllTriangles().size() << " total. (";
			output << (((double)tree.getUnassigned().size())/((double)tree.getAllTriangles().size())*100) << "%)" << endl;
		}
	}

	ofstream outStream("fractal.bin", ios_base::out | ios_base::trunc | ios_base::binary);
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

int decodeImage(const char * in, const char * out) {

	if (out == NULL) {
		out = DEFAULT_DEC_FNAME;
	}

	TriangleTree tree = TriangleTree::loadFractal(in, width, height);

	if (outputStd()) {
		output << "rendering fractal..." << endl;
	}

	for (int i = 0; i < iterations; i++) {
		if (outputVerbose()) {
			output << "evaulating iteration #" << i << endl;
		}
		tree.eval();
		if (false) {
			ostringstream s;
			s << "e" << i << ".png";
			string fname = s.str();
			output << "saving to " << fname << endl;
			FILE* oimg = fopen(fname.c_str(), "w");
			if (oimg == NULL) {
				output << "Could not load " << fname << ", dying." << endl;
				return 1;
			}
			gdImagePng(tree.getImage().getImage(), oimg);
			fclose(oimg);
		}
		if (outputVerbose()) {
			output << "iteration #" << i <<" done." << endl;
		}
	}

	if (outputStd()) {
		output << "rendering done, saving to " << out << "..." << endl;
	}

	FILE* outputImg = fopen(out, "w");
	if (outputImg == NULL) {
		if (outputError()) {
			output << "could not open " << out << " for writing." << endl;
		}
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
	printVersion();
	if (outputStd()) {
		output << "This program is used for encoding and decoding triangular fractal images." << endl;
		output << endl;
		output << "Usage: fractal [-Hvqed] [-o output] [-w width] [-h height] [-c cutoff] [-i iterations] input" << endl;
		output << "\t-H, --help           Print this help message." << endl;
		output << "\t-V, --version        Print version information." << endl;
		output << "\t-v, --verbose        Print verbose output (twice for debug)." << endl;
		output << "\t-q, --quiet          Surpress all output." << endl;
		output << "\t-e, --encode         Encode the input image (png) into a fractal representation." << endl;
		output << "\t-d, --decode         Decode the input fractal into a raster representation (png)." << endl;
		output << "\t-o, --output=fname   Output to fname. This could be either a fractal or a raster." << endl;
		output << "\t-w, --width=num      Set the output width to num." << endl;
		output << "\t-h, --height=num     Set the output height to num." << endl;
		output << "\t-i, --iterations=num Set the number of iterations for decoding to num." << endl;
		output << "\t-c, --cutoff=float   Set the error cutoff to float (rms intensity difference)." << endl;
		output << "\t-I, --info           Prints information about the input fractal." << endl;
	}
	return 0;
}

int infoImage(const char* in) {
	TriangleTree tree = TriangleTree::loadFractal(in, width, height);
	return 0;
}

int printVersion() {
	if (outputStd()) {
		output << name << " " << version << endl;
	}
	return 0;
}
