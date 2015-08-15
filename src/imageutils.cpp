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
#include "imageutils.h"

#include <cmath>
#include <random>
#include <cstdio>
#include <stdexcept>

#include "constant.h"
#include "output.h"

using namespace std;

static int _sobel(const unsigned char *data) {
	const int v_kernel[9] = {-1,  0,  1,
	                         -2,  0,  2,
	                         -1,  0,  1 };
	const int h_kernel[9] = {-1, -2, -1,
	                          0,  0,  0,
	                          1,  2,  1 };
	int v_grad = 0;
	int h_grad = 0;

	for (size_t i = 0; i < 9; i++) {
		v_grad += v_kernel[i] * data[i];
		h_grad += h_kernel[i] * data[i];
	}

	return sqrt ( v_grad * v_grad + h_grad * h_grad) ;
}

static int _laplace (const unsigned char* data) {
	const int kernel[9] = { 1,  1,  1,
	                        1, -8,  1,
	                        1,  1,  1 };
	int grad = 0;

	for (size_t i = 0; i < 9; i++) {
		grad += kernel[i] * data[i];
	}

	return grad;
}

gdImagePtr edgeDetectSobel(const gdImagePtr image, Channel channel) {
	return edgeDetect(image, _sobel, channel);
}

gdImagePtr edgeDetectLaplace(const gdImagePtr image, Channel channel) {
	return edgeDetect(image, _laplace, channel);
}

int wrappedGetPixel(const gdImagePtr img, int x, int y) {
	if (x < 0) {
		x += gdImageSX(img);
	}
	if (x >= gdImageSX(img)) {
		x -= gdImageSX(img);
	}
	if (y < 0) {
		y += gdImageSY(img);
	}
	if (y >= gdImageSY(img)) {
		y -= gdImageSY(img);
	}
	return gdImageTrueColorPixel(img, x, y);
}

unsigned char getGrey(const gdImagePtr img, int c) {
	const int r = gdImageRed(img, c);
	const int g = gdImageGreen(img, c);
	const int b = gdImageBlue(img, c);
	return (r+g+b)/3;
}

unsigned char getColor(const gdImagePtr img, int c, Channel channel) {
	switch(channel) {
	default:
	case C_GREY:
		return getGrey(img, c);
		break;
	case C_RED:
		return gdImageRed(img, c);
		break;
	case C_GREEN:
		return gdImageGreen(img, c);
		break;
	case C_BLUE:
		return gdImageBlue(img, c);
		break;
	}
}

unsigned char getPixel(const gdImagePtr img, int x, int y, Channel channel, bool check) {
	int c;
	if (check) {
		c = wrappedGetPixel(img, x, y);
	} else {
		c = gdImageTrueColorPixel(img, x, y);
	}
	return getColor(img, c, channel);
}

void setPixel(gdImagePtr img, int x, int y, unsigned char value, Channel channel, unsigned char alpha) {
	int c;
	switch(channel) {
	default:
	case C_GREY:
		c = gdTrueColorAlpha(value, value, value, alpha);
		break;
	case C_RED:
		c = gdImageGetPixel(img, x, y);
		c = gdTrueColorAlpha(value, gdImageGreen(img, c), gdImageBlue(img, c), alpha);
		break;
	case C_GREEN:
		c = gdImageGetPixel(img, x, y);
		c = gdTrueColorAlpha(gdImageRed(img, c), value, gdImageBlue(img, c), alpha);
		break;
	case C_BLUE:
		c = gdImageGetPixel(img, x, y);
		c = gdTrueColorAlpha(gdImageRed(img, c), gdImageGreen(img, c), value, alpha);
		break;
	}
	gdImageSetPixel(img, x, y, c);
}

