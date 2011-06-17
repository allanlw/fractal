#include "imageutils.hpp"

#include <cmath>

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

unsigned char getPixel(const gdImagePtr img, int x, int y, bool check) {
	if (check) {
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
	}
	int c = gdImageGetPixel(img, x, y);
	unsigned char r = gdImageRed(img, c);
	unsigned char g = gdImageGreen(img, c);
	unsigned char b = gdImageBlue(img, c);
	return (r+g+b)/3;
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
