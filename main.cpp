/*
 * Copyright (c) 2011 Allan Wirth <allanlw@gmail.com>
 *
 * This file is part of Fractal Image Compressor.
 *
 * Fractal Image Compressor is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fractal Image Compressor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Fractal Image Compressor.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include "gd.h"
#include "getopt.h"

#include "doubleimage.h"
#include "constant.h"
#include "imageutils.h"
#include "output.h"
#include "fractalimage.h"
#include "ioutils.h"

using namespace std;

signed char outputLevel = DEFAULT_OUTPUT_LEVEL;

std::ostream& output = DEFAULT_OUTPUT_STREAM;

static int width = DEFAULT_WIDTH;
static int height = DEFAULT_HEIGHT;
static int iterations = DEFAULT_ITERATIONS;
static double errorCutoff = DEFAULT_ERROR_CUTOFF;
static DoubleImage::SamplingType sType = DEFAULT_SAMPLING_TYPE;
static bool fixErrors = DEFAULT_FIX_ERRORS;
static FractalImage::ImageType colorMode = DEFAULT_COLOR_MODE;
static DoubleImage::DivisionType dType = DEFAULT_DIVISION_TYPE;
static DoubleImage::Metric metric = DEFAULT_METRIC;
static TriangleTree::SubdivisionMethod sMethod = DEFAULT_SUBDIVISION_METHOD;
static DoubleImage::EdgeDetectionMethod edMethod = DEFAULT_EDGE_DETECTION_METHOD;

static const char* name = "Fractal Image Compressor";

static const char* version = "0.1";

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
	{"sample", required_argument, 0, '1'},
	{"fixerrors", no_argument, 0, '4'},
	{"no-fixerrors", no_argument, 0, '5'},
	{"seed", required_argument, 0, 's'},
	{"color", no_argument, 0, 'C'},
	{"greyscale", no_argument, 0, 'G'},
	{"split", required_argument, 0, '2'},
	{"metric", required_argument, 0, '3'},
	{"subdivide", required_argument, 0, '6'},
	{"edges", required_argument, 0, '7'}
};

static const char* shortOptions = "vqedo:Hw:h:i:c:IVs:CG";

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
		case 'I':
			mode = M_INFO;
			break;
		case 'V':
			mode = M_VERSION;
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
		case '1': {
			string arg(optarg);
			if (arg == "sub") {
				sType = DoubleImage::T_SUBSAMPLE;
			} else if (arg == "super") {
				sType = DoubleImage::T_SUPERSAMPLE;
			} else if (arg == "both") {
				sType = DoubleImage::T_BOTHSAMPLE;
			} else {
				if (outputError()) {
					output << "Invalid sample type." << endl;
				}
			}
			break;
		}
		case '2': {
			string arg(optarg);
			if (arg == "high") {
				dType = DoubleImage::T_HIGHENTROPY;
			} else if (arg == "low") {
				dType = DoubleImage::T_LOWENTROPY;
			} else if (arg == "mid") {
				dType = DoubleImage::T_MIDDLE;
			} else {
				if (outputError()) {
					output << "Invalid split value." << endl;
				}
			}
			break;
		}
		case '3': {
			string arg(optarg);
			if (arg == "rms") {
				metric = DoubleImage::M_RMS;
			} else if (arg == "sup") {
				metric = DoubleImage::M_SUP;
			} else {
				if (outputError()) {
					output << "Invalid metric." << endl;
				}
			}
			break;
		}
		case '6': {
			string arg(optarg);
			if (arg == "quad") {
				sMethod = TriangleTree::M_QUAD;
			} else if (arg == "centeroid") {
				sMethod = TriangleTree::M_CENTEROID;
			} else {
				if (outputError()) {
					output << "Invalid subdivision method." << endl;
				}
			}
			break;
		}
		case '7': {
			string arg(optarg);
			if (arg == "sobel") {
				edMethod = DoubleImage::M_SOBEL;
			} else if (arg == "laplace") {
				edMethod = DoubleImage::M_LAPLACE;
			} else {
				if (outputError()) {
					output << "Invalid edge detection method." << endl;
				}
			}
			break;
		}
		case '4':
			fixErrors = true;
			break;
		case '5':
			fixErrors = false;
			break;
		case 's':
			seed = optarg;
			break;
		case 'C':
			colorMode = FractalImage::T_COLOR;
			break;
		case 'G':
			colorMode = FractalImage::T_GREYSCALE;
			break;
		default:
		case '?':
			return 1;
			break;
		}
	}

	int result = 0;

	switch (mode) {
	default:
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
			for (; optind < argc; optind++) {
				result = encodeImage(argv[optind], outputFilename);
			}
		}
		break;
	case M_DECODE:
		if (optind == argc) {
			if (outputError()) {
				output << "Decoding requires a source fractal." << endl;
			}
			result = 1;
		} else {
			for(; optind < argc; optind++) {
				result = decodeImage(argv[optind], outputFilename, seed);
			}
		}
		break;
	case M_INFO:
		if (optind == argc) {
			if (outputError()) {
				output << "Info requires a source fractal." << endl;
			}
			result = 1;
		} else {
			for(; optind < argc; optind++) {
				result = infoImage(argv[optind]);
			}
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

	DoubleImage img(lenna, sType, dType, metric, edMethod);
	FractalImage fractal(img, colorMode);
	gdFree(lenna);

	fractal.setSubdivisionMethod(sMethod);

	fractal.getMetadata().setSourceFilename(getBasename(in));

	if (outputStd()) {
		output << in << " loaded, encoding..." << endl;
	}

	fractal.encode(errorCutoff);

	ofstream outStream(out, ios_base::out | ios_base::trunc | ios_base::binary);

	if (!outStream.good()) {
		openError(out);
		return 1;
	}

	if (outputStd()) {
		output << "Saving fractal (" << fractal.getSize() << " triangles) to " << out << "." << endl;
	}

	fractal.serialize(outStream);
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

	DoubleImage img(seedImage, sType, dType, metric, edMethod);

	FractalImage fractal(inStream, img);

	gdFree(seedImage);
	inStream.close();

	if (outputStd()) {
		output << "fractal loaded. (" << fractal.getSize() << " triangles)" << endl;

		output << "rendering fractal..." << endl;
	}

	for (int i = 1; i <= iterations; i++) {
		if (outputVerbose()) {
			output << "evaulating iteration #" << i << endl;
		}
		DoubleImage result(fractal.decode(fixErrors), sType, dType, metric, edMethod);
		fractal.setImage(result);
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
			gdImagePng(fractal.getImage().getImage(), oimg);
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

	gdImagePng(fractal.getImage().getImage(), outputImg);
	fclose(outputImg);

	if (outputStd()) {
		output << "Done." << endl;
	}
	return 0;
}

int printHelp() {
	const string defaultMsg = " This is the default.";

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
	output << "      --sample=type    Sets sampling mode. Options are:" << endl;
	output << "                         \"sub\" - Subsampling, few errors.";
	if (DEFAULT_SAMPLING_TYPE == DoubleImage::T_SUBSAMPLE) {
		output << defaultMsg;
	}
	output << endl;
	output << "                         \"super\" - Supersampling, lots of errors.";
	if (DEFAULT_SAMPLING_TYPE == DoubleImage::T_SUPERSAMPLE) {
		output << defaultMsg;
	}
	output << endl;
	output << "                         \"both\" - sub and supersamples. Highest Quality.";
	if (DEFAULT_SAMPLING_TYPE == DoubleImage::T_BOTHSAMPLE) {
		output << defaultMsg;
	}
	output << endl;
	output << endl;
	output << "Decoding Options:" << endl;
	output << "  -w, --width=num      Set the output width in pixels. Default: " << DEFAULT_WIDTH << endl;
	output << "  -h, --height=num     Set the output height in pixels. Default: " << DEFAULT_HEIGHT <<  endl;
	output << "  -s, --seed=fname     Specify a custom seed image. (resized to w,h)" << endl;
	output << "  -i, --iterations=num Set the number of iterations for decoding. Default: " << DEFAULT_ITERATIONS << endl;
	output << "      --(no-)fixerrors Interpolate (or not) to fix errors. Default is " << (DEFAULT_FIX_ERRORS?"fix":"don't fix") << "." << endl;
	output << endl;
	output << "Encoding Options:" << endl;
	output << "  -c, --cutoff=float   Set the error cutoff (rms intensity). Default: " << DEFAULT_ERROR_CUTOFF << endl;
	output << "  -C, --color          Encode in RGB colorspace.";
	if (DEFAULT_COLOR_MODE == FractalImage::T_COLOR) {
		output << defaultMsg;
	}
	output << endl;
	output << "  -G, --greyscale      Encode in greyscale.";
	if (DEFAULT_COLOR_MODE == FractalImage::T_GREYSCALE) {
		output << defaultMsg;
	}
	output << endl;
	output << "      --split=type     Sets the triangle splitting type. Options are:" << endl;
	output << "                         \"high\" - split on high entropy.";
	if (DEFAULT_DIVISION_TYPE == DoubleImage::T_HIGHENTROPY) {
		output << defaultMsg;
	}
	output << endl;
	output << "                         \"low\" - split on low entropy.";
	if (DEFAULT_DIVISION_TYPE == DoubleImage::T_LOWENTROPY) {
		output << defaultMsg;
	}
	output << endl;
	output << "                         \"mid\" - split in the middle.";
	if (DEFAULT_DIVISION_TYPE == DoubleImage::T_MIDDLE) {
		output << defaultMsg;
	}
	output << endl;
	output << "      --metric=func    Sets the metric function. Options are:" << endl;
	output << "                         \"rms\" - Root Mean Square metric.";
	if (DEFAULT_METRIC == DoubleImage::M_RMS) {
		output << defaultMsg;
	}
	output << endl;
	output << "                         \"sup\" - Supremum metric.";
	if (DEFAULT_METRIC == DoubleImage::M_SUP) {
		output << defaultMsg;
	}
	output << endl;
	output << "      --subdivide=meth Sets the subdivision method. Options are:" << endl;
	output << "                         \"quad\" - Divide into fourths.";
	if (DEFAULT_SUBDIVISION_METHOD == TriangleTree::M_QUAD) {
		output << defaultMsg;
	}
	output << endl;
	output << "                         \"centeroid\" - Divide into thirds.";
	if (DEFAULT_SUBDIVISION_METHOD == TriangleTree::M_CENTEROID) {
		output << defaultMsg;
	}
	output << endl;
	output << "      --edges=func     Sets the edge detection method. Options are:" << endl;
	output << "                         \"sobel\" - Sobel filter.";
	if (DEFAULT_EDGE_DETECTION_METHOD == DoubleImage::M_SOBEL) {
		output << defaultMsg;
	}
	output << endl;
	output << "                         \"laplace\" - Laplace filter.";
	if (DEFAULT_EDGE_DETECTION_METHOD == DoubleImage::M_LAPLACE) {
		output << defaultMsg;
	}
	output << endl;
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

	FractalImage fractal(inStream, DoubleImage());

	inStream.close();

	output << "fractal loaded. (" << fractal.getSize() << " triangles)" << endl;

	output << "Metadata:" << endl;

	output << "    Width:" << fractal.getMetadata().getWidth() << endl;
	output << "    Height:" << fractal.getMetadata().getHeight() << endl;
	output << "    Filename:" << fractal.getMetadata().getSourceFilename() << endl;
	output << endl;

	output << "Num Channels: " << fractal.getChannels().size() << endl;
	for (vector<TriangleTree*>::const_iterator it = fractal.getChannels().begin(); it != fractal.getChannels().end(); it++) {
		output << "Channel " << channelToString((*it)->getChannel()) << ":" << endl;
		output << "    Num Triangles: " << (*it)->getAllTriangles().size() << endl;
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