gdImagePtr edgeDetect(const gdImagePtr image, int op(const unsigned char*), Channel channel) {
	gdImagePtr result = gdImageCreateTrueColor(gdImageSX(image), gdImageSY(image));

	int width = gdImageSX(image);
	int height = gdImageSY(image);

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			unsigned char pix[9];
			pix[0] = getPixel(image, x-1, y-1, channel);
			pix[1] = getPixel(image, x,   y-1, channel);
			pix[2] = getPixel(image, x+1, y-1, channel);
			pix[3] = getPixel(image, x-1, y  , channel);
			pix[4] = getPixel(image, x,   y  , channel);
			pix[5] = getPixel(image, x+1, y  , channel);
			pix[6] = getPixel(image, x-1, y+1, channel);
			pix[7] = getPixel(image, x,   y+1, channel);
			pix[8] = getPixel(image, x+1, y+1, channel);
			int c = op(pix);
			setPixel(result, x, y, boundColor(c), C_GREY);
		}
	}
	return result;
}

void clearAlpha(gdImagePtr img) {
	for (int x = 0; x < gdImageSX(img); x++) {
		for (int y = 0; y < gdImageSY(img); y++) {
			int oldC = gdImageGetPixel(img, x, y);
			gdImageSetPixel(img, x, y, gdTrueColorAlpha(gdImageRed(img, oldC),
			                gdImageGreen(img, oldC), gdImageBlue(img, oldC),
			                gdAlphaOpaque));
		}
	}
}

gdImagePtr blankCanvas(int width, int height, unsigned long seed) {
	std::mt19937 rand;

	rand.seed(seed);

	gdImagePtr result = gdImageCreateTrueColor(width, height);

	for(int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			setPixel(result, x, y, (rand() % (gdRedMax/2)) + (gdRedMax/4), C_GREY);
		}
	}

	return result;
}

void interpolateErrors(gdImagePtr image, Channel channel) {
	if (outputDebug()) {
		output << "Interpolating to correct error pixels..." << endl;
	}
	bool success = true;
	for (int x = 0; x < gdImageSX(image); x++) {
		for (int y = 0; y < gdImageSY(image); y++) {
			if (gdImageAlpha(image, gdImageGetPixel(image, x, y)) == gdAlphaOpaque) {
				success = (success && interpolatePixel(image, x, y, channel));
			}
		}
	}
	if (!success) {
		if (outputDebug()) {
			output << "Interpolation incomplete, interpolating again." << endl;
		}
		interpolateErrors(image, channel);
	}
}

bool interpolatePixel(gdImagePtr image, int x, int y, Channel channel) {
	const int pix[8] = {
		wrappedGetPixel(image, x-1, y-1),
		wrappedGetPixel(image, x,   y-1),
		wrappedGetPixel(image, x+1, y-1),
		wrappedGetPixel(image, x-1, y  ),
		wrappedGetPixel(image, x+1, y  ),
		wrappedGetPixel(image, x-1, y+1),
		wrappedGetPixel(image, x,   y+1),
		wrappedGetPixel(image, x+1, y+1)
	};

	size_t total = 0;
	size_t numColored = 0;

	for (size_t i = 0; i < 8; i++) {
		if (gdImageAlpha(image, pix[i]) != gdAlphaOpaque) {
			numColored++;
			total += getColor(image, pix[i], channel);
		}
	}

	if (numColored < PIXELS_FOR_INTERP) {
		return false;
	} else {
		setPixel(image, x, y, total/numColored, channel);
		return true;
	}
}

gdImagePtr loadImage(const char* fName) {
	if (outputDebug()) {
		output << "Opening " << fName << " for reading." << endl;
	}
	FILE* inputHandle = fopen(fName, "r");

	if (inputHandle == NULL) {
		throw runtime_error("Coud not open file for reading.");
	}

	gdImagePtr lenna = gdImageCreateFromPng(inputHandle);

	fclose(inputHandle);
	return lenna;
}

string channelToString(Channel channel) {
	switch(channel) {
	default:
	case C_GREY:
		return "grey";
		break;
	case C_RED:
		return "red";
		break;
	case C_BLUE:
		return "blue";
		break;
	case C_GREEN:
		return "green";
		break;
	}
}
