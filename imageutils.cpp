#include "imageutils.hpp"

#include <cmath>
#include <random>

#include "constant.hpp"
#include "output.hpp"

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

gdImagePtr edgeDetectSobel(const gdImagePtr image) {
	return edgeDetect(image, _sobel);
}

gdImagePtr edgeDetectLaplace(const gdImagePtr image) {
	return edgeDetect(image, _laplace);
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
	return gdImageGetPixel(img, x, y);
}

unsigned char getGrey(const gdImagePtr img, int c) {
	const int r = gdImageRed(img, c);
	const int g = gdImageGreen(img, c);
	const int b = gdImageBlue(img, c);
	return (r+g+b)/3;
}

unsigned char getPixel(const gdImagePtr img, int x, int y, bool check) {
	const int c = check?wrappedGetPixel(img, x, y):gdImageGetPixel(img,x,y);
	return getGrey(img, c);
}

void setPixel(gdImagePtr img, int x, int y, unsigned char grey) {
	int c = gdTrueColor(grey,grey,grey);
	gdImageSetPixel(img, x, y, c);
}

gdImagePtr edgeDetect(const gdImagePtr image, int op(const unsigned char*)) {
	gdImagePtr result = gdImageCreateTrueColor(gdImageSX(image), gdImageSY(image));

	int width = gdImageSX(image);
	int height = gdImageSY(image);

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			unsigned char pix[9];
			pix[0] = getPixel(image, x-1, y-1);
			pix[1] = getPixel(image, x,   y-1);
			pix[2] = getPixel(image, x+1, y-1);
			pix[3] = getPixel(image, x-1, y  );
			pix[4] = getPixel(image, x,   y  );
			pix[5] = getPixel(image, x+1, y  );
			pix[6] = getPixel(image, x-1, y+1);
			pix[7] = getPixel(image, x,   y+1);
			pix[8] = getPixel(image, x+1, y+1);
			int c = op(pix);
			setPixel(result, x, y, boundColor(c));
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

	gdImagePtr result = gdImageCreateTrueColor(width, height);
//	gdImageFill(result, gdRedMax/2, gdGreenMax/2, gdBlueMax/2);

	for(int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			setPixel(result, x, y, (rand() % (gdRedMax/2)) + (gdRedMax/4) );
		}
	}

	return result;
}

void interpolateErrors(gdImagePtr image) {
	if (outputVerbose()) {
		output << "Interpolating to correct error pixels..." << endl;
	}
	bool success = true;
	for (int x = 0; x < gdImageSX(image); x++) {
		for (int y = 0; y < gdImageSY(image); y++) {
			if (gdImageGetPixel(image, x, y) == ERROR_COLOR) {
				success = (success && interpolatePixel(image, x, y));
			}
		}
	}
	if (!success) {
		if (outputVerbose()) {
			output << "Interpolation incomplete, interpolating again." << endl;
		}
		interpolateErrors(image);
	}
}

bool interpolatePixel(gdImagePtr image, int x, int y) {
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
		if (pix[i] != ERROR_COLOR) {
			numColored++;
			total += getGrey(image, pix[i]);
		}
	}

	if (numColored < PIXELS_FOR_INTERP) {
		return false;
	} else {
		setPixel(image, x, y, total/numColored);
		return true;
	}
}
